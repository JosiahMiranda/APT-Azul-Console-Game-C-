#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <memory>
#include "MosaicStorage.h"
#include "../../Input.h"

class Player{
public:
    Player(std::string name);
    virtual ~Player();
    int getPoints();
    void setPoints(unsigned const int points);
    std::string getName();
    std::shared_ptr<MosaicStorage> getMosaicStorage();

    // Player class now handles input itself
    virtual std::string chooseTurn();
    virtual std::string chooseCentral();
    
private:
    int points;
    std::string name;
    std::shared_ptr<MosaicStorage> mosaicStorage;
    Input input;
};

#endif // PLAYER_H