#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "../Components/headers/LinkedList.h"
#include "../Components/headers/Factory.h"
#include "../Player/headers/Player.h"
#include "../Player/headers/AI.h"
#include "GameEngineCallback.h"

#include <fstream>
#include <string>
#include <sstream>

#define NUM_FACTORIES   6
#define NUM_PLAYERS     2
#define MAX_ROWS        5

enum Error_Message{
    INVALID_COMMAND,
    SUCCESS,
    SAVED,
    COLOUR_NOT_IN_FACTORY,
    INVALID_MOVE
};

// CAN REMOVE IF NOT NEED
class AI;
class GameEngineIO;
class GameEngine{
public:
    GameEngine();
    GameEngine(const int seed, bool AItesting);
    ~GameEngine();
    void commonGameEngine();

    // Game Modes

    // AI game
    void newGame(const std::string player1Name, unsigned const int numNormalFactories, unsigned const int numCentralFactories,
                    unsigned const int numPlayers);

    // 2 Player game
    void newGame(const std::string player1Name, const std::string player2Name, unsigned const int numNormalFactories,
                    unsigned const int numCentralFactories, unsigned const int numPlayers);

    // 3 Player game
    void newGame(const std::string player1Name, const std::string player2Name, const std::string player3Name,
                    unsigned const int numNormalFactories, unsigned const int numCentralFactories, unsigned const int numPlayers);

    // 4 Player game
    void newGame(const std::string player1Name, const std::string player2Name, const std::string player3Name,
                    const std::string player4Name, unsigned const int numNormalFactories,
                    unsigned const int numCentralFactories, unsigned const int numPlayers);

    // method for setting up the common code within the new game modes
    void setUp(std::string* names, int numPlayers, int numNormalFactories, int numCentralFactories, bool AIMode, bool loading, bool oldGame);

    //Gameplay Conditions
    void gameplayLoop(bool& eof, bool& continueMenuLoop);
    bool winConditionMet();
    bool endOfRoundConditionMet();
    void endOfRoundPreparations();

    //Player turn moves
    int playerTurn(const std::string playerTurnCommand);
    std::string interpretPlayerTurn(const int result);
    void swapCurrentTurn();
    void calculatePointsPerRound();
    void calculateEndGamePoints();
    void resetGame();

    //Player input validation
    bool checkInput();
    bool checkCommand1(const std::string input, int& factoryNo);
    bool checkCommand2(const std::string input, Type& tileType);
    bool checkCommand3(const std::string input, int& storageRow);
    bool tileExistsInFactory(Type tileType, int factoryNo);
    bool inputIsInt(const std::string input);

    //Moving of tiles
    bool moveTilesFromFactory(Player* player, unsigned const int factoryNumber, unsigned const int row, const Type type, const bool toBroken);
    // to lighten code duplication. Any extra tiles move to central factory.
    void moveToCentral(Player* player, std::vector<std::shared_ptr<Tile>> tilesToMove, unsigned const int factoryNumber);
    void returnFactoryTilesByColour(Type type);
    void moveTilesToMosaicStorage(Player* player, unsigned const int factoryNumber, unsigned const int row, const Type type);
    void moveTilesToBrokenTiles(Player* player, unsigned const int factoryNumber, const Type type);
    void moveTilesToLid(Player* player);
    void movePlayerTilesToMosaic();
    void moveFirstPlayerTiles(Player* player, std::shared_ptr<Tile> firstPlayerTile, int centralFactory);

    //Component Utilities
    void populateFactories();
    void populateBagAndShuffle();
    void startGameLog();
    void setGameLog(std::vector<std::string> gameLog);
    void setReplayMode(bool replayMode);
    void setReplayIndex(int replayIndex);
    void addToGameLog(std::string line);
    void shuffle(std::shared_ptr<Tile> bagToShuffle[] , unsigned const int size);
    void addTilesByColourToBag(const Type type, std::shared_ptr<Tile> bagToShuffle[], int &bagCount);
    void clearBoxLid();
    void refillBag();
    bool changeType(Type& tileType, char tileChar);
    
    //Getters
    std::string getCurrentTurn() const;
    std::string getPlayerStartingNextRound() const;
    Player* getCurrentPlayer() const;
    Player* getPlayer(unsigned const int index);
    Factory* getFactory(unsigned const int number) const;
    LinkedList* getTileBag() const;
    LinkedList* getBoxLid() const;
    std::vector<std::string> getGameLog();
    int getSeed() const;
    int getNumPlayers() const;
    int getNumFactories() const;
    int getNumCentral() const;
    bool getAIMode() const;

    //Setters
    void setPlayer(std::string playerName, unsigned const int index);
    void setSeed(int seed);
    void setCurrentTurn(std::string playerName);
    void setPlayerStartingNextRound();

private:
    Player** players;
    Factory** factory;
    LinkedList* bag;
    LinkedList* boxLid;
    std::string currentTurn;
    std::string playerStartingNextRound;
    GameEngineCallback* gec;
    int seed;
    std::string gameMode;
    int numPlayers;
    int numFactories;
    int numCentral;
    bool noMoreTiles;
    bool AIMode;

    bool oldGameFormat;

    // for replay enhancement
    std::vector<std::string> gameLog;
    bool replayMode;
    int replayIndex;
    // this is so we can add the central factory choice after the player turn for easier replay reading.
    std::string centralChoicePlaceholder;

    // For testing purposes
    bool testing;
    bool AItesting;

};

#endif // GAMEENGINE_H