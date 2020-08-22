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

    // method to determine whether the file to load is the old, or new save file format
    void loadGame(std::string fileName);
    void loadReplay(std::string fileName);
    void loadGameInfo(std::string fileName);
    void saveGame(std::string fileName, bool oldSave);

    void loadPlayers(bool replayMode);
    void loadFactories();
    void loadMosaics(Player* players[]);
    void loadStorageArea(Player* players[]);
    void loadBrokenTiles(Player* players[]);
    void loadBag();
    void loadLid();
    void loadSeed();
    void loadGameLog();
    bool getReadError();
    
private:
    GameEngine* gameEngine;
    std::vector<std::string> gameInfo;
    bool readError;  
    int numPlayers;
    int numNormalFactories;
    int numCentral;
    int gameInfoIndex;
    bool oldGameFormat;
};

#endif //GAMEENGINEIO_H