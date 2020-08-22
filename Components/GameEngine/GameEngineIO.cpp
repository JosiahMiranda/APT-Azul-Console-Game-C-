#include "GameEngine.h"
#include "GameEngineIO.h"


GameEngineIO::GameEngineIO(GameEngine* engine){
    this->gameEngine = engine;
    this->readError = false;
}
GameEngineIO::~GameEngineIO(){}

void GameEngineIO::loadGame(std::string fileName) {

    // Load in each line of the file as a unique game info string.
    std::ifstream ifs (fileName);
    std::string currentInfo;

    if(!ifs.is_open()){
        throw "File not found";
    }

    while (ifs.good()) {
        std::getline(ifs, currentInfo);
        gameInfo.push_back(currentInfo);
    }

    ifs.close();

    //load the individual components from the file
    loadPlayers(false);

    if (!readError) {
        loadFactories();
    }

    if (!readError) {
        Player* players[this->numPlayers];
        for (int player = 0; player < this->numPlayers; ++player) {
            players[player] = gameEngine->getPlayer(player);
        }

        if (!readError) {
            loadMosaics(players);
        }
        if (!readError) {
            loadStorageArea(players);
        }
        if (!readError) {
            loadBrokenTiles(players);
        }
        if (!readError) {
            loadLid();
        }
        if (!readError) {
            loadBag();
        }
        if (!readError) {
            loadSeed();
        }
        if (!readError) {
            loadGameLog();
        }

        std::cout << "CHECKING" << std::endl;
        for (std::string line : gameEngine->getGameLog()) {
            std::cout << line << " is in game log." << std::endl;
        }
    }

    if(readError){
        throw "There was an error reading the file.";
    }

}

void GameEngineIO::loadReplay(std::string fileName) {

    // Load in each line of the file as a unique game info string.
    std::ifstream ifs (fileName);
    std::string currentInfo;

    if(!ifs.is_open()){
        throw "File not found";
    }

    while (ifs.good()) {
        std::getline(ifs, currentInfo);
        gameInfo.push_back(currentInfo);
    }

    ifs.close();

    loadPlayers(true);

    // increment gameInfoIndex to start at game log.

    // start at 2 because 0 and 1 are set for game mode and num of central factories always
    gameInfoIndex = 2; 
    // now skip every name and points part.
    gameInfoIndex += (numPlayers*2);
    // now it's on the current player's turn.
    ++gameInfoIndex;
    // now we're at the factories. skip all of them
    int numFactories = numCentral + numNormalFactories;
    gameInfoIndex += numFactories;
    // now we're at the first player's mosaic.
    // skip mosaic and mosaic storage of each player
    gameInfoIndex += (numPlayers*(MAX_ROWS*2));
    // now we're at the first player's broken tiles. skip them all.
    gameInfoIndex += numPlayers;
    // now we're at the box lid. skip to the seed.
    gameInfoIndex += 2;
    loadSeed();
    // Now we're at the beginning of the gamelog.
    loadFactories();
    loadBag();

    gameEngine->startGameLog();
    
    loadGameLog();


    std::cout << "CHECKING" << std::endl;
    for (std::string line : gameEngine->getGameLog()) {
        std::cout << line << " is in game log." << std::endl;
    }

    // now we can load in user turns

    if(readError){
        throw "There was an error reading the file.";
    }

}

void GameEngineIO::loadPlayers(bool replayMode) {

    // starts at first index of game info, which is game mode
    this->gameInfoIndex = 0;
    bool AIMode = false;
    
    // reading in game mode
    if (gameInfo[gameInfoIndex] == "AI") {
        this->numPlayers = 2;
        AIMode = true;
    } else if (gameEngine->inputIsInt(gameInfo[this->gameInfoIndex])) {
        std::stringstream gameMode(gameInfo[this->gameInfoIndex]);
        gameMode >> this->numPlayers;
    } else {
        readError = true;
    }


    std::string names[numPlayers];
    
    // automatic setters with num of players and factories
    if (!readError) {
        if (numPlayers == 2) {
            this->numNormalFactories = 5;
        } else if (numPlayers == 3) {
            this->numNormalFactories = 7;
        } else if (numPlayers == 4) {
            this->numNormalFactories = 9;
        } else {
            readError = true;
        }
    }

    if (!readError) {
        // increases to now move on to num central factories
        ++this->gameInfoIndex;
        // reading in number of central factories
        if (gameEngine->inputIsInt(gameInfo[this->gameInfoIndex])) {
            std::stringstream gameMode(gameInfo[this->gameInfoIndex]);
            gameMode >> this->numCentral;
        } else {
            readError = true;
        }
    }


    if (!readError) {
        if (this->numCentral != 1 && this->numCentral != 2) {
            readError = true;
        }
    }

    if (!readError) {

        // increases again, for reading in player names
        ++this->gameInfoIndex;
        for (int name = 0; name < numPlayers; ++name) {
            names[name] = gameInfo[this->gameInfoIndex];
            ++this->gameInfoIndex;
        }
    }

    if (!readError) {
        gameEngine->setUp(names, this->numPlayers, this->numNormalFactories, this->numCentral, AIMode, true);
        // set replay mode if true, after setting up, since setUp method automatically sets replayMode to false
        gameEngine->setReplayMode(replayMode);
    }

    if (!readError) {
        if (!replayMode) {
            for (int player = 0; player < numPlayers; ++player) {
                if (gameEngine->inputIsInt(gameInfo[this->gameInfoIndex])) {
                    std::stringstream playerPoints(gameInfo[this->gameInfoIndex]);
                    int points;
                    playerPoints >> points;
                    gameEngine->getPlayer(player)->setPoints(points);
                    ++this->gameInfoIndex;
                } else {
                    readError = true;
                }
            }
        } else {
            // if its replay mode, we want it to be the start of the game. So all players have 0 points.
            for (int player = 0; player < numPlayers; ++player) {
                gameEngine->getPlayer(player)->setPoints(0);
            }
        }
    }

    if (!replayMode) {
        gameEngine->setCurrentTurn(gameInfo[this->gameInfoIndex]);
        ++this->gameInfoIndex;
    } else {
        // if it's replay mode, the current turn is the first player always
        gameEngine->setCurrentTurn(gameEngine->getPlayer(0)->getName());
    }

}

void GameEngineIO::loadFactories(){
    int numFactories = this->numCentral + this->numNormalFactories;
    for (int i = 0; i < numFactories; ++i) {

        std::stringstream factoryStream(gameInfo[this->gameInfoIndex]);
        char toAdd;
        factoryStream >> toAdd;
        while (factoryStream.good()) {
            
            Type tileType = Type::NONE;
            if(toAdd == '.'){
                // Do nothing
            }
            // If the Type is changed from NONE to something else, then add it to the Factory.
            else if (gameEngine->changeType(tileType, toAdd)) {
                std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);

                gameEngine->getFactory(i)->addTile(tile);
            } else {
                readError = true;
            }

            factoryStream >> toAdd;
        }

        ++this->gameInfoIndex;
    }
}

void GameEngineIO::loadMosaics(Player* players[]){
    for (int playerNum = 0; playerNum < this->numPlayers; ++playerNum) {

        for (int row = 0; row < MAX_ROWS; ++row) {

            std::stringstream playerMosaicStream(gameInfo[this->gameInfoIndex]);
            char toAdd;
            playerMosaicStream >> toAdd;

            for (int col = 0; col < MAX_COLS; ++col) {
                if (playerMosaicStream.good()) {
                    Type tileType = Type::NONE;
                    if (gameEngine->changeType(tileType, toAdd)) {
                        std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
                        if (!players[playerNum]->getMosaicStorage()->getMosaic()->addTile(tile, row, col)) {
                            readError = true;
                            tile = nullptr;
                        }
                    }
                    playerMosaicStream >> toAdd;
                }
            }
            ++this->gameInfoIndex;
            
        }
    }

    for (int player = 0; player < this->numPlayers; ++player) {
        gameEngine->getPlayer(player)->getMosaicStorage()->getMosaic()->resetPoints();
    }

}

void GameEngineIO::loadStorageArea(Player* players[]){ 

    for (int playerNum = 0; playerNum < this->numPlayers; ++playerNum) {

        for (int row = 0; row < MAX_ROWS; ++row) {

            std::stringstream mosaicStorageStream(gameInfo[this->gameInfoIndex]); 

            char toAdd;
            mosaicStorageStream >> toAdd;

            while (mosaicStorageStream.good()) {
                
                Type tileType = Type::NONE;
                if (gameEngine->changeType(tileType, toAdd)) {
                    std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);

                    if (players[playerNum]->getMosaicStorage()->isValidAdd(tileType, row)) {
                        players[playerNum]->getMosaicStorage()->addTile(tile, row);
                    } else {
                        readError = true;
                        tile = nullptr;
                    }
                }
                mosaicStorageStream >> toAdd;
            }

            ++this->gameInfoIndex;
        }
    }
}

void GameEngineIO::loadBrokenTiles(Player* players[]){

    for (int playerNum = 0; playerNum < this->numPlayers; ++playerNum) {

        std::stringstream brokenTileStream(gameInfo[this->gameInfoIndex]); // 31 and 32
        char toAdd;
        brokenTileStream >> toAdd;
        while (brokenTileStream.good()) {
            
            Type tileType = Type::NONE;
            if(toAdd == '.'){
                // Do nothing
            }
            else if (gameEngine->changeType(tileType, toAdd)) {
                std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
                players[playerNum]->getMosaicStorage()->getBrokenTiles()->addTile(tile);
            } else {
                readError = true;
            }
            brokenTileStream >> toAdd;
        }

        ++this->gameInfoIndex;
    }
}

void GameEngineIO::loadLid(){
    
    // Load Box Lid Tiles into  game
    std::stringstream boxLidStream(gameInfo[this->gameInfoIndex]);
    // Going to need to traverse the vector backwards since our box lid only has an add to front method
    std::vector<char> tilesToAdd;
    char toAdd;
    boxLidStream >> toAdd;

    while (boxLidStream.good()) {
        tilesToAdd.push_back(toAdd);
        boxLidStream >> toAdd;
    }

    unsigned int lastIndex = tilesToAdd.size() - 1;
    for (int i = lastIndex; i >= 0; --i) {

        Type tileType = Type::NONE;
        if (gameEngine->changeType(tileType, tilesToAdd.at(i))) {
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
            gameEngine->getBoxLid()->addTileToFront(tile);
        } else {
            readError = true;
        }
    }
    tilesToAdd.clear();

    ++this->gameInfoIndex;
}

void GameEngineIO::loadBag(){
    // Load Bag Tiles into game
    std::stringstream tileBagStream(gameInfo[this->gameInfoIndex]);
    char toAdd;
    std::vector<char> tilesToAdd;
    // Going to need to traverse the vector backwards since our tile bag only has an add to front method
    tileBagStream >> toAdd;

    while (tileBagStream.good()) {
        tilesToAdd.push_back(toAdd);
        tileBagStream >> toAdd;
    }

    unsigned int lastIndex = tilesToAdd.size() - 1;
    for (int i = lastIndex; i >= 0; --i) {
        Type tileType = Type::NONE;
        if (gameEngine->changeType(tileType, tilesToAdd.at(i))) {
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
            gameEngine->getTileBag()->addTileToFront(tile);
        } else {
            readError = true;
        }
    }
    tilesToAdd.clear();

    ++this->gameInfoIndex;
}

void GameEngineIO::loadSeed(){
    int seed;
    std::stringstream seedStream(gameInfo[this->gameInfoIndex]);
    seedStream >> seed;
    gameEngine->setSeed(seed);
    ++gameInfoIndex;
}

void GameEngineIO::loadGameLog() {
    int size = gameInfo.size();
    while (gameInfoIndex < size) {
        if (gameInfo[gameInfoIndex] != "") {
            gameEngine->addToGameLog(gameInfo[gameInfoIndex]);
        }
        ++gameInfoIndex;
    }

    // here we set the replay index.
    int replayIndex = 0;

    // skip all factories
    replayIndex += numCentral;
    replayIndex += numNormalFactories;
    // skip tile bag in game log.
    ++replayIndex;
    // now we are at the part of the gamelog where user turns were recorded
    gameEngine->setReplayIndex(replayIndex);

}

void GameEngineIO::saveGame(std::string fileName) {
    std::ofstream outFile;
    outFile.open(fileName);

    if (gameEngine->getAIMode()) {
        outFile << "AI" << std::endl;
    } else {
        outFile << gameEngine->getNumPlayers() << std::endl;
    }
    outFile << gameEngine->getNumCentral() << std::endl;

    std::vector<Player*> players;

    for (int player = 0; player < gameEngine->getNumPlayers(); ++player) {
        players.push_back(gameEngine->getPlayer(player));
    }


    if(outFile.good()){
        //save player info
        for (Player* player : players) {
            outFile << player->getName() << std::endl;
        }


        for (Player* player : players) {
            outFile << player->getPoints() << std::endl;
        }


        outFile << gameEngine->getCurrentTurn() << std::endl; 


        //save factories
        for(int factory = 0; factory < gameEngine->getNumFactories(); factory++){
            outFile << gameEngine->getFactory(factory)->toSave() << std::endl;
        }


        //save mosaics 

        for (Player* player : players) {
            for (int row = 0; row < MAX_ROWS; ++row) {
                outFile << player->getMosaicStorage()->getMosaic()->rowToSave(row) << std::endl;
            }
        }


        // save mosaic storages
        for (Player* player : players) {
            for (int row = 0; row < MAX_ROWS; ++row) {
                outFile << player->getMosaicStorage()->rowToSave(row) << std::endl;
            }
        }

        //save brokenTiles
        for (Player* player : players) {
            outFile << player->getMosaicStorage()->getBrokenTiles()->toSave() << std::endl;
        }

        //save box lid tiles
        outFile << gameEngine->getBoxLid()->toSave() <<std::endl;


        //save bag tiles
        outFile << gameEngine->getTileBag()->toSave() << std::endl;


        //save random seed
        outFile << gameEngine->getSeed() << std::endl;

        // save the game log

        for (std::string line : gameEngine->getGameLog()) {
            outFile << line << std::endl;
        }
    }

    outFile.close();
}