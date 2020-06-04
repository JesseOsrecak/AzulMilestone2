
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

std::string Tile::colouredToString(TileColour colour) {

    std::string str = "";
    if(colourStrings[colour] == "R")
    {
        str = "\033[107;31mR\033[0;0m";
    }
    else if(colourStrings[colour] == "B")
    {
        str = "\033[107;34mB\033[0;0m";
    }
    else if(colourStrings[colour] == "Y")
    {
        str = "\033[107;33mY\033[0;0m";
    }
    else if(colourStrings[colour] == "U")
    {
        str = "\033[107;30mU\033[0;0m";
    }
    else if(colourStrings[colour] == "L")
    {
        str = "\033[107;94mL\033[0;0m";
    }
    else if(colourStrings[colour] == "F")
    {
        str= "\033[107;30mF\033[0;0m";
    }
    else
    {
        str = colourStrings[colour];
    }
    return str;
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

std::string Tile::colouredToString()
{
    std::string str = "";
    
    if(colourStrings[colour] == "R")
    {
        str = "\033[107;31mR\033[0;0m";
    }
    else if(colourStrings[colour] == "B")
    {
        str = "\033[107;34mB\033[0;0m";
    }
    else if(colourStrings[colour] == "Y")
    {
        str = "\033[107;33mY\033[0;0m";
    }
    else if(colourStrings[colour] == "U")
    {
        str = "\033[107;30mU\033[0;0m";
    }
    else if(colourStrings[colour] == "L")
    {
        str = "\033[107;94mL\033[0;0m";
    }
    else if(colourStrings[colour] == "F")
    {
        str= "\033[107;30mF\033[0;0m";
    }
    else
    {
        str = colourStrings[colour];
    }
    return str;
}
