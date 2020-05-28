
#include <memory>
#include <string>
#include <vector>

#include "PatternLine.h"
#include "Tile.h"

using std::move;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

PatternLine::PatternLine(unsigned int size) :
    size(size),
    tiles(vector<unique_ptr<Tile>>())
{}

PatternLine::~PatternLine() {
    tiles.clear();
}

unsigned int PatternLine::getSize() {
    return size;
}

int PatternLine::getSpace() {
    return size - tiles.size();
}

bool PatternLine::isfull() {
    return getSpace() == 0 ? true : false;
}

int PatternLine::addToWall(Mosaic& wall, int row, shared_ptr<BoxLid> lid) {
    
    // Move a tile to the wall
    int score = wall.add(move(tiles[0]), row);
    
    // Move remaining tiles to lid
    for (unsigned int i = 1; i != size; ++i) {
        lid->add(move(tiles[i]));
    }

    return score;
}

TileColour PatternLine::getColour() {
    TileColour colour = NONE;

    if (tiles.size() != 0) {
        colour = tiles[0]->getColour();
    }

    return colour;
}

void PatternLine::addTile(unique_ptr<Tile> tile) {
    if (tiles.size() != size) {
        tiles.push_back(move(tile));
    }
}

unique_ptr<Tile> PatternLine::remove() {
    // Get the tile from the end of the line
    unique_ptr<Tile> tile = move(tiles[tiles.size() - 1]);

    // Remove the tile's place on the vector, thus reducing its size
    tiles.pop_back();

    return tile;
}

void PatternLine::reportTileCounts(std::map<TileColour, int>& tileCounts) {
    for (unsigned int i = 0; i != tiles.size(); ++i) {
        int newCount = 1;
        auto count = tileCounts.find(tiles.at(i)->getColour());
        
        if (count != tileCounts.end()) {
            newCount = count->second + 1;
        } 

        tileCounts[tiles.at(i)->getColour()] = newCount;
    }
}

std::string PatternLine::toString() {
    string result = "";
    unsigned int space = getSpace();
    TileColour colour = getColour();

    for (unsigned int i = 0; i != size; ++i) {
        if (i < space) {
            result += Tile::toString(NONE);
        } else {
            result += Tile::toString(colour);
        }
    }

    return result;
}
