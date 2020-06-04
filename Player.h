
/*
 * Player
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <string>

#include "PlayerBoard.h"

class Player {
    public:
        // Default needed for game load
        Player();

        // Constructor will generate board and set score to 0
        Player(std::string name);

        Player(std::string name, int score, std::shared_ptr<PlayerBoard> board);

        ~Player();

        // Returns the players score
        int getScore();

        // Sets the players score
        void setScore(const int score);

        // Returns the players name
        std::string& getName();

        // Set the players name
        void setName(std::string& name);

        // Returns the players board.
        std::shared_ptr<PlayerBoard> getBoard();

        // Sets the number of rows completed by a player at end of game
        void setRowsCompleted(int count);

        // Get the number of rows completed by a player at end of game
        int getRowsCompleted();

        // Returns a string represenation of the player
        std::string toString();

        std::string getPrintable();

    private:
        std::string name;
        int score;
        std::shared_ptr<PlayerBoard> board;

        // Calculated and used only at end of game, used in tie breakers
        int rowsCompleted;

};

#endif // PLAYER_H
