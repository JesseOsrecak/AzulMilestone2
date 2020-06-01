
#include <iostream>
#include <map>
#include <string>
#include <random>

#include "BoxLid.h"
#include "GameModel.h"
#include "ModelBuilder.h"
#include "Player.h"
#include "TileBag.h"

using std::map;
using std::make_shared;
using std::make_unique;
using std::move;
using std::pair;
using std::shared_ptr;
using std::stoi;
using std::string;
using std::unique_ptr;

ModelBuilder::ModelBuilder(GameModel& gameModel) :
    gameModel(gameModel)
{}

void ModelBuilder::loadSaveData(map<string, string>& rawData) {

    for(const pair<string, string>& item : rawData) {
        parseAndLoadDataPair(item.first, item.second);
    }

    for (auto player : tempPlayers) {
        gameModel.addPlayer(player.second);
    }

    for (unsigned int i = 0; i != tempFactories.size(); ++i) {
        gameModel.addFactory(tempFactories[i]);
    }

    gameModel.setCurrentPlayer(tempPlayers[currentPlayerId]);
}

void ModelBuilder::parseAndLoadDataPair(std::string key, std::string value) {
    if (key == BAG_KEY) {
        loadTileBag(value);
    } else if (key == LID_KEY) {
        loadBoxLid(value);
    } else if (key.find(PLAYER_KEY) == 0) {
        loadPlayerDataPair(key, value);
    } else if (key == CURRENT_PLAYER_KEY) {
        currentPlayerId = value;
    } else if (key.find(FACTORY_KEY) == 0) {
        loadFactory(key, value);
    } else if (key.find(TABLE_KEY) == 0) {
        loadFirstKey(value);
    }
}

void ModelBuilder::loadTileBag(string& tileList) {
    shared_ptr<TileBag> tileBag = gameModel.getTileBag();

    for (char colourCode : tileList) {
        tileBag->add(make_unique<Tile>(getTileColourFromChar(colourCode)));
    }
}

void ModelBuilder::loadBoxLid(string& tileList) {
    shared_ptr<BoxLid> lid = gameModel.getBoxLid();

    for (char colourCode : tileList) {
        lid->add(make_unique<Tile>(getTileColourFromChar(colourCode)));
    } 
}

void ModelBuilder::loadFactory(string key, string tileList) {
    // Drop the initial part of the key
    key.erase(0, FACTORY_KEY.length() + KEY_SPLIT_DELIMITER.length());

    if (key.length() >= CENTRE_KEY.length()) {

        gameModel.addTableCentre();

        for (char colourCode : tileList) {

            TileColour tileColour = getTileColourFromChar(colourCode);
            gameModel.getTableCentre(numberOfCentreFactories)->add(make_unique<Tile>(tileColour));

        }

        ++numberOfCentreFactories;
    } else {
        // do standard factory
        tempFactories[stoi(key)] = make_shared<Factory>();
        for (char colourCode : tileList) {

            TileColour tileColour = getTileColourFromChar(colourCode);
            tempFactories[stoi(key)]->add(make_unique<Tile>(tileColour));
        }
    }
}

void ModelBuilder::loadPlayerDataPair(string key, string value) {
    /**
     * Split key
     * check if player is in map
     *   create and add player to map
     * check data (pattern line, name, etc.)
     * pass to specific player load method
     */

    // Drop the initial part of the key
    key.erase(0, PLAYER_KEY.length() + KEY_SPLIT_DELIMITER.length());

    string playerId = key.substr(0, key.find(KEY_SPLIT_DELIMITER));
    shared_ptr<Player> player = nullptr;

    // Retrieve player from map, or create one if necessary
    auto search = tempPlayers.find(playerId);
    if (search == tempPlayers.end()) {
        player = make_shared<Player>();
        tempPlayers[playerId] = player;
    } else {
        player = search->second;
    }

    // Remove the player id from the key
    string subKey = key.erase(0, playerId.length() + KEY_SPLIT_DELIMITER.length());

    if(subKey == PLAYER_NAME_KEY) {
        // Set name
        player->setName(value);
    } else if (subKey == PLAYER_SCORE_KEY) {
        // Set score
        player->setScore(stoi(value));
    } else if (subKey.find(PLAYER_PATTERN_KEY) == 0) {
        // Set pattern line
        // Determine which line we're reading
        int row = stoi(subKey.erase(0, PLAYER_PATTERN_KEY.length() + KEY_SPLIT_DELIMITER.length()));
        loadPlayerPatternLine(player, row, value);
    } else if (subKey.find(PLAYER_FLOOR_KEY) == 0) {
        loadPlayerFloorLine(player, value);
    } else if (subKey.find(PLAYER_WALL_KEY) == 0) {
        // WALL_LINE_row
        int row = stoi(subKey.erase(0, PLAYER_WALL_KEY.length() + KEY_SPLIT_DELIMITER.length()));
        loadPlayerWallRow(player, row, value);
    } else {
        // unknown parameter
    }

}

void ModelBuilder::loadPlayerPatternLine(shared_ptr<Player> player, int row, std::string tileList) {
    for (char colourCode : tileList) {
        TileColour tileColour = getTileColourFromChar(colourCode);
        if (tileColour != NONE) {
            player->getBoard()->addTileToPatternLine(make_unique<Tile>(tileColour), row);
        }
    }
}

void ModelBuilder::loadPlayerFloorLine(std::shared_ptr<Player> player, std::string tileList) {
    for (char colourCode : tileList) {
        TileColour tileColour = getTileColourFromChar(colourCode);
        if (tileColour != NONE) { 
            player->getBoard()->addTileToFloorLine(make_unique<Tile>(tileColour));
        }
    }
}

void ModelBuilder::loadPlayerWallRow(std::shared_ptr<Player> player, int row, string tileList) {
    for (unsigned int i = 0; i != tileList.length(); ++i) {
        TileColour tileColour = getTileColourFromChar(tileList[i]);
        player->getBoard()->getMosaic().add(make_unique<Tile>(tileColour), row, i);
    }
}

void ModelBuilder::loadFirstKey(std::string tileList)
{
    for (char colourCode : tileList) {
        TileColour tileColour = getTileColourFromChar(colourCode);
        if (tileColour == FIRST) { 
            gameModel.placeFirstOnTable(make_unique<Tile>(tileColour)); 
        }
    }
}

// This method should probably be somewhere else
TileColour ModelBuilder::getTileColourFromChar(char colourCode) {
    TileColour result = RED;

    if(colourCode == 'R') {
        result = RED;
    } else if (colourCode == 'Y') {
        result = YELLOW;
    } else if (colourCode == 'B') {
        result = DARK_BLUE;
    } else if (colourCode == 'L') {
        result = LIGHT_BLUE;
    } else if (colourCode == 'U') {
        result = BLACK;
    } else if (colourCode == 'F') {
        result = FIRST;
    } else if (colourCode == '-') {
        result = NONE;
    } else {
        // unknown, do something error-like
    }

    return result;
}

// If expanded to support more players should have an array of player names and
// the amount of players in the constructor instead
bool ModelBuilder::createNewGame(int numberOfCentreFactories, std::string * playerNames, int numberOfPlayers, int seed) {
    // Instantiate players
    for(int i = 0; i < numberOfPlayers; ++i)
    {
        gameModel.addPlayer(make_shared<Player>(playerNames[i]));
    }
    
    for(int i = 0 ; i < numberOfCentreFactories ; ++i)
    {
        gameModel.addTableCentre();
    }
    
    gameModel.placeFirstOnTable(std::make_unique<Tile>(FIRST));
        
    int numberOfFactories = 5;
    if(numberOfPlayers == 3)
    {
        numberOfFactories = 7;
    }
    else if(numberOfPlayers == 4)
    {
        numberOfFactories = 9;
    }
    // Add factories to the model
    for (int i = 0; i != numberOfFactories; ++i) {
        gameModel.addFactory(make_shared<Factory>());
    }

    // Instantiate all the tiles and add to the bag
    int tilecount = 0;
    int red = 0;
    int light_blue = 0;
    int dark_blue = 0;
    int black = 0;
    int yellow = 0;

    // Setup random number generator
    std::random_device rand;
    std::default_random_engine engine;
    
    if (seed != std::numeric_limits<int>::min()) {
        engine.seed(seed);
    } else {
        engine.seed(rand());
    }

    std::uniform_int_distribution<int> uniform_dist(0, 4);
    std::shared_ptr<TileBag> tileBag = gameModel.getTileBag();

    while (tilecount != 100) {
        // Get a number
        int randomNumber = uniform_dist(engine);

        if (randomNumber == 0) {
            // Add dark blue tiles
            if (dark_blue < 20) {
                tileBag->add(std::make_unique<Tile>(DARK_BLUE));
                ++dark_blue;
                ++tilecount;
            } else {
                // Already have enough, try the next colour
                ++randomNumber;
            }
        }

        if (randomNumber == 1) {
            // Add red tiles
            if (red < 20) {
                tileBag->add(std::make_unique<Tile>(RED));
                ++red;
                ++tilecount;
            } else {
                // Already have enough, try the next colour
                ++randomNumber;
            }
        }

        if (randomNumber == 2) {
            // Add yellow tiles
            if (yellow < 20) {
                tileBag->add(std::make_unique<Tile>(YELLOW));
                ++yellow;
                ++tilecount;
            } else {
                // Already have enough, try the next colour
                ++randomNumber;
            }
        }

        if (randomNumber == 3) {
            // Add black tiles
            if (black < 20) {
                tileBag->add(std::make_unique<Tile>(BLACK));
                ++black;
                ++tilecount;
            } else {
                // Already have enough, try the next colour
                ++randomNumber;
            }
        }

        if (randomNumber == 4 && light_blue < 20) {
            // Add light blue tiles
            tileBag->add(std::make_unique<Tile>(LIGHT_BLUE));
            ++light_blue;
            ++tilecount;
        }
    }

    return true;
}
