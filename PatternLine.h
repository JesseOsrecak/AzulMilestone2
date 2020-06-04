
/*
 * Pattern Line
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef PATTERN_LINE_H
#define PATTERN_LINE_H

#include <memory>
#include <vector>

#include "BoxLid.h"
#include "Mosaic.h"
#include "Tile.h"

class PatternLine {
    public:
        // Size is used to instantiate array should be between 1-5
        PatternLine(unsigned int size);

        ~PatternLine();

        // Returns the size of collection
        unsigned int getSize();
        
        // Returns how many spaces in collection are null (How many tiles can be inserted)
        int getSpace();

        // Returns true if it is full
        bool isfull();

        // Adds a tile to players wall, remaning tiles to the lid, and returns the score
        int addToWall(Mosaic& wall, int row, std::shared_ptr<BoxLid> lid);

        // Checks what colour, if any, tiles are in this line.
        TileColour getColour();

        // Add a tile to the pattern line.
        // Caller MUST check if there is space first, or else the pointer will
        // need to be reinstantiated.
        void addTile(std::unique_ptr<Tile> tile);

        // Removes a tile from the line
        std::unique_ptr<Tile> remove();

        // Provide a breakdown of the tiles in a pattern line
        void reportTileCounts(std::map<TileColour, int>& tileCounts);

        // Get a printable string of the tile colours
        std::string toString();
        //Returns a printable version containing coloured text
        std::string getPrintable();
        //returns the number of tiles within the patternline
        int getNumberOfTiles();

    protected:
        unsigned int size;
        std::vector<std::unique_ptr<Tile>> tiles;

};

#endif // PATTERN_LINE_H
