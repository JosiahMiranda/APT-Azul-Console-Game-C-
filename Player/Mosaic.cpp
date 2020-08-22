#include "headers/Mosaic.h"

#include <iostream>

Mosaic::Mosaic(){
    // Making 5x5 2d array for tiles
    this->grid = new std::shared_ptr<Tile>*[MAX_ROWS];
    for (unsigned int row = 0; row < MAX_ROWS; ++row) {
        grid[row] = new std::shared_ptr<Tile>[MAX_COLS];
    }

    this->noOfBlacks = 0;
    this->noOfYellows = 0;
    this->noOfDarkBlues = 0;
    this->noOfLightBlues = 0;
    this->noOfReds = 0;
    this->pointsThisRound = 0;

}

Mosaic::~Mosaic(){
    // delete tiles
    for (unsigned int row = 0; row < MAX_ROWS; ++row) {
        for (unsigned int col = 0; col < MAX_COLS; ++col) {
            if(grid[row][col] != nullptr){
                grid[row][col] = nullptr;
            }
        }
        delete[] grid[row];
        grid[row] = nullptr;
    }

    delete[] this->grid;
    this->grid = nullptr;
    this->pointsThisRound = 0;
}

std::shared_ptr<Tile>** Mosaic::getGrid(){
    return this->grid;
}

bool Mosaic::findFullRow() const {
    bool fullRowFound = false;
    unsigned int counter = 0;

    for (unsigned int row = 0; row < MAX_ROWS; ++row) {
        for (unsigned int col = 0; col < MAX_COLS; ++col) {
            if(grid[row][col] != nullptr){
                ++counter;
                if(counter == MAX_NO_TILES){
                    fullRowFound = true;
                }
            }
        }
        counter = 0;
    }

    return fullRowFound;
}

bool Mosaic::findFullCol(unsigned const int col) const {
    bool colIsFull = false;
    unsigned int counter = 0;

    for (unsigned int row = 0; row < MAX_ROWS; ++row) {
        if(grid[row][col] != nullptr){
            ++counter;
            if(counter == MAX_NO_TILES){
                colIsFull = true;
            }
        }
    }

    return colIsFull;
}

unsigned int Mosaic::noOfFiveColours() const {
    unsigned int timesGotAll5Colours = 0;

    if(this->noOfBlacks == MAX_NO_TILES)
        ++timesGotAll5Colours;
    if(this->noOfReds == MAX_NO_TILES)
        ++timesGotAll5Colours;
    if(this->noOfDarkBlues == MAX_NO_TILES)
        ++timesGotAll5Colours;
    if(this->noOfLightBlues == MAX_NO_TILES)
        ++timesGotAll5Colours;
    if(this->noOfYellows == MAX_NO_TILES)
        ++timesGotAll5Colours;

    return timesGotAll5Colours;
}

void Mosaic::numberOfSequentialTiles(unsigned int row, unsigned int col){
    int numSequential = 0;
    if (checkSequentialCols(row, col)) {
        ++numSequential;
    }
    if (checkSequentialRows(row,col)) {
        ++numSequential;
    }

    if (numSequential == 2) {
        pointsThisRound += 2;
    } else {
        pointsThisRound += 1;
    }
}

bool Mosaic::checkSequentialRows(unsigned const int row, unsigned const int col){
    bool hasSequential = false;
    unsigned int numberOfSequentialTiles = 0;
    bool sequential = true;

    // Check all values before the row
    for (int i = row-1; i >= 0; --i) {
        if (sequential && grid[i][col] != nullptr) {
            hasSequential = true;
            ++numberOfSequentialTiles;
        } else {
            sequential = false;
        }
    }

    sequential =  true;

    // Check all values after the row
    for (int i = row+1; i < MAX_ROWS; ++i) {
        if (sequential && grid[i][col] != nullptr) {
            hasSequential = true;
            ++numberOfSequentialTiles;
        } else {
            sequential = false;
        }
    }

    pointsThisRound += numberOfSequentialTiles;

    return hasSequential;

}

bool Mosaic::checkSequentialCols(unsigned const int row, unsigned const int col){
    bool hasSequential = false;
    unsigned int numberOfSequentialTiles = 0;
    bool sequential = true;
    // Check all values before the row
    for (int i = col-1; i >= 0; --i) {
        if (sequential && grid[row][i] != nullptr) {
            hasSequential = true;
            ++numberOfSequentialTiles;
        } else {
            sequential = false;
        }
    }

    sequential = true;

    // Check all values after the row
    for (int i = col+1; i < MAX_COLS; ++i) { 
        if (sequential && grid[row][i] != nullptr) {
            hasSequential = true;
            ++numberOfSequentialTiles;
        } else {
            sequential = false;
        }
    }

    pointsThisRound += numberOfSequentialTiles;

    return hasSequential;
}

int Mosaic::getPointsThisRound(){
    return this->pointsThisRound;
}

void Mosaic::resetPoints(){
    this->pointsThisRound = 0;
}

bool Mosaic::isSpaceFree(unsigned int row, unsigned int col){
    bool valid = false;
    if(this->grid[row][col] == nullptr){
        valid = true;
    }
    return valid;
}

bool Mosaic::addTile(std::shared_ptr<Tile> tile, unsigned int row, unsigned int col){
    bool added = false;
    if(col >= 0 && col < MAX_COLS && row >=0 && row < MAX_ROWS){
        if(grid[row][col] == nullptr){
            this->grid[row][col] = tile;
            numberOfSequentialTiles(row,col);
            incrementColorCounter(tile->getType());
            added = true;
        }
    }

    return added;
}

void Mosaic::incrementColorCounter(Type tileType){
     if(tileType == Type::RED)
        this->noOfReds++;
    if(tileType == Type::BLACK)
        this->noOfBlacks++;
    if(tileType == Type::DARK_BLUE)
        this->noOfDarkBlues++;
    if(tileType == Type::LIGHT_BLUE)
        this->noOfLightBlues++;
    if(tileType == Type::YELLOW)
        this->noOfYellows++;
}

int Mosaic::getColourColumn(unsigned const int row, unsigned const int colour) const {
    int toReturn = -1;
    if (row >= 0 && row < MAX_ROWS && colour >= 0 && colour < MAX_COLS) {
        toReturn = colourColumns[row][colour];
    } 
    return toReturn;
}

std::string Mosaic::rowToString(int index) const{
    std::string string = "|| ";

    for(int i = 0; i< MAX_COLS; ++i){
        if(this->grid[index][i] != nullptr){
            string += grid[index][i]->getColourType();
            string += " ";
        }else{
            string += ". ";
        }
       
    }

    return string;
}

std::string Mosaic::templateRowToString(int index) const {
    std::string string = "";

    for(int i = 0; i< MAX_COLS; ++i){
        string += colourGrid[index][i];
        string += " ";
       
    }

    return string;
}

std::string Mosaic::rowToSave(int index) const {
    std::string string;
    for(int i = 0; i< MAX_COLS; ++i){
        if(this->grid[index][i] != nullptr){
            string += grid[index][i]->getColourType();
            string += " ";
        }else{
            string += toSavecolourGrid[index][i];
            string += " ";
        }      
    }
    return string;
}

int Mosaic::calculateEndGamePoints() {
    unsigned int endGamePoints = 0;


    endGamePoints += 2*numFullRows() + 7*numFullCols() + 10*noOfFiveColours();

    return endGamePoints;
}

unsigned int Mosaic::numFullCols() const {
    unsigned int counter = 0;
    unsigned int numCols = 0;

    for (unsigned int col = 0; col < MAX_COLS; ++col) {
        for (unsigned int row = 0; row < MAX_ROWS; ++row) {
            if(grid[row][col] != nullptr){
                ++counter;
                if(counter == MAX_NO_TILES){
                    ++numCols;
                }
            }
        }
        counter = 0;
    }

    return numCols;
}

unsigned int Mosaic::numFullRows() const {
    unsigned int counter = 0;
    unsigned int numRows = 0;

    for (unsigned int row = 0; row < MAX_ROWS; ++row) {
        for (unsigned int col = 0; col < MAX_COLS; ++col) {
            if(grid[row][col] != nullptr){
                ++counter;
                if(counter == MAX_NO_TILES){
                    ++numRows;
                }
            }
        }
        counter = 0;
    }

    return numRows;
}