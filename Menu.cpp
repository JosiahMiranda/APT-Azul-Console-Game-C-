#include <iostream>
#include "Menu.h"
#include<string>
#include<stdlib.h>
#include<exception>

Menu::Menu() {
    this->gameEngine = new GameEngine();
}

Menu::Menu(int seed, bool AItesting) {
    this->gameEngine = new GameEngine(seed, AItesting);
}

Menu::~Menu(){
    delete this->gameEngine;
    this->gameEngine = nullptr;
}

void Menu::printWelcome() {
    std::cout << "********************" << std::endl;
    std::cout << "Welcome to Azul!" << std::endl;
    std::cout << "********************" << std::endl;
}

void Menu::runMenu() {
    std::cout << "----" << std::endl;
    std::cout << "Menu" << std::endl;
    std::cout << "----" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game/Replay" << std::endl;
    std::cout << "3. Credits (Show Student Information)" << std::endl;
    std::cout << "4. Quit" << std::endl << std::endl;

}

bool Menu::runSelection(unsigned const int selection) {

    // Parameters used for io redirection
    bool eof = false;
    bool continueMenuLoop = true;

    bool error = false;

    Input input;

    if (selection == OPTIONS::NEW_GAME) {

        std::cout << "=== Game Mode ===" << std::endl << std::endl;

        std::cout << "1. 1 Vs 1 Against AI" << std::endl;
        std::cout << "2. 2-Player" << std::endl;
        std::cout << "3. 3-Player" << std::endl;
        std::cout << "4. 4-Player" << std::endl << std::endl;
        std::cout << "=================" << std::endl << std::endl;

        std::cout << "Please select Game Mode." << std::endl;
        int gameMode = input.getInt();


        if (gameMode == 1) {

            std::cout << "Please enter player one name:" << std::endl;
            std::string playerOneName = input.getString();

            if (playerOneName == "BOT") {
                std::cout << "Player cannot have the name 'BOT' as it is reserved for the AI." << std::endl;
                error = true;
            }

            if (!error) {
                std::cout << "Please choose your desired number of central factories - 1 or 2." << std::endl;
                int numCentralFactories = input.getInt();

                if (numCentralFactories == 1 || numCentralFactories == 2) {
                    std::cout << "Welcome " << playerOneName << "!" << std::endl;

                    int numNormalFactories = 5;
                    this->gameEngine->newGame(playerOneName, numNormalFactories, numCentralFactories, gameMode+1);
                    this->gameEngine->gameplayLoop(eof, continueMenuLoop);
                } else {
                    std::cout << "Number of Central Factories must be either 1 or 2." << std::endl;
                    error = true;
                }
            }

        } else if (gameMode == 2) {

            std::cout << "Please enter player one name:" << std::endl;
            std::string playerOneName = input.getString();
            std::cout << "Please enter player two name:" << std::endl;
            std::string playerTwoName = input.getString();

            if (playerOneName == playerTwoName) {
                std::cout << "Both players cannot have the same name." << std::endl;
                error = true;
            }

            if (!error) {

                std::cout << "Please choose your desired number of central factories." << std::endl;
                int numCentralFactories = input.getInt();

                if (numCentralFactories == 1 || numCentralFactories == 2) {
                    std::cout << "Welcome " << playerOneName << " and " << playerTwoName << "!" << std::endl;

                    int numNormalFactories = 5;
                    this->gameEngine->newGame(playerOneName, playerTwoName, numNormalFactories, numCentralFactories, gameMode);
                    this->gameEngine->gameplayLoop(eof, continueMenuLoop);
                } else {
                    std::cout << "Number of Central Factories must be either 1 or 2." << std::endl;
                }
            }

        } else if (gameMode == 3) {

            std::cout << "Please enter player one name:" << std::endl;
            std::string playerOneName = input.getString();
            std::cout << "Please enter player two name:" << std::endl;
            std::string playerTwoName = input.getString();
            std::cout << "Please enter player three name:" << std::endl;
            std::string playerThreeName = input.getString();

            if (playerOneName == playerTwoName || playerOneName == playerThreeName || playerTwoName == playerThreeName) {
                std::cout << "Two players cannot have the same name." << std::endl;
                error = true;
            }

            if (!error) {

                std::cout << "Please choose your desired number of central factories." << std::endl;
                int numCentralFactories = input.getInt();

                if (numCentralFactories == 1 || numCentralFactories == 2) {

                    std::cout << "Welcome " << playerOneName << ", " << playerTwoName << ", and " << playerThreeName << "!" << std::endl;

                    int numNormalFactories = 7;
                    this->gameEngine->newGame(playerOneName, playerTwoName, playerThreeName, numNormalFactories, numCentralFactories, gameMode);
                    this->gameEngine->gameplayLoop(eof, continueMenuLoop);
                } else {
                    std::cout << "Number of Central Factories must be either 1 or 2." << std::endl;
                }
            }

        } else if (gameMode == 4) {

            std::cout << "Please enter player one name:" << std::endl;
            std::string playerOneName = input.getString();
            std::cout << "Please enter player two name:" << std::endl;
            std::string playerTwoName = input.getString();
            std::cout << "Please enter player three name:" << std::endl;
            std::string playerThreeName = input.getString();
            std::cout << "Please enter player four name:" << std::endl;
            std::string playerFourName = input.getString();

            if (playerOneName == playerTwoName || playerOneName == playerThreeName || playerOneName == playerFourName
                || playerTwoName == playerThreeName || playerTwoName == playerFourName || playerThreeName == playerFourName) {
                std::cout << "Two players cannot have the same name." << std::endl;
                error = true;
            }

            if (!error) {
                std::cout << "Please choose your desired number of central factories." << std::endl;
                int numCentralFactories = input.getInt();

                if (numCentralFactories == 1 || numCentralFactories == 2) {

                    std::cout << "Welcome " << playerOneName << ", " << playerTwoName << ", " << playerThreeName
                                << ", and " << playerFourName << "!" << std::endl;

                    int numNormalFactories = 9;
                    this->gameEngine->newGame(playerOneName, playerTwoName, playerThreeName, playerFourName, numNormalFactories, numCentralFactories, gameMode);
                    this->gameEngine->gameplayLoop(eof, continueMenuLoop);
                } else {
                    std::cout << "Number of Central Factories must be either 1 or 2." << std::endl;
                }
            }

        }


    } else if (selection == OPTIONS::LOAD_GAME) { 
        std::string file = input.getString();

        std::string replay;
        bool validChoice = false;
        bool replayMode = false;

        std::cout << "Load game as a replay? (Type YES or NO)" << std::endl;
        while (!validChoice) {
            
            replay = input.getString();
            if (replay == "YES") {
                replayMode = true;
                validChoice = true;
            } else if (replay == "NO") {
                validChoice = true;
            } else {
                std::cout << "Invalid input. Please type YES or NO" << std::endl;
            }
        }

        GameEngineIO* gameEngineIO = new GameEngineIO(this->gameEngine);
        try{
            if (replayMode) {
                gameEngineIO->loadReplay(file);
                if (!gameEngineIO->getReadError()) {
                    std::cout << "Azul game successfully loaded" << std::endl;
                    this->gameEngine->gameplayLoop(eof, continueMenuLoop); 
                }
            } else {
                gameEngineIO->loadGame(file);
                if (!gameEngineIO->getReadError()) {
                    std::cout << "Azul game successfully loaded" << std::endl;
                    this->gameEngine->gameplayLoop(eof, continueMenuLoop); 
                }
            }

            if (gameEngineIO->getReadError()) {

            } else {
                std::cout << "" << std::endl;
            }


        }catch(const char* e){
            std::cerr<< e << std::endl;
        }
        delete gameEngineIO;
        
    } else if (selection == OPTIONS::CREDITS) {
        std::cout << "------------------------" << std::endl << std::endl;
        printCredits("Ian Nguyen", "s3788210");
        printCredits("Paula Kurniawan", "s3782041");
        printCredits("Josiah Miranda", "s3782051");
        std::cout << "------------------------" << std::endl << std::endl;
    } else if (selection == OPTIONS::QUIT) {
        std::cout << "Goodbye" << std::endl;
    } else {
        std::cout << "Please enter a valid number." << std::endl;
    }

    return continueMenuLoop;
}

void Menu::printCredits(const std::string name, const std::string studentID) {
    std::cout << "Name: " << name << std::endl;
    std::cout << "Student ID: " << studentID << std::endl;
    std::cout << "Email: " << studentID << "@student.rmit.edu.au" << std::endl << std::endl;
}

