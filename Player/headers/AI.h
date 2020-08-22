#ifndef AI_H
#define AI_H

#include "Player.h"

class GameEngine;
class AI : public Player {
public:

    AI(std::string name, GameEngine& gameEngine);
    // Overrides getInput method so it does it differently as AI does not need input
    virtual std::string chooseTurn();
    virtual std::string chooseCentral();


private:

    GameEngine& gameEngine;

    // Method that chooses the move the AI will make.
    std::string think();

    // Checks if the move is possible. Final flag is if checking for a perfect placement.
    bool isPossible(Type type, int storageRow, std::vector<std::shared_ptr<Tile>>* factoryTiles, int& factNumCheck, bool perfectFill);

    // Method checks the number of sequential tiles.
    int numSequentialTiles(int row, int col, std::shared_ptr<Tile>** mosaic);

    // Converting a type to the character for the type.
    char toColourCharacter(Type type);

    // Calculating the number of points lost from broken tiles.
    int pointsToLoseFromBroken(Type type, int storageRow, std::vector<std::shared_ptr<Tile>> factoryTiles);

    // method for trying to find a perfect fill from a factory to a mosaic storage row
    void findPerfectFill(std::vector<std::shared_ptr<Tile>>* factoryTiles, bool& choiceMade, int& factNumCheck,
                            std::string& placeToMove, std::string& tileType, int& factoryNumber);

    // method for trying to fill a storage row that is already semi-filled
    void findSemiFill(std::vector<std::shared_ptr<Tile>>* factoryTiles, bool& choiceMade, int& factNumCheck,
                            std::string& placeToMove, std::string& tileType, int& factoryNumber);

    // method for trying to find the tile-row combination that will provide the most sequential points
    void findMostSequentialPoints(std::vector<std::shared_ptr<Tile>>* factoryTiles, bool& choiceMade, int& factNumCheck,
                            std::string& placeToMove, std::string& tileType, int& factoryNumber, std::vector<Type> types);

    // method for trying to find any valid move it could possibly do
    void findAnyValidMove(std::vector<std::shared_ptr<Tile>>* factoryTiles, bool& choiceMade, int& factNumCheck,
                            std::string& placeToMove, std::string& tileType, int& factoryNumber);

    // method for when no valid move can be made.
    void moveBroken(std::vector<std::shared_ptr<Tile>>* factoryTiles, bool& choiceMade, int& factNumCheck,
                            std::string& placeToMove, std::string& tileType, int& factoryNumber, std::vector<Type> types);

    const int numFactories;

};

#endif // AI_H