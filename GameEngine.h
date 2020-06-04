
/*
 * Game Engine
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <memory>
#include <string>

#include "GameAction.h"
#include "GameModel.h"
#include "IOHandler.h"
#include "Menu.h"

class GameEngine {
    public:
        GameEngine();
        ~GameEngine();

        // Set the seed if it has been provided
        void setSeed(int seed);

        // Main entrypoint to the game
        void run();

        // Prints the game banner 
        void printBanner();

        // Prints the game exit credits
        void printCredits();

        // Print the menu options
        void printMenu();

        // Prints the current state of the game, for player info prior to a turn
        void printPreTurnInfo();
        
        // Loop the prompt until a valid input is received
        GameAction promptForAction();

        // Parse the input given from a player
        GameAction parseInput(const std::string input);

        // Attempt to create a game turn action from user input
        GameAction createGameTurn(std::string input);

        // Perform the requested action
        void performGameAction(GameAction action);
        
        // Fill the factories from the tile bag
        void fillFactories();

        // Refill the tile bag using tiles from the lid
        void refillTileBag();
        
        // Try to make a game turn
        void doTurn(std::shared_ptr<GameTurn> turn);

        // Check if end of round condition has been met
        bool endOfFactoryOffer();

        // Calculate scores and move tiles, per the game rules
        void doScoring();

        // Calculate and apply scores to each player
        void scorePlayer(std::shared_ptr<Player> player);

        // Calculate the score for a player's pattern lines
        int scorePlayerPatternLines(std::shared_ptr<Player> player);

        // Calculate the score for a players' floor line
        int scorePlayerFloorLine(std::shared_ptr<Player> player);

        // Check if the end of game condition has been met
        bool endOfGame();

        // Calculate the scores for the end of a game
        void doFinalScoring();

        // Determine a winner, display result
        void declareWinner();

        // Pass control to next player
        void passTurnToNextPlayer();

        // Load a saved game
        void loadGame();
        
        // Save the current game
        void saveGame();
        
        // Start a new game
        void newGame();

        GameAction createShowAction(std::string input);

        void printPlayerBoard(int playerIndex);

        void printCommands();

    private:
        std::shared_ptr<GameModel>    gameModel;
        std::shared_ptr<IOHandler>    ioHandler;
        std::shared_ptr<Menu>         menu;

        // For generating random data
        int seed;

        // False upon program start and after winning condition is met,
        // true otherwise
        bool inProgress;

        // True while a player is currently interacting with a menu
        bool inMenu;

        int getTurnSource(char sourceKey);
        int getTurnDestination(char destKey);
        TileColour getTurnColour(char colourKey);
        
};

#endif // GAME_ENGINE_H
