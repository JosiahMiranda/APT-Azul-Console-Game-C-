#include "Menu.h"
#include <iostream>
#include "Input.h"
#include <sstream>
#define EXIT_SUCCESS    0

int main(int argc, char** argv) {

    Input input;
    std::shared_ptr<Menu> menu;
    bool error = false;

    // Allows user to input seed to be used for the program
    if (argc >= 3) {
        if (input.inputIsInt(argv[2])) {
            std::stringstream sstream (argv[2]);
            int seed = 0;
            sstream >> seed;
            bool AItesting = false;
            if (argc >= 4) {
                // for extra testing. Command line argument lets all players be AI
                if (input.inputIsInt(argv[3])) {
                    std::stringstream AIstream (argv[3]);
                    int choice = 0;
                    AIstream >> choice;
                    // use 1 to symbol ai testing in 4th argument
                    if (choice == 1) {
                        AItesting = true;
                    }
                }
            }
            menu = std::make_shared<Menu>(seed, AItesting);
        } else {
            std::cout << "Seed must be an integer." << std::endl;
            error = true;
        }
    } else {
        menu = std::make_shared<Menu>();
    }

    if (!error) {
        
        menu->printWelcome();

        
        unsigned int selection = OPTIONS::NO_OPTION;
        bool continueMenuLoop = true;
        while (continueMenuLoop && selection != OPTIONS::QUIT) {
            menu->runMenu();
            selection = input.getInt();
            continueMenuLoop = menu->runSelection(selection);
        }

    }
    
    return EXIT_SUCCESS;
}