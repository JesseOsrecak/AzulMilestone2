
/*
 * Factory
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef FACTORY_H
#define FACTORY_H

#include <memory>
#include <vector>

#include "Tile.h"

class Factory {
    public:
        Factory();

        void add(std::unique_ptr<Tile> tile);

        // Checks if the factory contains a colour
        bool contains(TileColour colour);

        // Return a list of all the tiles
        std::vector<std::unique_ptr<Tile>> getTiles();

        // Provide a breakdown of the tiles in the factory
        void reportTileCounts(std::map<TileColour, int>& tileCounts);

        // Returns true if the factory is empty
        bool isEmpty();

        // Get a printable version of the tiles in the factory.
        std::string toString();
        
    protected:
        std::vector<std::unique_ptr<Tile>> tiles;
};

#endif // FACTORY_H
