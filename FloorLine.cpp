
#include <string>

#include "FloorLine.h"

#define LINE_SIZE   7

using std::string;

FloorLine::FloorLine() :
    PatternLine(LINE_SIZE)
{}
        
string FloorLine::toString() {
    string result = "";

    for (unsigned int i = 0; i != LINE_SIZE; ++i) {
        if (i < tiles.size()) {
            result += tiles[i]->toString();
        } else {
            result += Tile::toString(NONE);
        }
    }

    return result;
}

string FloorLine::getPrintable()
{
    string result = "";

    for (unsigned int i = 0; i != LINE_SIZE; ++i) {
        if (i < tiles.size()) {
            result += tiles[i]->colouredToString();
        } else {
            result += Tile::colouredToString(NONE);
        }
    }

    return result;
}
