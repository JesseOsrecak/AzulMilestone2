
#include "GameTurn.h"

GameTurn::GameTurn(std::shared_ptr<Factory> source,
                 std::shared_ptr<PatternLine> destination,
                 TileColour colour) :
    source(source),
    destination(destination),
    colour(colour)
{}

std::shared_ptr<Factory> GameTurn::getSource() {
    return source;
}

std::shared_ptr<PatternLine> GameTurn::getDestination() {
    return destination;
}

TileColour GameTurn::getColour() {
    return colour;
}
