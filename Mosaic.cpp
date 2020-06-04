
#include <memory>

#include "Mosaic.h"
#include "Types.h"
#include "Tile.h"

using std::move;
using std::string;
using std::unique_ptr;

Mosaic::Mosaic() {
    initialiseWall();
    buildTemplate();
}

void Mosaic::initialiseWall() {
    wall = new unique_ptr<Tile>*[5];

    for (int i = 0; i != 5; ++i) {
        wall[i] = new unique_ptr<Tile>[5];
        for (int j = 0; j != 5; ++j) {
            wall[i][j] = nullptr;
        }
    }
}

void Mosaic::buildTemplate() {
    for (int i = 0; i != 5; ++i) {
        wallTemplate[i] = new TileColour[5];
    }
    
    wallTemplate[0][0] = DARK_BLUE;
    wallTemplate[0][1] = YELLOW;
    wallTemplate[0][2] = RED;
    wallTemplate[0][3] = BLACK;
    wallTemplate[0][4] = LIGHT_BLUE;
    wallTemplate[1][0] = LIGHT_BLUE;
    wallTemplate[1][1] = DARK_BLUE;
    wallTemplate[1][2] = YELLOW;
    wallTemplate[1][3] = RED;
    wallTemplate[1][4] = BLACK;
    wallTemplate[2][0] = BLACK;
    wallTemplate[2][1] = LIGHT_BLUE;
    wallTemplate[2][2] = DARK_BLUE;
    wallTemplate[2][3] = YELLOW;
    wallTemplate[2][4] = RED;
    wallTemplate[3][0] = RED;
    wallTemplate[3][1] = BLACK;
    wallTemplate[3][2] = LIGHT_BLUE;
    wallTemplate[3][3] = DARK_BLUE;
    wallTemplate[3][4] = YELLOW;
    wallTemplate[4][0] = YELLOW;
    wallTemplate[4][1] = RED;
    wallTemplate[4][2] = BLACK;
    wallTemplate[4][3] = LIGHT_BLUE;
    wallTemplate[4][4] = DARK_BLUE;
}

Mosaic::~Mosaic() {
    for(int i = 0; i < 5; ++i) {
        for(int j = 0; j < 5; ++j) {
            wall[i][j] = nullptr;
        }
    }
}

int Mosaic::add(unique_ptr<Tile> tile, int row) {
    int score = 0;
    int column = 0;

    while (column != 5 && score == 0) {
        if (wallTemplate[row][column] == tile->getColour()) {
            score = add(move(tile), row, column);
        }

        ++column;
    }

    return score;
}

int Mosaic::add(unique_ptr<Tile> tile, int row, int column) {
    wall[row][column] = move(tile);

    return calculateScore(row, column);
}

int Mosaic::calculateScore(int row, int column) {
    int rowScore = tilesAbove(row, column) + tilesBelow(row, column);
    int colScore = tilesLeft(row, column) + tilesRight(row, column);

    int score = rowScore + colScore + 1;

    if (rowScore > 0 && colScore > 0) {
        // Double dip the new tile if a row and a column was formed
        score += 1;
    }

    return score;
}

int Mosaic::tilesAbove(int row, int column) {
    int count  = 0;
    int index = row - 1;

    while (index != -1 && wall[index][column] != nullptr) {
        if (!wall[index][column]->isEmpty()) {
            // if this space contains a non-empty tile, count it 
            ++count;
        } else {
            // force loop exit
            index = 0;
        }
        
        --index;
    }

    return count;
}

int Mosaic::tilesBelow(int row, int column) {
    int count = 0;
    int index = row + 1;

    while (index != 5 && wall[index][column] != nullptr) {
        if (!wall[index][column]->isEmpty()) {
            // if this space contains a non-empty tile, count it 
            ++count;
        } else {
            // force loop exit
            index = 4;
        }
        
        ++index;
    }

    return count;
}

int Mosaic::tilesLeft(int row, int column) {
    int count  = 0;
    int index = column - 1;

    while (index != -1 && wall[row][index] != nullptr) {
        if (!wall[row][index]->isEmpty()) {
            // if this space contains a non-empty tile, count it 
            ++count;
        } else {
            // force loop exit
            index = 0;
        }

        --index;
    }

    return count;
}

int Mosaic::tilesRight(int row, int column) {
    int count = 0;
    int index = column + 1;

    while (index != 5 && wall[row][index] != nullptr) {
        if (!wall[row][index]->isEmpty()) {
            // if this space contains a non-empty tile, count it 
            ++count;
        } else {
            // force loop exit
            index = 4;
        }

        ++index;
    }

    return count;
}

bool Mosaic::columnCompleted(int column) {
    bool completed = true;
    int index = 0;

    // Assume complete, until we hit nullptr or empty tile
    while (index != 5 && completed) {
        if (wall[index][column] == nullptr) {
            completed = false;
        } else if (wall[index][column]->isEmpty()) {
            completed = false;
        }

        ++index;
    }

    return completed;
}

bool Mosaic::rowComplete() {
    bool completed = false;
    int index = 0;

    while (index != 5 && !completed) {
        completed = rowComplete(index);
        ++index;
    }

    return completed;
}

bool Mosaic::rowComplete(int row) {
    bool completed = true;
    int index = 0;

    // Assume complete, until we hit nullptr or empty tile
    while (index != 5 && completed) {
        if (wall[row][index] == nullptr) {
            completed = false;
        } else if (wall[row][index]->isEmpty()) {
            completed = false;
        }

        ++index;
    }

    return completed;
}

bool Mosaic::inRow(TileColour colour, int row) {
    bool inRow = false;
    int index = 0;

    while (index != 5 && !inRow) {
        if (wall[row][index] != nullptr) {
            if(wall[row][index]->getColour() == colour) {
                inRow = true;
            }
        }
        
        ++index;
    }

    return  inRow;
}

void Mosaic::reportTileCounts(std::map<TileColour, int>& tileCounts) {
    for (int row = 0; row != 5; ++row) {
        for (int col = 0; col != 5; ++col) {
            if (wall[row][col] != nullptr) {
                int newCount = 1;
                auto count = tileCounts.find(wall[row][col]->getColour());
                
                if (count != tileCounts.end()) {
                    newCount = count->second + 1;
                } 

                tileCounts[wall[row][col]->getColour()] = newCount;
            }
        }
    }
}

string Mosaic::toString(int row) {
    return toString(row, false);
}

string Mosaic::toString(int row, bool useWallTemplate) {
    string result = "";

    for (int i = 0; i != 5; ++i) {
        if(useWallTemplate) {
            result += Tile::toString(wallTemplate[row][i]);
        } else {
            if (wall[row][i] != nullptr) {
                result += wall[row][i]->toString();
            } else {
                result += Tile::toString(NONE);
            }
            
        }
    }

    return result;
}

string Mosaic::getPrintable(int row)
{
    string result = "";



    for(int col = 0; col < 5; ++col)
    {
        if(wall[row][col] != nullptr)
        {
            result += wall[row][col]->colouredToString();
        }
        else
        {
            result += Tile::colouredToString(NONE);
        }
        
        
    }

    result += " <-> ";

    for(int col = 0; col < 5; ++col)
    {
        result += Tile::colouredToString(wallTemplate[row][col]);
    }

    return result;
}
