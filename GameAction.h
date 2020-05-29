
/*
 * Game Action
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef GAME_ACTION_H
#define GAME_ACTION_H

#include <memory>
#include <string>

#include "GameTurn.h"

enum ActionType {
    TURN,
    SHOW_PLAYER,
    SHOW_COMMANDS,
    SHOW_MENU,
    NEW,
    LOAD,
    SAVE,
    CREDITS,
    EXIT,
    UNKNOWN
};

class GameAction {
    public:
        GameAction();
        GameAction(ActionType type);
        GameAction(ActionType type, std::shared_ptr<GameTurn> turn);
        GameAction(ActionType, int playerIndex);

        // Returns the type of action
        ActionType type();

        // Return the turn requested
        std::shared_ptr<GameTurn> getTurn();

        //Returns players name
        int getPlayerIndex();

    private:
        ActionType type_;
        std::shared_ptr<GameTurn> turn;
        int playerIndex;
};

#endif // GAME_ACTION_H
