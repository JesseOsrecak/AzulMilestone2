
/*
 * Player Board
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef PLAYERBOARD_H
#define PLAYERBOARD_H

#include <memory>

#include "FloorLine.h"
#include "Mosaic.h"
#include "PatternLine.h"
#include "Types.h"

class PlayerBoard {
    public:
        PlayerBoard();
        ~PlayerBoard();

        std::shared_ptr<FloorLine> getFloorLine();

        std::shared_ptr<PatternLine> getPatternLine(int row);

        Mosaic& getMosaic();

        // Checks both the colour and mosaic template, to see if moving a tile
        // of the given colour is permitted
        bool canAccept(TileColour colour, int row);
        
        // Adds a tile to a pattern line on the specified row.
        // The caller of this method is responsible for ensuring there is
        // room in this pattern line BEFORE calling, otherwise the tile 
        // pointer will need to be reinstantiated.
        void addTileToPatternLine(std::unique_ptr<Tile> tile, int row);

        // Adds a tile to the floor line
        void addTileToFloorLine(std::unique_ptr<Tile> tile);

        // Provide a breakdown of the tiles on a board
        void reportTileCounts(std::map<TileColour, int>& tileCounts);

        // Return a printable version of the player board
        std::string toString();

        //Returns a coloured printable version of the playerboard
        std::string getPrintable();

    private:
        std::shared_ptr<FloorLine> floorLine;
        Mosaic wall;

        // PatternLines
        std::vector<std::shared_ptr<PatternLine>> lines;

};

#endif // PLAYERBOARD_H
