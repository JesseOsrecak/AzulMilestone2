
#include <memory>

#include "Player.h"

using std::to_string;

Player::Player() :
    Player(std::string("DEFAULT"))
{}

Player::Player(std::string name) {
    this->name = name;
    score = 0;
    board = std::make_shared<PlayerBoard>();
    rowsCompleted = 0;
}

Player::Player(std::string name, int score, std::shared_ptr<PlayerBoard> board) {
    this->name = name;
    this->score = score;
    this->board = board;
}

Player::~Player() {
    // delete board;
}

int Player::getScore() {
    return score;
}

void Player::setScore(const int score) {
    this->score = score;
}

std::string& Player::getName() {
    return name;
}

void Player::setName(std::string& name) {
    this->name = name;
}

std::shared_ptr<PlayerBoard> Player::getBoard() {
    return board;
}

void Player::setRowsCompleted(int count) {
    rowsCompleted = count;
}

int Player::getRowsCompleted() {
    return rowsCompleted;
}

std::string Player::toString() {
    std::string data = "Name: " + name + ", ";
    data += "Score: " + to_string(score) + "\n";
    data += board->toString();

    return data;
}

std::string Player::getPrintable() {
    std::string data = "Name: " + name + ", ";
    data += "Score: " + to_string(score) + "\n";
    data += board->getPrintable();

    return data;
}
