#include "headers/Player.h"

Player::Player(std::string name){
    this->points = 0;
    this->mosaicStorage = std::make_shared<MosaicStorage>();
    this->name = name;
}

Player::~Player(){
}

std::shared_ptr<MosaicStorage> Player::getMosaicStorage(){
    return this->mosaicStorage;
}

int Player::getPoints(){
    return this->points;
}

void Player::setPoints(unsigned const int points){
    this->points = points;
}

std::string Player::getName(){
    return this->name;
}

std::string Player::chooseTurn() {
    return input.getString();
}

std::string Player::chooseCentral() {
    return input.getString();
}

