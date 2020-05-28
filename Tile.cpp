
#include <map>
#include <string>

#include "Tile.h"

using std::map;
using std::string;

// Initialise static member
map<TileColour, string> Tile::colourStrings = {
    {RED, "R"},
    {DARK_BLUE, "B"},
    {YELLOW, "Y"},
    {BLACK, "U"},
    {LIGHT_BLUE, "L"},
    {FIRST, "F"},
    {NONE, "-"}
};

std::string Tile::toString(TileColour colour) {
    return colourStrings[colour];
}

Tile::Tile() :
    colour(NONE)
{}

Tile::Tile(TileColour colour) :
    colour(colour)
{}

Tile::Tile(const Tile& other) :
    colour(other.colour)
{}

Tile::Tile(Tile&& other) {
    colour = other.colour;
}

Tile::~Tile() {}

TileColour Tile::getColour() {
    return colour;
}

bool Tile::isStartingMarker() {
    return colour == FIRST ? true : false;
}

bool Tile::isEmpty() {
    return colour == NONE ? true : false;
}

std::string Tile::toString() {
    return colourStrings[colour];
}
