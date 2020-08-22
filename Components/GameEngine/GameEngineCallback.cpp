#include "GameEngineCallback.h"

GameEngineCallback::GameEngineCallback() {

}

GameEngineCallback::~GameEngineCallback() {

}

void GameEngineCallback::playerBoardUpdate(Player** players, int numPlayers) const {

    if (numPlayers == 2) {

        for (int player = 0; player < numPlayers; ++player) {
            std::string outputString;
            std::shared_ptr<MosaicStorage> mStorage = players[player]->getMosaicStorage();
            Mosaic* mosaic = mStorage->getMosaic();

            outputString = "Mosaic for " + formatString(players[player]->getName(), 11) + "\t\t" + "TEMPLATE" + "\n"
                + mStorage->rowToString(0) + mosaic->rowToString(0) + "\t" + mosaic->templateRowToString(0) + "\n"
                + mStorage->rowToString(1) + mosaic->rowToString(1) + "\t" + mosaic->templateRowToString(1) + "\n"
                + mStorage->rowToString(2) + mosaic->rowToString(2) + "\t" + mosaic->templateRowToString(2) + "\n"
                + mStorage->rowToString(3) + mosaic->rowToString(3) + "\t" + mosaic->templateRowToString(3) + "\n"
                + mStorage->rowToString(4) + mosaic->rowToString(4) + "\t" + mosaic->templateRowToString(4) + "\n"
                + mStorage->getBrokenTiles()->toString() + "\n";

            std::cout << outputString << std::endl;
        }   

    } else if (numPlayers == 3) {
        
        std::string outputString;
        std::shared_ptr<MosaicStorage> mStorage = players[0]->getMosaicStorage();
        std::shared_ptr<MosaicStorage> mStorage3 = players[2]->getMosaicStorage();
        Mosaic* mosaic = mStorage->getMosaic();
        Mosaic* mosaic3 = mStorage3->getMosaic();

        outputString = "Mosaic for " + formatString(players[0]->getName(), 11) + "\t\t" + "Mosaic for " +
                formatString(players[2]->getName(), 11) + "\t\t" + "TEMPLATE" + "\n"
            + mStorage->rowToString(0) + mosaic->rowToString(0) + "\t" + mStorage3->rowToString(0) +
                mosaic3->rowToString(0) + "\t" + mosaic->templateRowToString(0) + "\n"
            + mStorage->rowToString(1) + mosaic->rowToString(1) + "\t" + mStorage3->rowToString(1) +
                mosaic3->rowToString(1) + "\t" + mosaic->templateRowToString(1) + "\n"
            + mStorage->rowToString(2) + mosaic->rowToString(2) + "\t" + mStorage3->rowToString(2) +
                mosaic3->rowToString(2) + "\t" + mosaic->templateRowToString(2) + "\n"
            + mStorage->rowToString(3) + mosaic->rowToString(3) + "\t" + mStorage3->rowToString(3) +
                mosaic3->rowToString(3) + "\t" + mosaic->templateRowToString(3) + "\n"
            + mStorage->rowToString(4) + mosaic->rowToString(4) + "\t" + mStorage3->rowToString(4) +
                mosaic3->rowToString(4) + "\t" + mosaic->templateRowToString(4) + "\n"
            + formatString(mStorage->getBrokenTiles()->toString(), 20) + "\t\t" + mStorage3->getBrokenTiles()->toString() + "\n";

        std::cout << outputString << std::endl;

        std::string outputString2;
        std::shared_ptr<MosaicStorage> mStorage2 = players[1]->getMosaicStorage();
        Mosaic* mosaic2 = mStorage2->getMosaic();

        outputString2 = "Mosaic for " + formatString(players[1]->getName(), 11) + "\t\t" + "TEMPLATE" + "\n"
            + mStorage2->rowToString(0) + mosaic2->rowToString(0) + "\t" + mosaic->templateRowToString(0) + "\n"
            + mStorage2->rowToString(1) + mosaic2->rowToString(1) + "\t" + mosaic->templateRowToString(1) + "\n"
            + mStorage2->rowToString(2) + mosaic2->rowToString(2) + "\t" + mosaic->templateRowToString(2) + "\n"
            + mStorage2->rowToString(3) + mosaic2->rowToString(3) + "\t" + mosaic->templateRowToString(3) + "\n"
            + mStorage2->rowToString(4) + mosaic2->rowToString(4) + "\t" + mosaic->templateRowToString(4) + "\n"
            + mStorage2->getBrokenTiles()->toString() + "\n";

        std::cout << outputString2 << std::endl;


    } else if (numPlayers == 4) {

        for (int player = 0; player < 2; ++player) {
            std::string outputString;
            std::shared_ptr<MosaicStorage> mStorage = players[player]->getMosaicStorage();
            std::shared_ptr<MosaicStorage> mStorage2 = players[player+2]->getMosaicStorage();
            Mosaic* mosaic = mStorage->getMosaic();
            Mosaic* mosaic2 = mStorage2->getMosaic();

            outputString = "Mosaic for " + formatString(players[player]->getName(), 11) + "\t\t" + "Mosaic for " +
                    formatString(players[player+2]->getName(), 11) + "\t\t" + "TEMPLATE" + "\n"
                + mStorage->rowToString(0) + mosaic->rowToString(0) + "\t" + mStorage2->rowToString(0) +
                    mosaic2->rowToString(0) + "\t" + mosaic->templateRowToString(0) + "\n"
                + mStorage->rowToString(1) + mosaic->rowToString(1) + "\t" + mStorage2->rowToString(1) +
                    mosaic2->rowToString(1) + "\t" + mosaic->templateRowToString(1) + "\n"
                + mStorage->rowToString(2) + mosaic->rowToString(2) + "\t" + mStorage2->rowToString(2) +
                    mosaic2->rowToString(2) + "\t" + mosaic->templateRowToString(2) + "\n"
                + mStorage->rowToString(3) + mosaic->rowToString(3) + "\t" + mStorage2->rowToString(3) +
                    mosaic2->rowToString(3) + "\t" + mosaic->templateRowToString(3) + "\n"
                + mStorage->rowToString(4) + mosaic->rowToString(4) + "\t" + mStorage2->rowToString(4) +
                    mosaic2->rowToString(4) + "\t" + mosaic->templateRowToString(4) + "\n"
                + formatString(mStorage->getBrokenTiles()->toString(), 20) + "\t\t" + mStorage2->getBrokenTiles()->toString() + "\n";

            std::cout << outputString << std::endl;
        }
    }
}

std::string GameEngineCallback::formatString(std::string playerName, int numSpaces) const {
    std::string toReturn = playerName;
    int size = playerName.length();
    int spacesLeft = numSpaces-size;
    for (int i = spacesLeft; i >= 0; --i) {
        toReturn += " ";
    }

    return toReturn;
}

void GameEngineCallback::playerTurnUpdate(const std::string playerName) const {
    std::cout<< "It is now " << playerName <<"'s turn." << std::endl;
}

void GameEngineCallback::centralFactoryChoice(std::vector<std::shared_ptr<Tile>> tiles) const {
    std::string output = "Which Central Factory to move Tiles: [ ";
    for (std::shared_ptr<Tile> tile : tiles) {
        output += tile->getColourType();
        output += " ";
    }
    output += "] to? (0 or 1)";

    std::cout << output << std::endl;
}

void GameEngineCallback::playerTurnResult(const std::string turnResult) const {
    std::cout << turnResult << std::endl;
}

void GameEngineCallback::playerEndOfRoundResult(Player** players, int numPlayers) const {

    for (int player = 0; player < numPlayers; ++player) {
        std::cout << players[player]->getName() << " Points: " << players[player]->getPoints() << std::endl;
    }
    std::cout<< "\n=====End of round - Beginning next round!=====\n" << std::endl;
}

void GameEngineCallback::playerEndOfGameResult(Player** players, int numPlayers) const {

    for (int player = 0; player < numPlayers; ++player) {
        std::cout << players[player]->getName() << " Points: " << players[player]->getPoints() << std::endl;
    }

    std::cout<< "\n=================End of Game=================" << std::endl << std::endl;

    int mostPoints = 0;
    for (int player = 0; player < numPlayers; ++player) {
        if (players[player]->getPoints() > mostPoints) {
            mostPoints = players[player]->getPoints();
        }
    }

    std::vector<Player*> winners;
    for (int player = 0; player < numPlayers; ++player) {
        if (players[player]->getPoints() == mostPoints) {
            winners.push_back(players[player]);
        }
    }

    std::string output = "";
    if (winners.size() == 1) {
        output = winners.at(0)->getName() + " is the winner!";
    } else {
        output = winners.at(0)->getName();
        int size = winners.size();
        for (int player = 1; player < size; ++player) {
            output+= " and " + winners.at(player)->getName();
        }
        output += " are the winners!";
    }

    std::cout << output << std::endl << std::endl;
}

void GameEngineCallback::boardComponentUpdate(Factory** factory, int numFactories, int numCentral) const {
    std::cout<< "--------------------Next Turn------------------\n" << std::endl;
    std::vector<std::shared_ptr<Tile>> tilesOfCenter = factory[0]->getAllTiles();
    std::string outputString = "";

    outputString += "Factory 0: ";
    int size = tilesOfCenter.size();
    for(int j = 0; j < size; ++j){
        outputString.push_back(tilesOfCenter[j]->getColourType());
        outputString += " ";
    }
    outputString += "(CENTRAL)\n";

    for(int i = 1; i < numFactories; i++){
        std::vector<std::shared_ptr<Tile>> tiles = factory[i]->getAllTiles();
        outputString += "Factory " + std::to_string(i) += ": ";
        int size = tiles.size();
        for(int j = 0; j < size; ++j){
            outputString.push_back(tiles[j]->getColourType());
            outputString += " ";
        }
        if (numCentral == 2 && i == 1) {
            outputString += "(CENTRAL)";
        }
        outputString += "\n";
    }

    std::cout << outputString << std::endl;
}

void GameEngineCallback::tilesRanOut() const {
    std::cout << ">>                            <<" << std::endl;
    std::cout << "     Game ran out of tiles      " << std::endl;
    std::cout << "         Game Ending            " << std::endl;
    std::cout << ">>                            <<" << std::endl << std::endl;
}


void GameEngineCallback::replayTurn(Player* player, std::string playerCommand) const {
    std::cout << "\n ===== Player " << player->getName() << " did action " << playerCommand << " =====\n" << std::endl; 
}

void GameEngineCallback::replayCentral(Player* player) const {
    std::cout << "\n ===== Player " << player->getName() << " needs to choose a central factory to move the tiles to." << " =====\n" << std::endl; 
}

void GameEngineCallback::replayCentralChosen(Player* player, std::string usersChoice) const {
    std::cout << "\n ===== Player " << player->getName() << " moved tiles to Central " << usersChoice << " =====\n" << std::endl; 
}

void GameEngineCallback::replayNext() const {
    std::cout << "Type 'next' for next action." << std::endl;
}

void GameEngineCallback::replayEnd() const {
    std::cout << "\n =====  End of replay reached, but game isn't finished. " << " =====" << std::endl;
    std::cout << " =====       Would you like to continue the game?       " << " =====" << std::endl;
    std::cout << " ===== Type 'YES' to continue or 'NO' to return to menu." << " =====\n" << std::endl;
}

void GameEngineCallback::endOfGameSaveChoice() const {
    std::cout << "\n=== Game has ended! Do you wish to save the file for replay purposes? (YES or NO) ===\n" << std::endl;
}

void GameEngineCallback::endOfGameSaveName() const {
    std::cout << "Please enter file name to save as." << std::endl;
}