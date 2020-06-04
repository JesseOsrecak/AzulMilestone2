
/*
 * Tile
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef TILE_H
#define TILE_H

#include <map>

#include "Types.h"

class Tile {
    public:
        Tile();
        Tile(TileColour colour);
        Tile(const Tile& other);
        Tile(Tile&& other);
        ~Tile();

        TileColour getColour();
        bool isStartingMarker();
        bool isEmpty();

        // Return the tile colour as a string (for save/print)
        std::string toString();

        //Return tile Colour in coloured Text for Printing;
        std::string colouredToString();

        // Get the string for a specific colour.
        static std::string toString(TileColour colour);

        // Get the string for a specific colour.
        static std::string colouredToString(TileColour colour);

    private:
        TileColour colour;

        static std::map<TileColour, std::string> colourStrings;
};

#endif // TILE_H
