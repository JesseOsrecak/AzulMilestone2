
/*
 * Floor line
 * 
 * Extends the PatternLine class
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef FLOOR_LINE_H
#define FLOOR_LINE_H

#include "PatternLine.h"

class FloorLine : public PatternLine {
    public:
        FloorLine();

        // Get a printable string of the tile colours
        std::string toString();
};

#endif // FLOOR_LINE_H
