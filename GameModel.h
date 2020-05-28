
/*
 * Game Model
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#define KEY_VALUE_DELIMITER std::string("=")
#define KEY_SPLIT_DELIMITER std::string("_")
#define BAG_KEY             std::string("BAG")
#define LID_KEY             std::string("LID")
#define PLAYER_KEY          std::string("PLAYER")
#define PLAYER_NAME_KEY     std::string("NAME")
#define PLAYER_PATTERN_KEY  std::string("PATTERN_LINE")
#define PLAYER_FLOOR_KEY    std::string("FLOOR_LINE")
#define PLAYER_SCORE_KEY    std::string("SCORE")
#define PLAYER_WALL_KEY     std::string("MOSAIC")
#define CURRENT_PLAYER_KEY  std::string("CURRENT_PLAYER")
#define FACTORY_KEY         std::string("FACTORY")
#define CENTRE_KEY          std::string("CENTRE")

#include <memory>
#include <vector>

#include "BoxLid.h"
#include "Factory.h"
#include "Player.h"
#include "TileBag.h"

class GameModel {
    public:
        GameModel();
        ~GameModel();

        // Get the maximum number of players supported
        int getMaxPlayers();

        // Get the minimum number of players supported
        int getMinPlayers();

        // Checks the game state to ensure total number of tiles is correct
        bool validate();

        // Add a factory to the model
        void addFactory(std::shared_ptr<Factory> factory);

        // Get a list of the factories
        std::shared_ptr<Factory> getFactory(unsigned int index);

        // Return the number of factories in the game, excluding the table centre
        unsigned int getNumberOfFactories();

        int getNumberOfPlayers();

        // Must only be called when there are fewer than 2 players already in the game
        void addPlayer(std::shared_ptr<Player> player);
        
        std::shared_ptr<Player> getCurrentPlayer();

        void setCurrentPlayer(std::shared_ptr<Player> player);

        std::vector<std::shared_ptr<Player>>& getAllPlayers();

        std::shared_ptr<TileBag> getTileBag();

        std::shared_ptr<BoxLid> getBoxLid();

        std::shared_ptr<Factory> getTableCentre();

        // Used when saving a game
        std::string toString();

    private:
        // Game data
        std::shared_ptr<TileBag> tileBag;
        std::shared_ptr<BoxLid> lid;
        std::vector<std::shared_ptr<Player>> players;
        std::shared_ptr<Player> currentPlayer;
        std::vector<std::shared_ptr<Factory>> factories;
        std::shared_ptr<Factory> tableCentre;     

};

#endif // GAME_MODEL_H
