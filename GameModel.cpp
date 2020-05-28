
#include <iostream>
#include <memory>

#include "BoxLid.h"
#include "Factory.h"
#include "GameModel.h"
#include "GameTurn.h"
#include "Player.h"
#include "TileBag.h"

#define MAX_PLAYERS 2
#define MIN_PLAYERS 2

using std::make_shared;
using std::map;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::vector;

GameModel::GameModel() :
    tileBag(make_shared<TileBag>()),
    lid(make_shared<BoxLid>()),
    players(vector<shared_ptr<Player>>()),
    currentPlayer(nullptr),
    factories(vector<shared_ptr<Factory>>()),
    tableCentre(make_shared<Factory>())
{}

GameModel::~GameModel() {
    // TODO
}

int GameModel::getMaxPlayers() {
    return MAX_PLAYERS;
}

int GameModel::getMinPlayers() {
    return MIN_PLAYERS;
}

bool GameModel::validate() {
    bool success = false;
    map<TileColour, int> totals;

    tileBag->reportTileCounts(totals);
    lid->reportTileCounts(totals);

    for (shared_ptr<Factory> factory : factories) {
        factory->reportTileCounts(totals);
    }

    for (shared_ptr<Player> player : players) {
        player->getBoard()->reportTileCounts(totals);
    }

    tableCentre->reportTileCounts(totals);
    
    if (totals[RED] == 20 &&
        totals[YELLOW] == 20 &&
        totals[DARK_BLUE] == 20 &&
        totals[LIGHT_BLUE] == 20 &&
        totals[BLACK] == 20 &&
        totals[FIRST] == 1) {
        
        success = true;
    }

    return success;
}

void GameModel::addFactory(shared_ptr<Factory> factory) {
    factories.push_back(factory);
}

shared_ptr<Factory> GameModel::getFactory(unsigned int index) {
    return factories[index];
}

unsigned int GameModel::getNumberOfFactories() {
    return factories.size(); 
}

int GameModel::getNumberOfPlayers() {
    return players.size();
}

void GameModel::addPlayer(shared_ptr<Player> player) {
    players.push_back(player);
}

shared_ptr<Player> GameModel::getCurrentPlayer() {
    return currentPlayer;
}

void GameModel::setCurrentPlayer(shared_ptr<Player> player) {
    currentPlayer = player;
}

vector<shared_ptr<Player>>& GameModel::getAllPlayers() {
    return players;
}

shared_ptr<TileBag> GameModel::getTileBag() {
    return tileBag;
}

shared_ptr<BoxLid> GameModel::getBoxLid() {
    return lid;
}

shared_ptr<Factory> GameModel::getTableCentre() {
    return tableCentre;
}

std::string GameModel::toString() {
    string result = "";
    string currentPlayerId = "";

    // Print bag and lid
    result += BAG_KEY + KEY_VALUE_DELIMITER + tileBag->toString() + "\n";
    result += LID_KEY + KEY_VALUE_DELIMITER + lid->toString() + "\n";

    // Print taable centre
    result += FACTORY_KEY + KEY_SPLIT_DELIMITER + CENTRE_KEY + KEY_VALUE_DELIMITER + tableCentre->toString() + "\n";

    // Print factories
    for (unsigned int i = 0; i != factories.size(); ++i) {
        result += FACTORY_KEY + KEY_SPLIT_DELIMITER + to_string(i) + KEY_VALUE_DELIMITER + factories[i]->toString() + "\n";
    }

    for (unsigned int playerId = 0; playerId != players.size(); ++playerId) {
        if (players[playerId] == currentPlayer) {
            currentPlayerId = to_string(playerId);
        }

        // Print player name
        result += PLAYER_KEY + KEY_SPLIT_DELIMITER + to_string(playerId) +
                  KEY_SPLIT_DELIMITER + PLAYER_NAME_KEY + KEY_VALUE_DELIMITER +
                  players[playerId]->getName() + "\n";

        // Print player score
        result += PLAYER_KEY + KEY_SPLIT_DELIMITER + to_string(playerId) + KEY_SPLIT_DELIMITER + PLAYER_SCORE_KEY + KEY_VALUE_DELIMITER + to_string(players[playerId]->getScore()) + "\n";
        
        // Print player pattern lines
        for (int i = 0; i != 5; ++i) {
            result += PLAYER_KEY + KEY_SPLIT_DELIMITER + to_string(playerId) + KEY_SPLIT_DELIMITER + PLAYER_PATTERN_KEY + KEY_SPLIT_DELIMITER + to_string(i) + KEY_VALUE_DELIMITER + players[playerId]->getBoard()->getPatternLine(i)->toString() + "\n";
        }

        // Print player floor line
        result += PLAYER_KEY + KEY_SPLIT_DELIMITER + to_string(playerId) + KEY_SPLIT_DELIMITER + PLAYER_FLOOR_KEY + KEY_VALUE_DELIMITER +players[playerId]->getBoard()->getFloorLine()->toString() + "\n";

        // Print player wall
        for (int i = 0; i != 5; ++i) {
            result += "PLAYER_" + to_string(playerId) + "_MOSAIC_" + to_string(i) + "=" + players[playerId]->getBoard()->getMosaic().toString(i) + "\n";
        }

        
    }

    // Print current player id
    result += CURRENT_PLAYER_KEY + KEY_VALUE_DELIMITER + currentPlayerId + "\n";

    return result;
}
