
/*
 * Game Turn
 * 
 * Describes a potential turn the game may take
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef GAME_TURN_H
#define GAME_TURN_H

#include <memory>

#include "Factory.h"
#include "Player.h"

class GameTurn {
    public:
        GameTurn(std::shared_ptr<Factory> source,
                 std::shared_ptr<PatternLine> destination,
                 TileColour colour);

        // Get the source factory of the requested move
        std::shared_ptr<Factory> getSource();

        // Get the destination patternline of the requested move
        std::shared_ptr<PatternLine> getDestination();

        // Get the tile colour that the player wishes to keep from the source,
        // to place into the destination
        TileColour getColour();

    private:
        std::shared_ptr<Factory> source;
        std::shared_ptr<PatternLine> destination;
        TileColour colour;
};

#endif // GAME_TURN_H
