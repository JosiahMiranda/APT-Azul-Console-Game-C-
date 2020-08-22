#include "headers/AI.h"
#include "../GameEngine/GameEngine.h"

AI::AI(std::string name, GameEngine& gameEngine) :
    Player(name), gameEngine(gameEngine), numFactories(gameEngine.getNumFactories())
{}

std::string AI::chooseTurn() {
    return think();
}

std::string AI::chooseCentral() {

    // Chooses the central factory with the least amount of tiles in it. That way, it keeps the
    // factories more balanced, and reduces its own risk of overdrawing broken tiles.
    std::string choice = "0";
    std::vector<std::shared_ptr<Tile>> centralZero = gameEngine.getFactory(0)->getAllTiles();
    std::vector<std::shared_ptr<Tile>> centralOne = gameEngine.getFactory(1)->getAllTiles();

    int centralZeroSize = centralZero.size();
    int centralOneSize = centralOne.size();

    if (centralOneSize < centralZeroSize) {
        choice = "1";
    }

    return choice;

}

std::string AI::think() {

    // fields for decisions and to store the strings that will be returned.
    bool choiceMade = false;
    int factoryNumber = -1;
    int factNumCheck = -1;
    std::string placeToMove;
    std::string tileType;
    std::string choice;

    std::vector<std::shared_ptr<Tile>> factoryTiles[numFactories];
    for (int i = 0; i < numFactories; ++i) {
        factoryTiles[i] = gameEngine.getFactory(i)->getAllTiles();
    }

    // quick iteration through types in the loops for the methods by making vector of types.
    std::vector<Type> types;
    types.push_back(Type::RED);
    types.push_back(Type::BLACK);
    types.push_back(Type::LIGHT_BLUE);
    types.push_back(Type::DARK_BLUE);
    types.push_back(Type::YELLOW);

    // AI tries to find a perfect fill - move from factory to storage rows that will fill the row.
    findPerfectFill(factoryTiles, choiceMade, factNumCheck, placeToMove, tileType, factoryNumber);
    if (!choiceMade) {
        // If it couldn't do the prior move, it then tries to fill a semi-filled storage row.
        findSemiFill(factoryTiles, choiceMade, factNumCheck, placeToMove, tileType, factoryNumber);
    }
    if (!choiceMade) {
        /* 
         * If it again couldn't do the prior move, the AI checks all possible combinations of tiles to its current storage
         * and finds the move that will lead to it obtaining the most sequential points.
         */ 
        findMostSequentialPoints(factoryTiles, choiceMade, factNumCheck, placeToMove, tileType, factoryNumber, types);
    }
    if (!choiceMade) {
        // This is a fail safe check, simply just to make any valid move it can possibly make.
        findAnyValidMove(factoryTiles, choiceMade, factNumCheck, placeToMove, tileType, factoryNumber);
    }
    if (!choiceMade) {
        /* 
         * If it reached here, no valid move could be made. AI is forced to move to broken tiles. It moves the smallest number
         * of tiles possible to broken tiles, to minimise point reduction.
         */ 
        moveBroken(factoryTiles, choiceMade, factNumCheck, placeToMove, tileType, factoryNumber, types);
    }

    choice = "turn " + std::to_string(factoryNumber) + " " + tileType + " " + placeToMove;
    
    std::cout << choice << std::endl;
    return choice;
}

void AI::findPerfectFill(std::vector<std::shared_ptr<Tile>>* factoryTiles, bool& choiceMade, int& factNumCheck,
                            std::string& placeToMove, std::string& tileType, int& factoryNumber) {
    for (int storageRow = 0; storageRow < MAX_ROWS; ++storageRow) {
        for (int factory = 0; factory < numFactories; ++factory) {
            int factorySize = factoryTiles[factory].size();
            for (int tile = 0; tile < factorySize; ++tile) {
                if (factoryTiles[factory].at(tile) != nullptr && factoryTiles[factory].at(tile)->getType() != Type::FIRST_PLAYER) {
                    if (isPossible(factoryTiles[factory].at(tile)->getType(), storageRow, factoryTiles, factNumCheck, true)) {
                        choiceMade = true;
                        // because the actual row name is + 1, since rows start at index 0
                        placeToMove = std::to_string(storageRow + 1);
                        tileType = factoryTiles[factory].at(tile)->getColourType();
                        factoryNumber = factNumCheck;
                    }
                }
            }
        }
    }
}

void AI::findSemiFill(std::vector<std::shared_ptr<Tile>>* factoryTiles, bool& choiceMade, int& factNumCheck,
                            std::string& placeToMove, std::string& tileType, int& factoryNumber) {
    for (int storageRow = 0; storageRow < MAX_ROWS && !choiceMade; ++storageRow) {
        std::shared_ptr<Tile>* rowTiles = getMosaicStorage()->getRow(storageRow);
        int sizeOfRowStorage = storageRow + 1;
        for (int tile = 0; tile < sizeOfRowStorage && !choiceMade; ++tile) {
            if (rowTiles[tile] != nullptr) {
                // check if this is possible
                if (isPossible(rowTiles[tile]->getType(), storageRow, factoryTiles, factNumCheck, false)) {
                    choiceMade = true;
                    // because the actual row name is + 1, since rows start at index 0
                    placeToMove = std::to_string(storageRow + 1);
                    tileType = rowTiles[tile]->getColourType();
                    factoryNumber = factNumCheck;
                }
            }
        }
    }
}

void AI::findMostSequentialPoints(std::vector<std::shared_ptr<Tile>>* factoryTiles, bool& choiceMade, int& factNumCheck,
                            std::string& placeToMove, std::string& tileType, int& factoryNumber, std::vector<Type> types) {
    // scan the mosaic
    std::shared_ptr<Tile>** mosaic = getMosaicStorage()->getMosaic()->getGrid();

    int maxPoints = 0;

    for (int row = 0; row < MAX_ROWS; ++row) {
        for (int col = 0; col < MAX_COLS; ++col) {
            // Find an empty spot and see how many points we can get from filling that place out.
            if (mosaic[row][col] == nullptr) {
                Type type = Type::NONE;

                for (Type iterType : types) {
                    int pointsForThisType = 1;
                    if (isPossible(iterType, row, factoryTiles, factNumCheck, false)) {
                        
                        // if its possible, count how many points can be retrieved from filling it out
                        pointsForThisType += numSequentialTiles(row, col, mosaic);
                        pointsForThisType -= pointsToLoseFromBroken(iterType, row, factoryTiles[factNumCheck]);

                        // Choose the move that will rack up the most points.
                        if (pointsForThisType > maxPoints) {
                            
                            placeToMove = std::to_string(row + 1);
                            maxPoints = pointsForThisType;
                            type = iterType;
                            tileType = toColourCharacter(type);
                            factoryNumber = factNumCheck;

                        }
                    }
                }

                // if the type was changed then there was a valid choice.
                if (type != Type::NONE) {
                    choiceMade = true;
                }
            }
        }
    }
}

void AI::findAnyValidMove(std::vector<std::shared_ptr<Tile>>* factoryTiles, bool& choiceMade, int& factNumCheck,
                            std::string& placeToMove, std::string& tileType, int& factoryNumber) {
    for (int factory = 0; factory < numFactories; ++factory) {
        int size = factoryTiles[factory].size();
        for (int tile = 0; tile < size; ++tile) {
            if (factoryTiles[factory].at(tile) != nullptr && factoryTiles[factory].at(tile)->getType() != Type::FIRST_PLAYER) {
                for (int storageRow = 0; storageRow < MAX_ROWS; ++storageRow) {
                    if (getMosaicStorage()->isValidAdd(factoryTiles[factory].at(tile)->getType(), storageRow)) {
                        factoryNumber = factory;
                        tileType = factoryTiles[factory].at(tile)->getColourType();
                        placeToMove = std::to_string(storageRow + 1);
                        choiceMade = true;
                    }
                }
            }
        }
    }
}

void AI::moveBroken(std::vector<std::shared_ptr<Tile>>* factoryTiles, bool& choiceMade, int& factNumCheck,
                            std::string& placeToMove, std::string& tileType, int& factoryNumber, std::vector<Type> types) {
    int minNumTiles = INT32_MAX;

    for (Type type : types) {
        for (int factory = 0; factory < numFactories; ++factory) {
            int numType = 0;
            int size = factoryTiles[factory].size();
            for (int tile = 0; tile < size; ++tile) {
                if (factoryTiles[factory].at(tile) != nullptr && factoryTiles[factory].at(tile)->getType() == type) {
                    ++numType;
                }
            }
            if (numType != 0 && numType <= minNumTiles) {
                factoryNumber = factory;
                minNumTiles = numType;
                tileType = toColourCharacter(type);
                placeToMove = "B";
            }
        }
    }
}

bool AI::isPossible(Type type, int storageRow, std::vector<std::shared_ptr<Tile>>* factoryTiles, int& factNumCheck, bool perfectFill) {

    int numToFill = 0;
    int numEmptyFactories = 0;
    bool equal = false;
    bool possible = true;
    int sizeOfStorageRow = storageRow + 1;
    bool returnBool = false;

    // if it's not a valid add to the row then automatically it's not possible
    if (!getMosaicStorage()->isValidAdd(type, storageRow)) {
        possible = false;
    }


    std::shared_ptr<Tile>* rowTiles = getMosaicStorage()->getRow(storageRow);

    for (int tile = 0; tile < sizeOfStorageRow; ++tile) {
        if (rowTiles[tile] == nullptr) {
            ++numToFill;
        }
    }

    // it's not a possible move if the storage row is already full, meaning there's nothing to fill
    if (numToFill == 0) {
        possible = false;
    }

    if (possible) {
        int factoryDiff[numFactories];

        for (int factory = 0; factory < numFactories; ++factory) {
            int numOfType = 0;
            
            int size = factoryTiles[factory].size();
            for (int tile = 0; tile < size; ++tile) {
                if (factoryTiles[factory].at(tile) != nullptr) {
                    if (factoryTiles[factory].at(tile)->getType() == type) {
                        // if there does exist a tile of that type 
                        ++numOfType;
                    }
                }
            }

            if (numOfType == numToFill) {
                factNumCheck = factory;
                equal = true;
            } else if (numOfType == 0) {
                ++numEmptyFactories;
                factoryDiff[factory] = 0;
            } 

            if (numOfType != 0) {
                factoryDiff[factory] = abs(numToFill - numOfType);
            }
            // store absolute difference
            

        }
        // If all of the factories are empty of this type then this is not possible
        if (numEmptyFactories == numFactories) {
            possible = false;
        } else if (!equal) {
            // then use the most prioritized factory.

            int i = -1;
            int minDifference = -1;

            // properly initialising i and mindifference to be used next.
            for (int factory = 0; factory < numFactories; ++factory) {
                if (factoryDiff[factory] != 0) {
                    minDifference = factoryDiff[factory];
                    i = factory;
                }
            }

            if (minDifference != -1) {
                for (int factory = 0; factory < numFactories; ++factory) {
                    if (factoryDiff[factory] != 0 && factoryDiff[factory] < minDifference) {
                        i = factory;
                    }
                }
            }

            // if this is still -1, then it wasn't possible.
            if (i == -1) {
                possible = false;
            } else {
                factNumCheck = i;
            }
        }

    }

    // if looking for perfect fill, then the return should be true only if the move can be done perfectly

    if (perfectFill) {
        returnBool = equal;
    } else {
        returnBool = possible;
    }

    return returnBool;
}

int AI::numSequentialTiles(int row, int col, std::shared_ptr<Tile>** mosaic) {
    bool sequential = true;
    int pointsForThisType = 0;

    // Check all values before the row
    for (int i = row-1; i >= 0; --i) {
        if (sequential && mosaic[i][col] != nullptr) {
            ++pointsForThisType;
        } else {
            sequential = false;
        }
    }

    sequential =  true;

    // Check all values after the row
    for (int i = row+1; i < MAX_ROWS; ++i) {
        if (sequential && mosaic[i][col] != nullptr) {
            ++pointsForThisType;
        } else {
            sequential = false;
        }
    }

    sequential = true;

    // Check all values before the col
    for (int i = col-1; i >= 0; --i) {
        if (sequential && mosaic[row][i] != nullptr) {
            ++pointsForThisType;
        } else {
            sequential = false;
        }
    }

    sequential = true;

    // Check all values after the col
    for (int i = col+1; i < MAX_COLS; ++i) { 
        if (sequential && mosaic[row][i] != nullptr) {
            ++pointsForThisType;
        } else {
            sequential = false;
        }
    }

    return pointsForThisType;

}

int AI::pointsToLoseFromBroken(Type type, int storageRow, std::vector<std::shared_ptr<Tile>> factoryTiles) {

    int numEmpty = 0;
    int sizeOfStorageRow = storageRow + 1;

    std::shared_ptr<Tile>* rowTiles = getMosaicStorage()->getRow(storageRow);
    for (int row = 0; row < sizeOfStorageRow; ++row) {
        if (rowTiles[row] == nullptr) {
            ++numEmpty;
        }
    }

    int factorySize = factoryTiles.size();
    int numOfType = 0;
    for (int tile = 0; tile < factorySize; ++tile) {
        if (factoryTiles.at(tile)->getType() == type) {
            ++numOfType;
        }
    }

    int numBrokenTiles = numOfType - numEmpty;
    int lostPoints = 0;

    if (numBrokenTiles == 1) {
        lostPoints = 1;
    } else if (numBrokenTiles == 2) {
        lostPoints = 2;
    } else if (numBrokenTiles == 3) {
        lostPoints = 4;
    } else if (numBrokenTiles == 4) {
        lostPoints = 6;
    } else if (numBrokenTiles == 5) {
        lostPoints = 8;
    } else if (numBrokenTiles == 6) {
        lostPoints = 11;
    } else if (numBrokenTiles >= 7) {
        lostPoints = 14;
    }

    return lostPoints;
}

char AI::toColourCharacter(Type type) {
    char tileChar = 'X';
    if(type == Type::RED) {
        tileChar = 'R';
    }
    if(type == Type::YELLOW) {
        tileChar = 'Y';
    }
    if(type == Type::DARK_BLUE) {
        tileChar = 'B';
    }
    if(type == Type::LIGHT_BLUE) {
        tileChar = 'L';
    }
    if(type == Type::BLACK) {
        tileChar = 'U';
    }
    if(type == Type::FIRST_PLAYER) {
        tileChar = 'F';
    }

    return tileChar;
}