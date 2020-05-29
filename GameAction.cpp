
#include "GameAction.h"

using std::shared_ptr;

GameAction::GameAction() :
    type_(UNKNOWN)
{}

GameAction::GameAction(ActionType type) :
    type_(type)
{}

GameAction::GameAction(ActionType type, shared_ptr<GameTurn> turn) :
    type_(type),
    turn(turn)
{}

GameAction::GameAction(ActionType type, int playerIndex)
{
    type_ = type;
    this->playerIndex = playerIndex;
}

ActionType GameAction::type() {
    return type_;
}

shared_ptr<GameTurn> GameAction::getTurn() {
    return turn;
}

int GameAction::getPlayerIndex()
{
    return playerIndex;
}
