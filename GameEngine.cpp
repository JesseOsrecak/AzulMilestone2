
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
    ioHandler->printToStdOut("\nTable Centre\nC: " + gameModel->getTableCentre()->toString() + "\n\n");

    // Print the factories
    ioHandler->printToStdOut("Factories\n");
    for (unsigned int i = 0; i != gameModel->getNumberOfFactories(); ++i) {
        ioHandler->printToStdOut(to_string(i + 1) + ": " + gameModel->getFactory(i)->toString() + "\n");
    }

    // Print the current player
    ioHandler->printToStdOut("\nCurrent Player\n");
    ioHandler->printToStdOut(gameModel->getCurrentPlayer()->toString() + "\n\n");
    
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

    return action;
}

GameAction GameEngine::createGameTurn(string input) {
    GameAction action = UNKNOWN;

    /* valid formats: "{1} {2} {3}"
    *   {1} = {'c', 'C', 1, 2, 3, 4, 5}
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
                if (gameModel->getTableCentre()->contains(colour)) {
                    source = gameModel->getTableCentre();
                }
            }
            
            // All validation tests pass, so create the game turn
            if (source && destination) {
                action = GameAction(TURN, make_shared<GameTurn>(source, destination, colour));
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
        // exit();
    }
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
        } else if (sourceTiles[i]->isStartingMarker()) {
            // the tile is the starting marker, which is placed on the floor line
            gameModel->getCurrentPlayer()->getBoard()->getFloorLine()->addTile(move(sourceTiles[i]));
        } else {
            // excess tiles are moved to the table centre
            gameModel->getTableCentre()->add(move(sourceTiles[i]));
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

    isEnd = gameModel->getTableCentre()->isEmpty();
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
            gameModel->getTableCentre()->add(move(tile));

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

    if (players[0]->getScore() == players[1]->getScore()) {
        if (players[0]->getRowsCompleted() == players[1]->getRowsCompleted()) {
            // draw
        } else if (players[0]->getRowsCompleted() > players[1]->getRowsCompleted()) {
            winner = players[0];
        } else {
            winner = players[1];
        }
    } else if (players[0]->getScore() > players[1]->getScore()) {
        winner = players[0];
    } else {
        winner = players[1];
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
    
    if (players[0] == gameModel->getCurrentPlayer()) {
        gameModel->setCurrentPlayer(players[1]);
    } else {
        gameModel->setCurrentPlayer(players[0]);
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
    ioHandler->printToStdOut("Enter Player1 Name:\n");
    std::string player1Name;
    ioHandler->readFromStdIn(player1Name);
    
    ioHandler->printToStdOut("Enter Player2 Name:\n");
    std::string player2Name;
    ioHandler->readFromStdIn(player2Name);

    ModelBuilder modelBuilder = ModelBuilder(*gameModel);
    modelBuilder.createNewGame(player1Name, player2Name, seed);

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
        source = 5;
    } else if (sourceKey == '1') {
        source = 0;
    } else if (sourceKey == '2') {
        source = 1;
    } else if (sourceKey == '3') {
        source = 2;
    } else if (sourceKey == '4') {
        source = 3;
    } else if (sourceKey == '5') {
        source = 4;
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