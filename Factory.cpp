
#include <map>
#include <memory>
#include <vector>

#include "Factory.h"
#include "Tile.h"

using std::map;
using std::move;
using std::string;
using std::unique_ptr;
using std::vector;

Factory::Factory() :
    tiles(vector<unique_ptr<Tile>>())
{}

void Factory::add(unique_ptr<Tile> tile) {
    tiles.push_back(move(tile));
}

bool Factory::contains(TileColour colour) {
    bool result = false;
    
    for (const unique_ptr<Tile>& tile : tiles) {
        if (tile->getColour() == colour) {
            result = true;
        }
    }

    return result;
}

vector<unique_ptr<Tile>> Factory::getTiles() {
    vector<unique_ptr<Tile>> result = move(tiles);
    tiles = vector<unique_ptr<Tile>>();
    return result;
}

void Factory::reportTileCounts(std::map<TileColour, int>& tileCounts) {

    for (unsigned int i = 0; i != tiles.size(); ++i) {
        int newCount = 1;
        auto count = tileCounts.find(tiles.at(i)->getColour());
        
        if (count != tileCounts.end()) {
            newCount = count->second + 1;
        } 

        tileCounts[tiles.at(i)->getColour()] = newCount;
    }
}

bool Factory::isEmpty() {
    return tiles.size() == 0 ? true : false;
}

string Factory::toString() {
    std::string result = "";

    for (unsigned int i = 0; i != tiles.size(); ++i) {
        result += tiles.at(i)->toString();
    }

    return result;
}
