
/*
 * Box lid
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef BOX_LID_H
#define BOX_LID_H

#include "LinkedList.h"

class BoxLid {
    public:
        BoxLid();
        
        // Return the number of tiles in the lid
        unsigned int getNumberOfTiles();

        // Add a tile to the lid
        void add(std::unique_ptr<Tile> tile);

        // Removes and returns a tile from the bag
        std::unique_ptr<Tile> remove();

        // Provide a breakdown of the tiles in the lid
        void reportTileCounts(std::map<TileColour, int>& tileCounts);

        // Printable list of tiles in the lid
        std::string toString();
        
    private:
        LinkedList tiles;
};

#endif // BOX_LID_H
