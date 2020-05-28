
/*
 * Mosaic
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef MOSAIC_H
#define MOSAIC_H

#include <memory>

#include "Tile.h"

class Mosaic
{
    public:
        Mosaic();
        ~Mosaic();

        // Checks template for where in row the tile goes, returns the score
        int add(std::unique_ptr<Tile> tile, int row);

        // Adds tile to wall and returns the number of points sccored
        int add(std::unique_ptr<Tile> tile, int row, int column);

        // Returns true if given column is completed
        bool columnCompleted(int column);

        // Returns true is a row is completed
        bool rowComplete();

        // Returns true is specified row is completed
        bool rowComplete(int row);

        // Returns true if colour is in row
        bool inRow(TileColour colour, int row);

        // Builds the template array
        void buildTemplate();

        // Calculate the score for a tile placed at row, column
        int calculateScore(int row, int column);

        // Returns the amount of tiles above the specified location
        int tilesAbove(int row, int column);

        // Returns the amount of tiles below the specified location
        int tilesBelow(int row, int column);

        // Returns the amount of tiles to the left of the specified location
        int tilesLeft(int row, int right);

        // Returns the amount of tiles to the right of the specified location
        int tilesRight(int row, int column);

        // Provide a breakdown of the tiles on the wall
        void reportTileCounts(std::map<TileColour, int>& tileCounts);

        // Get a printable string for the given row
        std::string toString(int row);

        // Get a printable string for the given row of the mosaic template
        std::string toString(int row, bool useWallTemplate);

    private:
        void initialiseWall();

        // Player's tiles, as they have chosen to place them on their wall
        std::unique_ptr<Tile>** wall;

        // Matrix of which tiles go in which position on the wall
        TileColour** wallTemplate = new TileColour*[5];
};

#endif // MOSAIC_H
