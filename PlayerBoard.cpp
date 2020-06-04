
#include <memory>
#include <vector>

#include "FloorLine.h"
#include "Mosaic.h"
#include "PlayerBoard.h"

using std::make_shared;
using std::move;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;

PlayerBoard::PlayerBoard() {
    floorLine = make_shared<FloorLine>();
    wall = Mosaic();

    for (int i = 1; i != 6; ++i) {
        lines.push_back(make_shared<PatternLine>(i));
    }
    
}

PlayerBoard::~PlayerBoard() {}

shared_ptr<FloorLine> PlayerBoard::getFloorLine() {
    return floorLine;
}

shared_ptr<PatternLine> PlayerBoard::getPatternLine(int row) {
    return lines[row];
}

Mosaic& PlayerBoard::getMosaic() {
    return wall;
}

bool PlayerBoard::canAccept(TileColour colour, int row) {
    bool result = false;
    TileColour lineColour = lines[row]->getColour();
    
    // The existing row must either match the colour or be empty
    // And the corresponding row on the wall must not already contain this colour
    if ((lineColour == colour || lineColour == NONE) && !wall.inRow(colour, row)) {
        result = true;
    }

    return result;
}

void PlayerBoard::addTileToPatternLine(unique_ptr<Tile> tile, int row) {
    lines[row]->addTile(move(tile));
}

void PlayerBoard::addTileToFloorLine(unique_ptr<Tile> tile) {
    floorLine->addTile(move(tile));
}

void PlayerBoard::reportTileCounts(std::map<TileColour, int>& tileCounts) {
    for (shared_ptr<PatternLine> line : lines) {
        line->reportTileCounts(tileCounts);
    }

    floorLine->reportTileCounts(tileCounts);
    wall.reportTileCounts(tileCounts);
}

string PlayerBoard::toString() {
    string result = "";

    for (int i = 0; i != 5; ++i) {
        result += to_string(i + 1) + ": ";

        // Add pattern lines, with padding
        string line = lines[i]->toString();
        result += line.insert(0, 5 - line.size(), ' ');
        result += " || ";

        // Add player's wall
        result += wall.toString(i);
        result += " <-> ";

        // Add wall template
        result += wall.toString(i, true);

        result += "\n";
    }

    result += "Floor: ";
    result += floorLine->toString();

    return result;
}

string PlayerBoard::getPrintable()
{
    string result = "";

    for (int i = 0; i != 5; ++i) {
        result += to_string(i + 1) + ": ";

        // Add pattern lines, with padding
        string line = lines[i]->getPrintable();
        result += line.insert(0, 5 - lines[i]->getSize(), ' ');
        result += " || ";

        // Add player's wall + template
        result += wall.getPrintable(i);



        result += "\n";
    }

    result += "Floor: ";
    result += floorLine->getPrintable();

    return result;
}