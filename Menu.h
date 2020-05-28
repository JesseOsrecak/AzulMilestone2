
/*
 * Menu
 * 
 * Should not directly read or write to any stream
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef MENU_H
#define MENU_H

#define MENU_NEW        std::string("1")
#define MENU_LOAD       std::string("2")
#define MENU_SAVE       std::string("3")
#define MENU_CREDITS    std::string("4")
#define MENU_EXIT       std::string("5")

class Menu {
    public:
        Menu();
        ~Menu();

        // The title banner
        std::string getBanner();

        // The credits, including authors
        std::string getCredits();

        // The style used for user input
        std::string getUserPrompt();

        // Prompt user for input, and provide contextual message
        std::string getUserPrompt(std::string msg);

        // Get list of potential options for player to choose from
        std::string getOptions();
};

#endif // MENU_H
