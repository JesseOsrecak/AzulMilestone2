
#include "BoxLid.h"

using std::make_shared;
using std::make_unique;
using std::map;
using std::unique_ptr;

BoxLid::BoxLid() :
    tiles(LinkedList())
{}

unsigned int BoxLid::getNumberOfTiles() {
    return tiles.size();
}

/* We're converting from unique to shared, so the underlying data structure
 * can freely copy items in its algorithms. That is to say, we are *trusting*
 * the linked list implementation to never make a duplicate tile and to never
 * lose a tile.
 */
void BoxLid::add(std::unique_ptr<Tile> tile) {
    tiles.addBack(make_shared<Tile>(Tile(*tile)));
}

/* Convert back to a unique pointer to signal to the calling class that it is
 * now the owner of, and responsible for, the object.
 */
unique_ptr<Tile> BoxLid::remove() {
    return make_unique<Tile>(*tiles.deleteFront());
}

void BoxLid::reportTileCounts(std::map<TileColour, int>& tileCounts) {

    for (unsigned int i = 0; i != tiles.size(); ++i) {
        int newCount = 1;
        auto count = tileCounts.find(tiles.get(i)->getColour());
        
        if (count != tileCounts.end()) {
            newCount = count->second + 1;
        } 

        tileCounts[tiles.get(i)->getColour()] = newCount;
    }
}

std::string BoxLid::toString() {
    std::string result = "";

    for (unsigned int i = 0; i != tiles.size(); ++i) {
        result += tiles.get(i)->toString();
    }

    return result;
}
