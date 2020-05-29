
#include <fstream>
#include <iostream>
#include <string>

#define COMMENT_MARKER      '#'
#define KEY_VALUE_DELIMITER '='

#include "IOHandler.h"

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::ofstream;
using std::string;

IOHandler::IOHandler() :
    wasEof(false)
{}

IOHandler::~IOHandler() {}

bool IOHandler::readFromStdIn(string& input) {
    return readFromStream(input, cin);
}

bool IOHandler::loadGameFile(map<string, string>& rawData, const string& fileName) {
    std::ifstream inputFile(fileName);
    string line = "";
    
    bool success = false;

    if(inputFile) {
        while(readFromStream(line, inputFile)) {
            if(!line.empty()) {
                if (line.at(0) != COMMENT_MARKER) {
                    addDataTuple(rawData, line);
                }
            }
        }
        
        inputFile.close();
    }
    
    return success;
}

void IOHandler::addDataTuple(map<string, string>& rawData, const string& line) {
    // Stored in the file as: SOME_KEY=SOME_VALUE
    string key = line.substr(0, line.find(KEY_VALUE_DELIMITER));
    string value = line.substr(key.length() + 1);

    if (!key.empty()) {
        rawData[key] = value;
    }
}

bool IOHandler::readFromStream(std::string& input, std::istream& stream) {
    bool success = false;
    wasEof = false;

    if(stream.good()) {
        std::getline(stream, input);
        success = true;
    }
    
    if(stream.eof()) {
        wasEof = true;
    }

    return success;
}

void IOHandler::printToStdOut(string output) {
    cout << output;
}

void IOHandler::printToFile(string output, string fileName) {
    ofstream outputFile(fileName);

    if (outputFile.is_open()) {
        outputFile << output;
        outputFile.close();
    } else {
        // something went wrong
    }
}

bool IOHandler::eof() {
    return wasEof;
}

int IOHandler::readIntFromStdIn() 
{
    int input = 0;

        std::cin >> input;

    return input;
}
