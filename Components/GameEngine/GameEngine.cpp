#include "GameEngine.h"
#include "GameEngineIO.h"
#include <random>
#include "../Input.h"


GameEngine::GameEngine() {
    commonGameEngine();
    this->testing = false;
}

GameEngine::GameEngine(const int seed) {
    commonGameEngine();
    this->seed = seed;
    this->testing = true;
}

void GameEngine::commonGameEngine(){
    gec = new GameEngineCallback();

    this->bag = new LinkedList();
    this->boxLid = new LinkedList();

    this->currentTurn = "";
    this->playerStartingNextRound = "";
    // this->playerOne = nullptr;
    // this->playerTwo = nullptr;
}

GameEngine::~GameEngine() {

    delete gec;
    delete bag;   
    delete boxLid;

    // deleting players
    for (int player = 0; player < numPlayers; ++player) {
        if (players[player] != nullptr) {
            delete players[player];
            players[player] = nullptr;
        }
    } 

    // deleting factories
    for(int i = 0;  i < numFactories; ++i){
        if(factory[i] != nullptr){
            delete factory[i];
            factory[i] = nullptr;
        }
    }

}

//setters

void GameEngine::setPlayer(std::string playerName, unsigned const int index) {
    players[index] = new Player(playerName);
}

void GameEngine::setSeed(int seed){
    this->seed = seed;
}

void GameEngine::setPlayerStartingNextRound(std::string playerName){
    this->playerStartingNextRound = playerName;
}

void GameEngine::setCurrentTurn(std::string playerName){
    this->currentTurn = playerName;
}

void GameEngine::swapCurrentTurn(){

    bool swapped = false;
    for (int player = 0; player < numPlayers && !swapped; ++player) {
        if (players[player]->getName() == this->currentTurn) {
            swapped = true;
            int newPlayerIndex = player + 1;
            if (newPlayerIndex == numPlayers) {
                newPlayerIndex = 0;
            }
            this->currentTurn = players[newPlayerIndex]->getName();
        }
    }
}


//getters

int GameEngine::getNumPlayers() const {
    return this->numPlayers;
}

int GameEngine::getNumFactories() const {
    return this->numFactories;
}

int GameEngine::getNumCentral() const {
    return this->numCentral;
}

Player* GameEngine::getPlayer(unsigned const int index) {
    return players[index];
}

std::vector<std::string> GameEngine::getGameLog() {
    return gameLog;
}

bool GameEngine::getAIMode() const {
    return this->AIMode;
}

Factory* GameEngine::getFactory(unsigned const int number) const {
    return factory[number];
}

LinkedList* GameEngine::getTileBag()const {
    return bag;
}

LinkedList* GameEngine::getBoxLid() const {
    return boxLid;
}

std::string GameEngine::getPlayerStartingNextRound() const {
    return this->playerStartingNextRound;
}

Player* GameEngine::getCurrentPlayer() const {
    Player* toReturn = nullptr;

    for (int player = 0; player < numPlayers; ++player) {
        if (players[player]->getName() == this->currentTurn) {
            toReturn = players[player];
        }
    }

    return toReturn;
}

std::string GameEngine::getCurrentTurn() const {
    return this->currentTurn;
}

int GameEngine::getSeed() const {
    return this->seed;
}


//gameplay

void GameEngine::newGame(const std::string player1Name, unsigned const int numNormalFactories,
                            unsigned const int numCentralFactories, unsigned const int numPlayers) {

    std::string names[numPlayers];
    names[0] = player1Name;
    setUp(names, numPlayers, numNormalFactories, numCentralFactories, true, false);
}

void GameEngine::newGame(const std::string player1Name, const std::string player2Name, unsigned const int numNormalFactories,
                            unsigned const int numCentralFactories, unsigned const int numPlayers) {

    std::string names[numPlayers];
    names[0] = player1Name;
    names[1] = player2Name;
    setUp(names, numPlayers, numNormalFactories, numCentralFactories, false, false);
}

void GameEngine::newGame(const std::string player1Name, const std::string player2Name, const std::string player3Name,
                        unsigned const int numNormalFactories, unsigned const int numCentralFactories, unsigned const int numPlayers) {

    std::string names[numPlayers];
    names[0] = player1Name;
    names[1] = player2Name;
    names[2] = player3Name;
    setUp(names, numPlayers, numNormalFactories, numCentralFactories, false, false);
}

void GameEngine::newGame(const std::string player1Name, const std::string player2Name, const std::string player3Name,
                        const std::string player4Name, unsigned const int numNormalFactories,
                        unsigned const int numCentralFactories, unsigned const int numPlayers) {

    std::string names[numPlayers];
    names[0] = player1Name;
    names[1] = player2Name;
    names[2] = player3Name;
    names[3] = player4Name;
    setUp(names, numPlayers, numNormalFactories, numCentralFactories, false, false);
}

void GameEngine::setUp(std::string* names, int numPlayers, int numNormalFactories, int numCentralFactories, bool AIMode, bool loading) {
    this->noMoreTiles = false;
    this->AIMode = AIMode;
    this->numPlayers = numPlayers;
    this->numCentral = numCentralFactories;
    this->numFactories = numNormalFactories + numCentralFactories;
    this->players = new Player*[numPlayers];
    this->factory = new Factory*[numFactories];
    this->centralChoicePlaceholder = "";
    this->replayIndex = 0;
    this->replayMode = false;

    for(int i = 0; i< numFactories; ++i){
        factory[i] = new Factory();
    }

    for (int player = 0; player < numPlayers; ++player) {
        if (AIMode && player == 1) {
            players[player] = new AI("BOT", *this);
        } else {
            // players[player] = new Player(names[player]);
            players[player] = new AI(names[player], *this);
        }

        players[player]->setPoints(0);
        players[player]->getMosaicStorage()->getMosaic()->resetPoints();
    }

    currentTurn = players[0]->getName();
    playerStartingNextRound = "";

    
    if (!loading) {
        populateBagAndShuffle();
        populateFactories();
        startGameLog();
    }

}

void GameEngine::startGameLog() {
    // will store initial tilebag, and factories into the game log first. the user turns will be stored as the game plays
    for (int factory = 0; factory < numFactories; ++factory) {
        gameLog.push_back(getFactory(factory)->toSave());
    }

    gameLog.push_back(bag->toSave());

}

void GameEngine::addToGameLog(std::string line) {
    gameLog.push_back(line);
}

void GameEngine::setGameLog(std::vector<std::string> gameLog) {
    this->gameLog = gameLog;

    for (std::string line : gameLog) {
        std::cout << line << " is in game log" << std::endl;
    }
}

void GameEngine::setReplayMode(bool replayMode) {
    this->replayMode = replayMode;
}

void GameEngine::setReplayIndex(int replayIndex) {
    this->replayIndex = replayIndex;
}

int GameEngine::playerTurn(std::string playerTurnCommand){
    Input input;
    int toReturn = 1;
    std::stringstream commandLine(playerTurnCommand);
    std::string commandPart;

    //0 = "turn", 1 = factory num, 2 = Tile, 3 = storage row
    std::string commands[4];
    commandLine >> commandPart;

    int counter = 0;
    while (commandLine.good() || counter < 4) {
        commands[counter] = commandPart;
        commandLine >> commandPart;
        counter++;
    }

    if(commands[0] == "turn"){

        //pass by ref to get converted values (if possible)
        int factoryNo;
        Type tileType;
        int storageRow;

        //validate the three command arguments first before proceeding
        if(checkCommand1(commands[1], factoryNo) && checkCommand2(commands[2], tileType) && checkCommand3(commands[3],storageRow)){

            if(!tileExistsInFactory(tileType, factoryNo)){
                toReturn = Error_Message::COLOUR_NOT_IN_FACTORY;
            }

            //continue if the aforementioned checks pass
            if(toReturn == Error_Message::SUCCESS){
                if (commands[3] != "B") {
                    if (moveTilesFromFactory(this->getCurrentPlayer(),factoryNo,(storageRow-1),tileType, false)) {
                        // here we want to do the check to see which central factory they want to move to 
                        toReturn = Error_Message::SUCCESS;
                    } else {
                        toReturn = Error_Message::INVALID_MOVE;
                    }
                } else {
                    if (moveTilesFromFactory(this->getCurrentPlayer(),factoryNo,(storageRow-1),tileType, true)) {
                        toReturn = Error_Message::SUCCESS;
                    } else {
                        toReturn = Error_Message::INVALID_MOVE;
                    }
                }
            }  
        } else{
            toReturn = Error_Message::INVALID_COMMAND;
        }
    } else if (commands[0] == "save") {
        GameEngineIO* geIO = new GameEngineIO(this);
        geIO->saveGame(commands[1]);
        toReturn = Error_Message::SAVED;
        delete geIO;
    } else {
        toReturn = Error_Message::INVALID_COMMAND;
    }

    // this is how we keep track of the players turns in a replay mode.
    if (toReturn == Error_Message::SUCCESS && !replayMode) {
        gameLog.push_back(playerTurnCommand);
        if (centralChoicePlaceholder != "") {
            gameLog.push_back(centralChoicePlaceholder);
            centralChoicePlaceholder = "";
        }
    }

    return toReturn;
}

//check if input is int and valid
bool GameEngine::checkCommand1(const std::string input, int& factoryNo){
    bool success = inputIsInt(input);

    if(success){
        std::stringstream factoryNoAsString(input);
        factoryNoAsString >> factoryNo;       
        if(factoryNo < 0 || factoryNo > numFactories - 1){
            success = false;
        }
    }
    return success;
}

//check if input isn't first player tile and is a valid colour
bool GameEngine::checkCommand2(const std::string input, Type& tileType){
    bool success = true;
    if (input.length() != 1) {
        success = false;
    } else if (input[0] == 'F') {
        success = false;
    } else {
        success = changeType(tileType,input[0]);
    }
    return success;
}

//check if storage row is convertible to int or if trying to move to broken tiles
bool GameEngine::checkCommand3(const std::string input, int& storageRow){
    bool success = false;

    if(input.length() == 1){
        success = true;
        if (inputIsInt(input)) {
            std::stringstream storageRowNoAsString(input);
            storageRowNoAsString >> storageRow;

            if(storageRow < 1 || storageRow > 5)
                success = false;

        } else if (input[0] != 'B') {
            success = false;
        }
    }
    return success;
}

bool GameEngine::inputIsInt(const std::string input){
    bool success = true;
    for(unsigned int i = 0; i < input.length(); i++){
        if(std::isalpha(input[i]))
            success = false;
    }
    return success;
}

bool GameEngine::tileExistsInFactory(Type tileType, int factoryNo){
    bool success = false;
    std::vector<std::shared_ptr<Tile>> chosenFactory = factory[factoryNo]->getAllTiles();
    int size = chosenFactory.size();

    for(int i = 0; i < size; ++i){
        if(chosenFactory[i]->getType() == tileType)
            success = true;
    }
    return success;
}

//must check each round if there is a full row so the game can end
bool GameEngine::winConditionMet(){
    bool winConditionMet = false;

    Mosaic* playerMosaic = nullptr;

    for (int player = 0; player < numPlayers; ++player) {
        playerMosaic = players[player]->getMosaicStorage()->getMosaic();
        if (playerMosaic->findFullRow()) {
            winConditionMet = true;
        }
    }

    return winConditionMet;
}

//convert character version of tiles to enums for more efficient use in the program
bool GameEngine::changeType(Type& tileType, char tileChar){
    bool changed = true;
    if (tileChar == RED_CHAR) {
        tileType = Type::RED;
    } else if (tileChar == YELLOW_CHAR) {
        tileType = Type::YELLOW;
    } else if (tileChar == DARKBLUE_CHAR) {
        tileType = Type::DARK_BLUE;
    } else if (tileChar == LIGHTBLUE_CHAR) {
        tileType = Type::LIGHT_BLUE;
    } else if (tileChar == BLACK_CHAR) {
        tileType = Type::BLACK;
    } else if (tileChar == FIRST_CHAR) {
        tileType = Type::FIRST_PLAYER;
    } else {
        changed = false;
    }
    return changed;
}

//calculates the points gained and lost through broken tiles and mosaic 
void GameEngine::calculatePointsPerRound() {

    for (int player = 0; player < numPlayers; ++player) {
        std::shared_ptr<MosaicStorage> playersMosaicStorage = players[player]->getMosaicStorage();
        int playersLostPoints = playersMosaicStorage->getBrokenTiles()->calculatePointsLost();
        int playersPointsThisRound = playersMosaicStorage->getMosaic()->getPointsThisRound();
        int playersEndRoundPoints = (players[player]->getPoints() + playersPointsThisRound) - playersLostPoints;
        if (playersEndRoundPoints < 0) {
            playersEndRoundPoints = 0;
        }
        players[player]->setPoints(playersEndRoundPoints);
        playersMosaicStorage->getMosaic()->resetPoints();
        playersMosaicStorage->endOfRoundDiscardBrokenTiles();
    }

    gec->playerEndOfRoundResult(this->players, this->numPlayers);

}

//terminates the round when there are no more tiles in the factories
bool GameEngine::endOfRoundConditionMet(){
    bool endOfRound = false;
    int counter = 0;
    for(int i = 0; i < numFactories; ++i){
        if(factory[i]->getAllTiles().size() == 0)
            ++counter;
    }
    if(counter == numFactories)
        endOfRound = true;

    return endOfRound;
}

void GameEngine::movePlayerTilesToMosaic(){

    for (int player = 0; player < numPlayers; ++player) {
        players[player]->getMosaicStorage()->endOfRoundMove();
    }

}

void GameEngine::endOfRoundPreparations(){
    movePlayerTilesToMosaic();
    calculatePointsPerRound();

    for (int player = 0; player < numPlayers; ++player) {
        moveTilesToLid(players[player]);
    }

}

bool GameEngine::moveTilesFromFactory(Player* player, unsigned int factoryNumber, unsigned int row, const Type type, const bool toBroken) {
    bool turnSuccess = true;
    if (toBroken)

        //need to take into consideration wanting to move tiles to broken tiles manually
        moveTilesToBrokenTiles(player, factoryNumber, type);
        
    else if (player->getMosaicStorage()->isValidAdd(type, row))
        //player has chosen to put the tiles from the factory somewhere in their mosaic storage
        moveTilesToMosaicStorage(player, factoryNumber, row, type);
        
    else
        //no turns have been taken 
        turnSuccess = false;
    return turnSuccess;
}

void GameEngine::moveTilesToMosaicStorage(Player* player, unsigned const int factoryNumber, unsigned const int row, const Type type) {
    std::vector<std::shared_ptr<Tile>> allTiles = factory[factoryNumber]->getCopiedTilesAndRemove();
    int size = allTiles.size();

    std::vector<std::shared_ptr<Tile>> tilesToMove;

    for(int i = 0; i < size; i++){
        std::shared_ptr<Tile> tileToAdd = allTiles[i];
        if(tileToAdd->getType() == Type::FIRST_PLAYER){
            //automatically move the first player tile to the broken tiles
            moveFirstPlayerTiles(player, tileToAdd, factoryNumber);
        } else if(allTiles[i]->getType() == type) {
            player->getMosaicStorage()->addTile(tileToAdd, row);
        } else{
            // store these tiles into a vector which will be moved to a central factory
            tilesToMove.push_back(tileToAdd);
        }  
    }

    moveToCentral(player, tilesToMove, factoryNumber);

}

void GameEngine::moveFirstPlayerTiles(Player* player, std::shared_ptr<Tile> firstPlayerTile, int centralFactory) {
    player->getMosaicStorage()->getBrokenTiles()->addTile(firstPlayerTile);
    this->setPlayerStartingNextRound(player->getName());

    // then, if there are two central factories, we need to remove the other first player tile from the game
    int factoryToRemove = 1;
    if (numCentral == 2) {
        if (centralFactory == 1) {
            factoryToRemove = 0;
        }

        // Remove the first player tile from the other central factory.
        std::vector<std::shared_ptr<Tile>> allTiles = factory[factoryToRemove]->getCopiedTilesAndRemove();
        for (std::shared_ptr<Tile> tile : allTiles) {
            if (tile->getType() != Type::FIRST_PLAYER) {
                factory[factoryToRemove]->addTile(tile);
            }
        }
    }
}

void GameEngine::moveTilesToBrokenTiles(Player* player, unsigned const int factoryNumber, const Type type){
    int maxBrokenTiles = 7;
    std::vector<std::shared_ptr<Tile>> allTiles = factory[factoryNumber]->getCopiedTilesAndRemove();
    std::shared_ptr<MosaicStorage> mosaicStorage = player->getMosaicStorage();
    BrokenTiles* brokenTiles = mosaicStorage->getBrokenTiles();

    std::vector<std::shared_ptr<Tile>> tilesToMove;
    
    int size = allTiles.size();
    for (int i = 0; i < size; ++i) {
        std::shared_ptr<Tile> tileToAdd = allTiles[i];
        if (allTiles[i]->getType() == type) {
            //make sure that the player can only have a max of 7 tiles; the rest go to the box lid if the max is reached
            if(brokenTiles->getSize() < maxBrokenTiles)
                brokenTiles->addTile(tileToAdd);
            else 
                mosaicStorage->addTileToDiscardedTiles(tileToAdd);
        } else {
            tilesToMove.push_back(tileToAdd);
        }
    }

    moveToCentral(player, tilesToMove, factoryNumber);
}

void GameEngine::moveToCentral(Player* player, std::vector<std::shared_ptr<Tile>> tilesToMove, unsigned const int factoryNumber) {
    int centralFactory = 0;
    if (tilesToMove.size() != 0 && (factoryNumber == 0 || (factoryNumber == 1 && numCentral == 2))) {
        centralFactory = factoryNumber;
    } else if (tilesToMove.size() != 0 && numCentral == 2) {
        bool validInput = false;
        // Error checking to ensure that they select a correct central factory to move to.
        while (!validInput) {
            gec->centralFactoryChoice(tilesToMove);
            std::string usersChoice = "";
            if (!replayMode) {
                // only do this if not replay mode.
                usersChoice = player->chooseCentral();
            } else {
                Input input;
                // if replay mode, then it does it automatically.
                gec->replayCentral(player);
                std::string nextString;
                while (nextString != "next") {
                    gec->replayNext();
                    nextString = input.getString();
                }
                usersChoice = gameLog[replayIndex];
                gec->replayCentralChosen(player, usersChoice);
                ++replayIndex;
            }
            int intChoice = -1;
            if (inputIsInt(usersChoice)) {
                std::stringstream choiceStringStream(usersChoice);
                choiceStringStream >> intChoice;
            }
            if (intChoice == 0 || intChoice == 1) {
                validInput = true;
                centralFactory = intChoice;
                centralChoicePlaceholder = usersChoice;
            }
        }
    }

    // finally, move all of the tiles to the central factory
    for (std::shared_ptr<Tile> tile : tilesToMove) {
        factory[centralFactory]->addTile(tile);
    }
}

//called at the end of each round to get rid of no longer usable tiles
void GameEngine::moveTilesToLid(Player* player){
    std::vector<std::shared_ptr<Tile>> discarded = *(player->getMosaicStorage()->getDiscardedTiles());

    int discardedSize = discarded.size();

    for(int i = 0; i < discardedSize; i++){
        boxLid->addTileToFront(discarded[i]);
        discarded[i] = nullptr;
    }

    player->getMosaicStorage()->resetDiscardTilesVector();
} 

void GameEngine::refillBag() {
    int totalTiles = boxLid->getSize();

    for(int i = 0; i < totalTiles; ++i) {
        bag->addTileToFront(boxLid->getAndRemoveFirstTile());
    }
}

void GameEngine::populateFactories(){
    for (int central = 0; central < numCentral; ++central) {
        factory[central]->addTile(std::make_shared<Tile>(Type::FIRST_PLAYER));
    }
    int startIndex = numCentral;


    //start at startIndex so we don't populate the central factory(s)
    for(int i = startIndex; i < numFactories; i++){
        //fill each factory with 4 tiles
        for(int j = 0; j < 4; ++j){
            if (bag->getSize() > 0) {
                factory[i]->addTile(bag->getAndRemoveFirstTile());
            } else {
                refillBag();
                // in this case, we have run out of tiles in the game, so it must end.
                if (bag->getSize() == 0) {
                    noMoreTiles = true;
                } else {
                    factory[i]->addTile(bag->getAndRemoveFirstTile());
                }
            }
        }
    }
}

void GameEngine::populateBagAndShuffle(){
    //populate array for later shuffling
    const int bagToShuffleSize = 100;
    std::shared_ptr<Tile> bagToShuffle[bagToShuffleSize];
    int bagCount = 0;

    addTilesByColourToBag(Type::BLACK, bagToShuffle, bagCount);
    addTilesByColourToBag(Type::DARK_BLUE, bagToShuffle, bagCount);
    addTilesByColourToBag(Type::LIGHT_BLUE, bagToShuffle, bagCount);
    addTilesByColourToBag(Type::RED, bagToShuffle, bagCount);
    addTilesByColourToBag(Type::YELLOW, bagToShuffle, bagCount);

    shuffle(bagToShuffle, bagToShuffleSize);
    
    //add to linked list format (which is the one used for the rest of the game)
    for(int i = 0; i < bagToShuffleSize; i++){
        bag->addTileToFront(bagToShuffle[i]);
        bagToShuffle[i] = nullptr;
    }
}

//shuffle the bag through a Knuths/Fisher-Yates shuffle algorithm. Can't use seed 0.
void GameEngine::shuffle(std::shared_ptr<Tile> bagToShuffle[] , unsigned const int size){
    std::default_random_engine engine;

    //instantiate the engine with either the given seed or random seed
    if (seed != 0) {
        engine.seed(seed);
    } else {
        seed = std::random_device{}();
        engine.seed(seed);
    }

    for(int i = size - 1; i > 0 ; --i){
        //pick an index so that 0 <= j <= i
        std::uniform_int_distribution<int> uniform_dist(0, i);
        int j = uniform_dist(engine);

        //swap the tiles
        std::shared_ptr<Tile> temp = bagToShuffle[i];
        bagToShuffle[i] = bagToShuffle[j];
        bagToShuffle[j] = temp;
    }
}

//enables 20 tiles of each colour to be added to the tile bag for later shuffling
void GameEngine::addTilesByColourToBag(const Type type, std::shared_ptr<Tile> bagToShuffle[], int &bagCount){
    int valToAssignBagCount = 0;
    int maxTiles = 20;

    for(int i = bagCount; i < (bagCount + maxTiles); ++i){
        valToAssignBagCount = i;
        bagToShuffle[i] = std::make_shared<Tile>(type);
    }

    bagCount = ++valToAssignBagCount;
}

/*
*Interpret the following integers as:
* 0: invalid command
* 1: successful turn
* 2: saved game
* 3: error due to nothing in central factory
* 4: colour not in factory
* 5: invalid moves
*/
std::string GameEngine::interpretPlayerTurn(const int result){
    std::string toReturn;
    if(result == Error_Message::INVALID_COMMAND)
        toReturn = "Error: Invalid Command.\n";
    if(result == Error_Message::SUCCESS)
        toReturn = "Turn Successful.\n";
    if(result == Error_Message::SAVED)
        toReturn = "Saved Game.\n";
    if(result == Error_Message::COLOUR_NOT_IN_FACTORY)
        toReturn = "Error: The colour specified is not in this factory.\n";
    if(result == Error_Message::INVALID_MOVE)
        toReturn = "Error: The move you are trying to make is invalid.\n";

    return toReturn;
}

//loop enables the game to keep playing until someone wins or someone quits
void GameEngine::gameplayLoop(bool& endOfCommands, bool& continueMenuLoop) {
    // new flag for loop. Needed for the new replay functionality.
    bool continueAfterReplay = true;
    Input input;
    while(!endOfCommands && !std::cin.eof() && !winConditionMet() && continueAfterReplay){
        while(!endOfCommands && !endOfRoundConditionMet() && continueAfterReplay){
            
            //output relevant information to players
            gec->boardComponentUpdate(factory, this->numFactories, this->numCentral);
            gec->playerBoardUpdate(players, numPlayers);
            gec->playerTurnUpdate(currentTurn);
            std::string playerCommand = "";
            int turnResult = 0;
            while(!endOfCommands && !std::cin.eof() && (turnResult != 1) && continueAfterReplay){

                // used to check if output to user is required again due to end of replay conflicts
                bool continuedReplayThisRound = false;
                std::string continueString = "";

                int sizeGameLog = gameLog.size();
                // checking for replay mode and if reached the end.
                if (replayMode) {
                    if (replayIndex >= sizeGameLog || gameLog[replayIndex] == "") {
                        // this means that the end of the replay has been reached.
                        
                        while (continueString != "YES" && continueString != "NO") {
                            gec->replayEnd();
                            continueString = input.getString();
                        }

                        // the check for "YES" happens later to stop multiple user input expected from user.
                        if (continueString == "NO") {
                            continueAfterReplay = false;
                        }

                    } else {
                        // run the current move in the game log.
                        std::string nextString = "";
                        while(nextString != "next") {
                            gec->replayNext();
                            nextString = input.getString();
                        }
                        playerCommand = gameLog[replayIndex];
                        gec->replayTurn(getCurrentPlayer(), playerCommand);
                        ++replayIndex;
                    }
                }

                if (continueString == "YES") {
                    replayMode = false;
                    continueAfterReplay = true;
                    continuedReplayThisRound = true;
                    // resetting variable. Is safe to do this since it's impossible for a replay to end during centrail choice decision.
                    centralChoicePlaceholder = "";
                }

                if (!replayMode) {
                    // print information again to user if terminal was interrupted by end of replay
                    if (continuedReplayThisRound) {
                        gec->boardComponentUpdate(factory, this->numFactories, this->numCentral);
                        gec->playerBoardUpdate(players, numPlayers);
                        gec->playerTurnUpdate(currentTurn);
                    }
                    // do a check here if the player is an AI, then it shouldn't take in any input
                    playerCommand = getCurrentPlayer()->chooseTurn();
                } 

                // in case of this being replay mode, don't run this if its not meant to continue
                if (continueAfterReplay) {
                    turnResult = playerTurn(playerCommand);
                    gec->playerTurnResult(interpretPlayerTurn(turnResult));
                }

            }
            // This only runs for io redirection; program automatically exits if the eof is reached
            if(std::cin.eof()){
                gec->playerTurnResult("Program will now exit.");
                endOfCommands = true;
                continueMenuLoop = false;
            }

            swapCurrentTurn();
        }
        // don't do the following if the user doesn't wish to continue after replay. only applicable when replay
        if(!endOfCommands && continueAfterReplay){
            endOfRoundPreparations();
            populateFactories();
            setCurrentTurn(getPlayerStartingNextRound());
            setPlayerStartingNextRound("");
        }
    }

    //loop breaks so we can finalise scores and decide on winner
    if (winConditionMet() || noMoreTiles) {

        if (noMoreTiles) {
            gec->tilesRanOut();
        }

        gec->playerBoardUpdate(players, numPlayers);
        calculateEndGamePoints();

        // When testing, we save the game before it ends to see the end of game save file
        if (testing) {
            GameEngineIO* geIO = new GameEngineIO(this);
            geIO->saveGame("actualoutcome.save");
            delete geIO;
        }

        // choice to save game at the end for replay purposes.
        std::string saveChoice = "";
        while (saveChoice != "YES" && saveChoice != "NO") {
            gec->endOfGameSaveChoice();
            saveChoice = input.getString();
        }

        // if they wish to save, it asks for a file name
        if (saveChoice == "YES") {
            gec->endOfGameSaveName();
            std::string fileName = input.getString();
            GameEngineIO* geIO = new GameEngineIO(this);
            geIO->saveGame(fileName);
            delete geIO;
        }

        resetGame();
    }

    // if the user wishes to stop the replay, it ensures to reset the gameengine afterwards
    if (!continueAfterReplay) {
        resetGame();
    }

}

void GameEngine::calculateEndGamePoints() {

    for (int player = 0; player < numPlayers; ++player) {
        int playersAdditionalPoints = players[player]->getMosaicStorage()->getMosaic()->calculateEndGamePoints();
        players[player]->setPoints(players[player]->getPoints() + playersAdditionalPoints);
    }

    gec->playerEndOfGameResult(this->players, this->numPlayers);
}

void GameEngine::resetGame(){
    // new players are made with each new game instance

    // deleting players
    for (int player = 0; player < numPlayers; ++player) {
        if (players[player] != nullptr) {
            delete players[player];
            players[player] = nullptr;
        }
    } 
    
    //don't delete components as they get instantiated with GE
    for(int i = 0; i < numFactories; i++){
        factory[i]->clear();
    }
    bag->clear();
    boxLid->clear();

    gameLog.clear();
}

