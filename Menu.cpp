
#include <iostream>
#include <string>

#include "Menu.h"

Menu::Menu() {}

Menu::~Menu() {}

std::string Menu::getBanner() {

    std::string intro = "\n                  db                         `7MMF'\n";
    intro += "     ,;//'       ;MM:     M\"\"\"MMV `7MM  `7MM   MM        `\\\\.\n";
    intro += "  ,;//'         ,V^MM.    '  AMV    MM    MM   MM           `\\\\:.\n";
    intro += ",//'  ,og.  ,  ,M  `MM      AMV     MM    MM   MM     ,og.  ,  `\\\\.\n";
    intro += "`\\\\. \"  `6o\"   AbmmmqMA    AMV  ,   MM    MM   MM   ,\"  `6o\"  ,;//'\n";
    intro += "   `\\\\:.      A'     VML  AMMmmmM   `Mbod\"Y'   MM    ,M    ,;//'\n";
    intro += "      `\\\\.  .AMA.   .AMMA.                   .JMMmmmMMM  ,//'\n\n";

    intro += "                     A GAME BY MICHAEL KIESLING\n\n";

    return intro;
}

std::string Menu::getOptions() {
    std::string menu = 
    "\nNew Game:   " + MENU_NEW + "\n"
    + "Load Game:  " + MENU_LOAD + "\n"
    + "Save Game:  " + MENU_SAVE + "\n"
    + "Credits:    " + MENU_CREDITS + "\n"
    + "Exit:       " + MENU_EXIT + "\n\n";

    return menu;
}

std::string Menu::getCredits() {
    std::string credits = 
               "\n                              AUTHORS\n";
    credits += "            Chris Hodgen  (s3031209@student.rmit.edu.au)\n";
    credits += "            Jesse Osrecak (s3782455@student.rmit.edu.au)\n\n";

    credits += "                             BANNER ART\n";
    credits += "         Adapted from tools available at http://patorjk.com\n\n";

    return credits;
}

std::string Menu::getUserPrompt() {
    return "> ";
}

std::string Menu::getUserPrompt(std::string msg) {
    std::string userPrompt = "";
    userPrompt += msg;
    userPrompt += "\n";
    userPrompt += getUserPrompt();
    return userPrompt;
}
