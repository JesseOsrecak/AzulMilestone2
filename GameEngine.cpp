
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "GameAction.h"
#include "GameEngine.h"
#include "GameTurn.h"
#include "IOHandler.h"
#include "Menu.h"
#include "ModelBuilder.h"
#include "Types.h"

#include <random>

using std::make_shared;
using std::map;
using std::numeric_limits;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;

GameEngine::GameEngine() :
    gameModel(make_shared<GameModel>()),
    ioHandler(make_shared<IOHandler>()),
    menu(make_shared<Menu>()),
    seed(numeric_limits<int>::min()),
    inProgress(false),
    inMenu(false)
{}

GameEngine::~GameEngine() {}

void GameEngine::setSeed(int seed) {
    this->seed = seed;
}

void GameEngine::run() {
    bool exit = false;

    while (!exit) {
        if (inProgress) {
            printPreTurnInfo();
        } else {
            printMenu();
        }

        GameAction action = promptForAction();

        // Do the thing the player wants
        performGameAction(action);

        if (action.type() == EXIT) {
            exit = true;
        }
    }
}

void GameEngine::printBanner() {
    ioHandler->printToStdOut(menu->getBanner());
}

void GameEngine::printCredits() {
    printBanner();
    ioHandler->printToStdOut(menu->getCredits());
}

void GameEngine::printMenu() {
    ioHandler->printToStdOut(menu->getOptions());
    inMenu = true;
}

void GameEngine::printPreTurnInfo() {
    // Print the centre table
    ioHandler->printToStdOut("\nTable Centre\nC: " + gameModel->getTableCentre(0)->getPrintable() + "\n");

    if(gameModel->getNumberOfCentreFactories() == 2)
    {
        ioHandler->printToStdOut("D: " + gameModel->getTableCentre(1)->getPrintable() + "\n");
    }

    if(gameModel->isFirst())
    {
        ioHandler->printToStdOut( Tile::colouredToString(FIRST) + "\n\n");
    }
    else
    {
        ioHandler->printToStdOut("\n");
    }
    
    // Print the factories
    ioHandler->printToStdOut("Factories\n");
    for (unsigned int i = 0; i != gameModel->getNumberOfFactories(); ++i) {
        ioHandler->printToStdOut(to_string(i + 1) + ": " + gameModel->getFactory(i)->getPrintable() + "\n");
    }

    // Print the current player
    ioHandler->printToStdOut("\nCurrent Player\n");
    ioHandler->printToStdOut(gameModel->getCurrentPlayer()->getPrintable() + "\n\n");
    
}

GameAction GameEngine::promptForAction() {
    GameAction gameAction = UNKNOWN;
    std::string input = "";

    // Get selection from user
    while (gameAction.type() == UNKNOWN) {
        // Show a prompt
        ioHandler->printToStdOut(menu->getUserPrompt());
        ioHandler->readFromStdIn(input);
        if (ioHandler->eof()) {
            // ctrl-d
            gameAction = EXIT;
        } else {
            gameAction = parseInput(input);
            if (gameAction.type() == UNKNOWN) {
                ioHandler->printToStdOut("Invalid option.\n");
            }
        }
    }

    // If this was a menu prompt, exit the menu
    if (inMenu) {
        inMenu = false;
    }

    return gameAction;
}

GameAction GameEngine::parseInput(const std::string input) {
    GameAction action = UNKNOWN;

    if (inMenu) {
        if (input == MENU_NEW) {
            action = NEW;
        } else if (input == MENU_LOAD) {
            action = LOAD;
        } else if (input == MENU_SAVE) {
            action = SAVE;
        } else if (input == MENU_CREDITS) {
            action = CREDITS;
        } else if (input == MENU_EXIT) {
            action = EXIT;
        }
    } else {
        // try parsing as game command
        
        action = createGameTurn(input);

        
        if (action.type() == UNKNOWN) 
        {
            action = createShowAction(input);
            // May still be a valid command
            if (action.type() == UNKNOWN) {
                
                if (input == "q" ||
                    input == "quit" ||
                    input == "exit" ||
                    input == "x") {
                    action = EXIT;
                } else if (input == "h" ||
                        input == "help" ||
                        input == "?") {
                    action = SHOW_COMMANDS;
                } else if (input == "m" ||
                        input == "menu") {
                    action = SHOW_MENU;
                } else if (input == "s" ||
                        input == "save") {
                    action = SAVE;
                }
            }
        }
    }

    return action;
}

GameAction GameEngine::createGameTurn(string input) {
    GameAction action = UNKNOWN;

    /* valid formats: "{1} {2} {3}"
    *   {1} = {'c', 'C', 'd', 'D', 1, 2, 3, 4, 5, 6, 7, 8, 9}
    *   {2} = {'R', 'Y', 'B', 'L', 'U'}
    *   {3} = {'F', 1, 2, 3, 4, 5}
    */

    try {
        char sourceKey = input.at(0);
        char colourKey = input.at(2);
        char destKey = input.at(4);

        int sourceIndex = getTurnSource(sourceKey);
        int destinationIndex = getTurnDestination(destKey);
        TileColour colour = getTurnColour(colourKey);

        if (sourceIndex != -1 && destinationIndex != -1 && colour != NONE) {
            shared_ptr<Factory> source = nullptr;
            shared_ptr<PatternLine> destination = nullptr;

            bool sourceCentre = false;
            // Get pointer to the requested destination, but only if its a valid move
            if (destinationIndex < 5) {
                // any pattern line from 0-4
                if (gameModel->getCurrentPlayer()->getBoard()->canAccept(colour, destinationIndex)) {
                    destination = gameModel->getCurrentPlayer()->getBoard()->getPatternLine(destinationIndex);
                }

            } else {
                // floor line, can always accept tiles, excess just goes to the lid
                destination = gameModel->getCurrentPlayer()->getBoard()->getFloorLine();
            }

            if ((unsigned int) sourceIndex < gameModel->getNumberOfFactories()) {
                if (gameModel->getFactory(sourceIndex)->contains(colour)) {
                    source = gameModel->getFactory(sourceIndex);
                }
            } else {
                sourceIndex -= 9;
                if ( sourceIndex < gameModel->getNumberOfCentreFactories() && gameModel->getTableCentre(sourceIndex)->contains(colour)) {
                    sourceCentre = true;
                    source = gameModel->getTableCentre(sourceIndex);
                }
            }
            // All validation tests pass, so create the game turn
            if (source && destination) {

                int dumpIndex = -1;
                std::string dumpIn = "";

                //if sourceCentre is true no tiles will be dumped hence it does not matter which source is passed
                if(sourceCentre) {   
                    dumpIndex = 0;
                }

                //if there is only 1 centre factory set the index No need to ask which factory to place into
                if(gameModel->getNumberOfCentreFactories() == 1)
                {
                    dumpIndex = 0;
                }
                
                while(dumpIndex < 0 || dumpIndex > 2 ) {
                    ioHandler->printToStdOut("Which factory will you like to dump to: \n");
                    ioHandler->readFromStdIn(dumpIn);
                    if(dumpIn == "C" || dumpIn == "c") {
                        dumpIndex = 0;
                    } else if (dumpIn == "D" || dumpIn == "d") {
                        dumpIndex = 1;
                    }
                    if(dumpIndex < 0 || dumpIndex > 2) {
                        ioHandler->printToStdOut("ERROR: ");
                    }
                }
                action = GameAction(TURN, make_shared<GameTurn>(source, destination, colour, gameModel->getTableCentre(dumpIndex)));
            }
        }
    } catch (std::out_of_range& e) {

    }
    
    return action;
}

GameAction GameEngine::createShowAction(std::string input)
{
    GameAction action = UNKNOWN;

    vector<shared_ptr<Player>> players = gameModel->getAllPlayers();

        try {
        
        std::string keyword = input.substr(0,4);
        int length = input.length();
        std::string playerName = input.substr(5,length - 4);

        if(keyword == "show" || keyword == "SHOW" || keyword == "Show")
        {

            for(int i = 0; i < gameModel->getNumberOfPlayers(); ++i)
            {
                if(playerName == players[i]->getName())
                {
                    action = GameAction(SHOW_PLAYER, i);
                }
            }
 
        }
    } catch (std::out_of_range& e) {

    }

    return action;
}

void GameEngine::performGameAction(GameAction action) {
    if (action.type() == UNKNOWN) {
        // unknown();
    } else if (action.type() == TURN) {
        doTurn(action.getTurn());
    } else if (action.type() == SHOW_MENU) {
        printMenu();
        performGameAction(promptForAction());
    } else if (action.type() == NEW) {
        newGame();
    } else if (action.type() == LOAD) {
        loadGame();
    } else if (action.type() == SAVE) {
        saveGame();
    } else if (action.type() == CREDITS) {
        printCredits();
    } else if (action.type() == EXIT) {
        
    } else if (action.type() == SHOW_PLAYER) {
        printPlayerBoard(action.getPlayerIndex());
    } else if (action.type() == SHOW_COMMANDS) {
        printCommands();
    }
}

void GameEngine::printPlayerBoard(int playerIndex)
{
    vector<shared_ptr<Player>> players = gameModel->getAllPlayers();

    ioHandler->printToStdOut("Name: " + players[playerIndex]->getName() + "\n" + players[playerIndex]->getBoard()->getPrintable() +"\n\n");
}

void GameEngine::printCommands()
{
    std::string commands = "Commands:\n";
    commands += "Show a player's board -> show <playerName>\n";
    commands += "To make a turn -> <source> <Tile> <destination>\n";
    commands += "To get the commands list -> help\n";
    commands += "To get the menu -> menu\n";
    commands += "To save the game -> save\n";
    commands += "To exit the game -> exit\n";

    ioHandler->printToStdOut(commands);
}

void GameEngine::fillFactories() {
    shared_ptr<TileBag> bag = gameModel->getTileBag();
    unsigned int factoryIndex = 0;
    bool tilesAvailable = true;
    
    while (factoryIndex != gameModel->getNumberOfFactories() && tilesAvailable) {
        shared_ptr<Factory> factory = gameModel->getFactory(factoryIndex);
        int tileIndex = 0;

        // Factories should take four tiles from the bag, each
        while (tileIndex != 4 && tilesAvailable) {
            // Check if the bag is empty
            if (bag->getNumberOfTiles() == 0) {
                refillTileBag();
                if (bag->getNumberOfTiles() == 0) {
                    // Bag and box are both empty, so stop filling and start
                    // turn with incomplete factories
                    tilesAvailable = false;
                }
            }

            if (tilesAvailable) {
                factory->add(bag->remove());
            }
            
            ++tileIndex;
        }

        ++factoryIndex;
    }
}

void GameEngine::refillTileBag() {
    shared_ptr<TileBag> bag = gameModel->getTileBag();
    shared_ptr<BoxLid> lid = gameModel->getBoxLid();
    unsigned int tilesInLid = lid->getNumberOfTiles();
    
    for (unsigned int i = 0; i != tilesInLid; ++i) {
        bag->add(lid->remove());
    }
}

void GameEngine::doTurn(shared_ptr<GameTurn> turn) {
    vector<unique_ptr<Tile>> sourceTiles = turn->getSource()->getTiles();

    for(int i = 0; i < gameModel->getNumberOfCentreFactories(); ++i)
    {
        if(turn->getSource() == gameModel->getTableCentre(i))
        {
            if(gameModel->isFirst())
            {
                gameModel->getCurrentPlayer()->getBoard()->getFloorLine()->addTile(move(gameModel->removeFirstFromTable()));
            }
        }
    }
    


    for (unsigned int i = 0; i != sourceTiles.size(); ++i) {
        if (sourceTiles[i]->getColour() == turn->getColour()) {
            // colour matches what player wants
            if (!turn->getDestination()->isfull()) {
                // theres room on the line, so put it there
                turn->getDestination()->addTile(move(sourceTiles[i]));
            } else {
                // there's no room on the line, so put it on the floor line
                if (!gameModel->getCurrentPlayer()->getBoard()->getFloorLine()->isfull()) {
                    // there's room to put the tile on the floor line
                    gameModel->getCurrentPlayer()->getBoard()->getFloorLine()->addTile(move(sourceTiles[i]));
                } else {
                    // the player's floor line is also full, so put it in the lid
                    gameModel->getBoxLid()->add(move(sourceTiles[i]));
                }
            }
        } else {
            // excess tiles are moved to the table centre
            
            //TODO 
            turn->getCentre()->add(move(sourceTiles[i]));
            // gameModel->getTableCentre(0)->add(move(sourceTiles[i]));
        }
    }

    if(endOfFactoryOffer()) {
        doScoring();
        if (endOfGame()) {
            doFinalScoring();
            declareWinner();
        } else {
            fillFactories();
        }
    } else {
        passTurnToNextPlayer();
    }
}

bool GameEngine::endOfFactoryOffer() {
    // end of the round if all factories are empty
    bool isEnd = true;
    map<TileColour, int> counts;

    isEnd = !gameModel->isFirst();

    for(int i = 0; i < gameModel->getNumberOfCentreFactories() &&  isEnd; ++i) {
        isEnd  = gameModel->getTableCentre(i)->isEmpty();
    }

    unsigned int index = 0;

    while (index != gameModel->getNumberOfFactories() && isEnd) {
        isEnd = gameModel->getFactory(index)->isEmpty();
        ++index;
    }

    return isEnd;
}

void GameEngine::doScoring() {
    for (shared_ptr<Player> player : gameModel->getAllPlayers()) {
        scorePlayer(player);
    }
}

void GameEngine::scorePlayer(shared_ptr<Player> player) {
    // Get the current score for a player
    int newScore = player->getScore();

    // Update their score based on their pattern lines
    newScore += scorePlayerPatternLines(player);

    // Update their score based on their floor line
    newScore += scorePlayerFloorLine(player);
    
    // Players don't fall below a score of 0
    if (newScore < 0) {
        newScore = 0;
    }

    player->setScore(newScore);
}

int GameEngine::scorePlayerPatternLines(shared_ptr<Player> player) {
    shared_ptr<PlayerBoard> board = player->getBoard();
    Mosaic& wall = board->getMosaic();

    int score = 0;

    // Calculate positive scoring from moving to the wall
    for (unsigned int i = 0; i != 5; ++i) {
        // check each line from top to bottom
        shared_ptr<PatternLine> line = board->getPatternLine(i);

        if (line->isfull()) {
            // Move a single tile to the wall
            score += wall.add(move(line->remove()), i);

            // Move remaining tiles to the box lid
            int numExcessTiles = line->getSize() - line->getSpace();
            while (numExcessTiles != 0) {
                gameModel->getBoxLid()->add(move(line->remove()));
                --numExcessTiles;
            }
        }
    }

    return score;
}

int GameEngine::scorePlayerFloorLine(shared_ptr<Player> player) {
    int score = 0;

    // Calculate negative scoring for tiles on the floor
    shared_ptr<PatternLine> floorline = player->getBoard()->getFloorLine();
    int numTilesInFloor = floorline->getSize() - floorline->getSpace();
    for (int i = 0; i != numTilesInFloor; ++i) {
        unique_ptr<Tile> tile = floorline->remove();

        // Move the tile to the correct place
        if (tile->isStartingMarker()) {
            // put the marker in the centre
            gameModel->placeFirstOnTable(move(tile));

            // setting current player here, so they start the next round
            gameModel->setCurrentPlayer(player);
        } else {
            // All other tiles to the lid
            gameModel->getBoxLid()->add(move(tile));
        }

        // Adjust score based on position of tile in floor line
        if (i <= 1) {
            score -= 1;
        } else if (i <= 4) {
            score -= 2;
        } else {
            score -= 3;
        }
    }

    return score;
}

bool GameEngine::endOfGame() {
    bool isEnd = false;

    for (shared_ptr<Player> player : gameModel->getAllPlayers()) {
        if (player->getBoard()->getMosaic().rowComplete()) {
            isEnd = true;
        }
    }

    return isEnd;
}

void GameEngine::doFinalScoring() {
    
    for (shared_ptr<Player> player : gameModel->getAllPlayers()) {
        int score = player->getScore();
        int rowsCompleted = 0;
        
        for (int i = 0; i != 5; ++i) {
            // calculate any row bonuses
            if (player->getBoard()->getMosaic().rowComplete(i)) {
                score += 2;
                ++rowsCompleted;
            }

            // calculate any column bonuses
            if (player->getBoard()->getMosaic().columnCompleted(i)) {
                score += 7;
            }
        }

        // Set rows completed, used in the case of a tie breaker on score alone
        player->setRowsCompleted(rowsCompleted);

        // 5 colours on a wall nets 10 points per colour
        map<TileColour, int> colourCounts;
        player->getBoard()->getMosaic().reportTileCounts(colourCounts);
        for (auto count : colourCounts) {
            if (count.second == 5) {
                score += 10;
            }
        }

        player->setScore(score);
    }
}

void GameEngine::declareWinner() {
    shared_ptr<Player> winner = nullptr;

    vector<shared_ptr<Player>> players = gameModel->getAllPlayers();

    //Find player with the highest score
    int highestScore = 0;
    vector<int> highestScoreIndex;
    for (int i = 0; i < gameModel->getNumberOfPlayers(); ++i) {
        if (players[i]->getScore() > highestScore) {
            highestScoreIndex.clear();
            highestScoreIndex.push_back(i);
            highestScore = players[i]->getScore();
        } else if (players[i]->getScore() == highestScore) {
            highestScoreIndex.push_back(i);
        }
    }

    //If scores eqaul find player with the most collumns, Declare the winnner
    if (highestScoreIndex.size() > 1) {
        int mostRowsCompleted = 0;
        vector<int> mostRowsCompletedIndex;
        for (unsigned int i = 0; i < highestScoreIndex.size(); ++i)
        {
            if (players[highestScoreIndex[i]]->getRowsCompleted() > mostRowsCompleted){
                mostRowsCompletedIndex.clear();
                mostRowsCompletedIndex.push_back(i);
                mostRowsCompleted = players[highestScoreIndex[i]]->getRowsCompleted();
            } else if(players[highestScoreIndex[i]]->getRowsCompleted() == mostRowsCompleted) {
                mostRowsCompletedIndex.push_back(i);
            }
        }

        if(mostRowsCompletedIndex.size()) {
            //draw
        }
        else {
            winner = players[mostRowsCompletedIndex[0]];
        }
    } else {
        winner = players[highestScoreIndex[0]];
    }

    if (!winner) {
        ioHandler->printToStdOut("It's a draw!\n");
    } else {
        ioHandler->printToStdOut(winner->getName() + " is the winner!\n");
    }

    inProgress = false;
}

void GameEngine::passTurnToNextPlayer() {
    vector<shared_ptr<Player>>& players = gameModel->getAllPlayers();
    int currentPlayerNumber = 0;

    for(int i = 0; i < gameModel->getNumberOfPlayers(); ++i)
    {
        if(players[i] == gameModel->getCurrentPlayer())
        {
            currentPlayerNumber = i;
        }
    }
    if(gameModel->getNumberOfPlayers() - 1 == currentPlayerNumber)
    {
        gameModel->setCurrentPlayer(players[0]);
    }
    else
    {
        gameModel->setCurrentPlayer(players[++currentPlayerNumber]);
    }
}

void GameEngine::loadGame() {
    gameModel = make_shared<GameModel>();
    ioHandler->printToStdOut(menu->getUserPrompt("\nEnter filename"));
    std::string fileName;

    if(ioHandler->readFromStdIn(fileName)) {
        
        map<string, string> rawData;
        ioHandler->loadGameFile(rawData, fileName);

        ModelBuilder modelBuilder = ModelBuilder(*gameModel);
        modelBuilder.loadSaveData(rawData);

        

        if (gameModel->validate()) {
            
            ioHandler->printToStdOut("Game successfully loaded.\n");
            // Update game state
            inProgress = true;
        } else {
            ioHandler->printToStdOut("Error: Save game is defective.\n");
        }

    } else {
        ioHandler->printToStdOut("Error: Invalid filename.\n");
    }
}

void GameEngine::saveGame() {
    ioHandler->printToStdOut(menu->getUserPrompt("\nEnter filename"));

    std::string fileName;

    if(ioHandler->readFromStdIn(fileName)) {
        ioHandler->printToFile(gameModel->toString(), fileName);
    } else {
        ioHandler->printToStdOut("Error: Invalid filename.\n");
    }

}

void GameEngine::newGame() {
    gameModel = make_shared<GameModel>();

    
    int numberOfPlayers = -1;
    int numberOfCentralFactories = -1;

    while(numberOfCentralFactories > 2 || numberOfCentralFactories < 1)
    {
        ioHandler->printToStdOut("Enter the number of factories between 1 & 2:\n");
        numberOfCentralFactories = ioHandler->readIntFromStdIn();
        if(numberOfCentralFactories > 2 || numberOfCentralFactories < 1)
        {
            ioHandler->printToStdOut("Error: ");
        }
    }

    

    while(numberOfPlayers > 4 || numberOfPlayers < 2)
    {
        ioHandler->printToStdOut("Enter the number of players between 2 & 4:\n");
        numberOfPlayers = ioHandler->readIntFromStdIn();
        if(numberOfPlayers > 4 || numberOfPlayers < 2)
        {
            ioHandler->printToStdOut("Error: ");
        }
    }
        std::string * playerNames = new std::string[numberOfPlayers];
        std::string playerName = "";
    
    ioHandler->readFromStdIn(playerName);
    for(int i = 0; i < numberOfPlayers; ++i)
    {
        ioHandler->printToStdOut("Enter Player Name:\n");
        ioHandler->readFromStdIn(playerName);
        playerNames[i] = playerName;
    }

    ModelBuilder modelBuilder = ModelBuilder(*gameModel);
    modelBuilder.createNewGame(numberOfCentralFactories, playerNames, numberOfPlayers, seed);

    if (gameModel->validate()) {
        ioHandler->printToStdOut("Game successfully created.\n");
        // Update game state
        // inProgress = true;
    } else {
        ioHandler->printToStdOut("Error: Game is defective.\n");
    }

    // First player is default first turn, change to prompt for portugal thing
    gameModel->setCurrentPlayer(gameModel->getAllPlayers()[0]);

    // Add tiles to the factories
    fillFactories();

    // Update the game state to start the game
    inProgress = true;
}

int GameEngine::getTurnSource(char sourceKey) {
    int source = -1;

    if (sourceKey == 'c' || sourceKey == 'C') {
        source = 9;
    } else if(sourceKey == 'd' || sourceKey == 'D') {
        source = 10;
    } else if (sourceKey == '1') {std::string numberOfPlayers;
        source = 0;
    } else if (sourceKey == '2') {
        source = 1;
    } else if (sourceKey == '3') {
        source = 2;
    } else if (sourceKey == '4') {
        source = 3;
    } else if (sourceKey == '5') {
        source = 4;
    } else if (sourceKey == '6' && gameModel->getNumberOfPlayers() >= 3) {
        source = 5;
    } else if (sourceKey == '7' && gameModel->getNumberOfPlayers() >= 3) {
        source = 6;
    } else if (sourceKey == '8' && gameModel->getNumberOfPlayers() >= 4) {
        source = 7;
    } else if (sourceKey == '9' && gameModel->getNumberOfPlayers() >= 4) {
        source = 8;
    } else {
        // invalid source key
    }

    return source;
}

int GameEngine::getTurnDestination(char destKey) {
    int dest = -1;

    if (destKey == 'f' || destKey == 'F') {
        dest = 5;
    } else if (destKey == '1') {
        dest = 0;
    } else if (destKey == '2') {
        dest = 1;
    } else if (destKey == '3') {
        dest = 2;
    } else if (destKey == '4') {
        dest = 3;
    } else if (destKey == '5') {
        dest = 4;
    } else {
        // invalid dest key
    }

    return dest;
}

TileColour GameEngine::getTurnColour(char colourKey) {
    TileColour colour = NONE;

    if (colourKey == 'R' || colourKey == 'r' ) {
        colour = RED;
    } else if (colourKey == 'Y' || colourKey == 'y' ) {
        colour = YELLOW;
    } else if (colourKey == 'B' || colourKey == 'b' ) {
        colour = DARK_BLUE;
    } else if (colourKey == 'L' || colourKey == 'l' ) {
        colour = LIGHT_BLUE;
    } else if (colourKey == 'U' || colourKey == 'u' ) {
        colour = BLACK;
    } else {
        // invalid colour
    }

    return colour;
}
