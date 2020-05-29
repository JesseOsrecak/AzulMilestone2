
/*
 * IO Handler
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include <map>
#include <string>

class IOHandler {
    public:
        IOHandler();
        ~IOHandler();

        // Receive a line of user input using stdin
        bool readFromStdIn(std::string& input);

        int readIntFromStdIn();

        // Load game file into data map
        bool loadGameFile(std::map<std::string, std::string>& rawData,
                          const std::string& fileName);
        
        // Print string to stdout
        void printToStdOut(const std::string output);

        // Print a string to a file, overwriting it if it exists
        void printToFile(const std::string output, const std::string fileName);

        // Check for eof
        bool eof();

    private:
        // Set if last read request resulted in an eof
        bool wasEof;

        // Receive a line from an input stream
        bool readFromStream(std::string& input, std::istream& stream);

        // Parse and add key/value tuple to game data while loading
        void addDataTuple(std::map<std::string, std::string>& rawData,
                          const std::string& line);
};

#endif // IO_HANDLER_H
