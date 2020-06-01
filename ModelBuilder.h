
/*
 * Model Builder
 * 
 * Helper class for constructing a GameModel.
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef MODEL_BUILDER_H
#define MODEL_BUILDER_H

#include <map>
#include <memory>
#include <string>

#include "Factory.h"
#include "GameModel.h"
#include "Player.h"
#include "Types.h"

class ModelBuilder {
    public:
        ModelBuilder(GameModel& gameModel);

        // Parse a map of key/value pairs, as would appear in a save game file
        void loadSaveData(std::map<std::string, std::string>& rawData);

        // Parse a string of tiles from a saved game and put them in the tilebag
        void loadTileBag(std::string& tileList);

        void loadBoxLid(std::string& tileList);

        bool createNewGame(int numberOfCentreFactories, std::string * playerNames, int numberOfPlayers, int seed);

    private:
        GameModel& gameModel;

        // Cache the factory data 
        std::map<int, std::shared_ptr<Factory>> tempFactories;

        // Cache the player data as it is read in in pieces
        std::map<std::string, std::shared_ptr<Player>> tempPlayers;

        // Cache the id of the current player
        std::string currentPlayerId;
        
        void parseAndLoadDataPair(std::string key, std::string value);
        TileColour getTileColourFromChar(char colour);

        // Parse and load data into a factory
        void loadFactory(std::string key, std::string tileList);

        // Parse and load player data from save file
        void loadPlayerDataPair(std::string key, std::string value);

        // Parse and load a single pattern line for a player
        void loadPlayerPatternLine(std::shared_ptr<Player> player, int row, std::string tileList);

        // Parse and load a floor line for a player
        void loadPlayerFloorLine(std::shared_ptr<Player> player, std::string tileList);

        // Parse and load a row of the wall for a player
        void loadPlayerWallRow(std::shared_ptr<Player> player, int row, std::string tileList);

        //
        void loadFirstKey(std::string value);

        int numberOfCentreFactories = 0;
};

#endif // MODEL_BUILDER_H
