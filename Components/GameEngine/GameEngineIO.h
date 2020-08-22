#ifndef GAMEENGINEIO_H
#define GAMEENGINEIO_H

#include <string>

#include <fstream>
#include <string>
#include <sstream>

class GameEngine;
class GameEngineIO {
public:
    GameEngineIO(GameEngine* engine);
    ~GameEngineIO();

    void loadGame(std::string fileName);
    void loadReplay(std::string fileName);
    void saveGame(std::string fileName);

    void loadPlayers(bool replayMode);
    void loadFactories();
    void loadMosaics(Player* players[]);
    void loadStorageArea(Player* players[]);
    void loadBrokenTiles(Player* players[]);
    void loadBag();
    void loadLid();
    void loadSeed();
    void loadGameLog();
    
private:
    GameEngine* gameEngine;
    std::vector<std::string> gameInfo;
    bool readError;  
    int numPlayers;
    int numNormalFactories;
    int numCentral;
    int gameInfoIndex;
};

#endif //GAMEENGINEIO_H