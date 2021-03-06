#ifndef MENU_H
#define MENU_H

#include <string>
#include "Input.h"
#include "GameEngine/GameEngine.h"
#include "GameEngine/GameEngineIO.h"

enum OPTIONS {
    NO_OPTION,
    NEW_GAME,
    LOAD_GAME,
    CREDITS,
    QUIT,
};

class Menu {
public:
    // Constructor
    Menu();

    // Constructor for when they define seed
    Menu(int seed, bool AItesting);

    ~Menu();
    // Print the menu.
    void runMenu();

    // Print Welcome Message.
    void printWelcome();

    bool runSelection(unsigned const int selection);

    void printCredits(const std::string name, const std::string studentID);

private:
    // Input for Menu
    Input input;
    GameEngine* gameEngine;
};

#endif // MENU_H