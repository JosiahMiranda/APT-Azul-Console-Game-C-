#ifndef MOSAIC_H
#define MOSAIC_H

#include "../../Components/headers/Tile.h"

#define MAX_ROWS        5
#define MAX_COLS        5
#define MAX_NO_TILES    5

// #define RED         0
// #define YELLOW      1
// #define DARK_BLUE   2
// #define LIGHT_BLUE  3
// #define BLACK       4

class Mosaic{
public:
    Mosaic();
    ~Mosaic();

    //Getters
    std::shared_ptr<Tile>** getGrid();
    int getPointsThisRound();
    int getColourColumn(unsigned const int row, unsigned const int colour) const;

    //Tile movers
    bool addTile(const std::shared_ptr<Tile> tile, unsigned int row, unsigned int col);
    void incrementColorCounter(const Type tileType);

    //Point calculators
    int calculateEndGamePoints();
    void numberOfSequentialTiles(unsigned const int row, unsigned const int col);
    bool checkSequentialRows(unsigned const int row, unsigned const int col);
    bool checkSequentialCols(unsigned const int row, unsigned const int col);

    //Utilities
    bool findFullRow() const;
    bool findFullCol(unsigned const int col) const;
    bool isSpaceFree(unsigned const int row, unsigned const int col);
    unsigned int numFullRows() const;
    unsigned int numFullCols() const;
    unsigned int noOfFiveColours() const;
    void resetPoints();

    //Printers
    std::string templateRowToString(int index) const;
    std::string rowToString(int index) const;
    std::string rowToSave(int index) const;
   
private:
    std::shared_ptr<Tile>** grid;
    unsigned int noOfReds;
    unsigned int noOfYellows;
    unsigned int noOfDarkBlues;
    unsigned int noOfLightBlues;
    unsigned int noOfBlacks;
    unsigned int pointsThisRound;

    /* Represents the column in which the color belongs to
     * Col 1 = Red
     * Col 2 = Yellow
     * Col 3 = Dark Blue
     * Col 4 = Light Blue
     * Col 5 = Black
     */
    const int colourColumns[MAX_ROWS][MAX_COLS] = {
        {2, 1, 0, 4, 3} ,
        {3, 2, 1, 0, 4} ,
        {4, 3, 2, 1, 0} ,
        {0, 4, 3, 2, 1} ,
        {1, 0, 4, 3, 2} ,
    };

    const char colourGrid[MAX_ROWS][MAX_COLS] = {
        {'B', 'Y', 'R', 'U', 'L'} ,
        {'L', 'B', 'Y', 'R', 'U'} ,
        {'U', 'L', 'B', 'Y', 'R'} ,
        {'R', 'U', 'L', 'B', 'Y'} ,
        {'Y', 'R', 'U', 'L', 'B'} ,
    };

    const char toSavecolourGrid[MAX_ROWS][MAX_COLS] = {
        {'b', 'y', 'r', 'u', 'l'} ,
        {'l', 'b', 'y', 'r', 'u'} ,
        {'u', 'l', 'b', 'y', 'r'} ,
        {'r', 'u', 'l', 'b', 'y'} ,
        {'y', 'r', 'u', 'l', 'b'} ,
    };
};

#endif // MOSAIC_H