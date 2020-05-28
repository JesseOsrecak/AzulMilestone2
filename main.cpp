
/*
 * Main
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#include <iostream>
#include <memory>
#include <string>

#include "GameEngine.h"

class Args {
public:
   bool haveSeed;
   int seed;
};

void processArgs(int argc, char** argv, Args& args);

int main(int argc, char** argv) {
    // Process the args
    Args args;
    processArgs(argc, argv, args);

    GameEngine gameEngine;

    if (args.haveSeed) {
        gameEngine.setSeed(args.seed);
    }
    
    // Print the welcome game banner
    gameEngine.printBanner();
    
    // Start the primary game loop
    gameEngine.run();

    // Display the exit credits
    gameEngine.printCredits();

    return EXIT_SUCCESS;
}

void processArgs(int argc, char** argv, Args& args) {
    args.haveSeed = false;

    if (argc >= 2) {
        try {
            args.seed = std::stoi(argv[1]);
            args.haveSeed = true;
        } catch (std::invalid_argument& e) {
            std::cout << "Warning: provided seed is not valid." << std::endl;
        }
    } 
}
