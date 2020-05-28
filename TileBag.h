
/*
 * Tile Bag
 * 
 * Abstract type, representing the bag of tiles.
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef TILE_BAG_H
#define TILE_BAG_H

#include <map>
#include <memory>

#include "LinkedList.h"
#include "Tile.h"

class TileBag {
    public:
        TileBag();

        // Return the number of tiles in the bag
        unsigned int getNumberOfTiles();

        // Add a tile to the bag
        void add(std::unique_ptr<Tile> tile);

        // Removes and returns a tile from the bag
        std::unique_ptr<Tile> remove();

        // Provide a breakdown of the tiles in the bag
        void reportTileCounts(std::map<TileColour, int>& tileCounts);

        // Printable list of tiles in the bag
        std::string toString();

    private:
        LinkedList tiles;
};

#endif // TILE_BAG_H
