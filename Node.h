
/*
 * Node
 * 
 * Node for linked list class where its value is actually a tile.
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef NODE_H
#define NODE_H

#include <memory>

#include "Tile.h"

class Node {
    public:
        // Creates a node with a tile
        Node(std::shared_ptr<Tile> data);

        // Constructs a node with a tile, that points to another node.
        Node(std::shared_ptr<Tile> data, std::shared_ptr<Node> next);

        // Creates a copy of a node
        Node(const Node& other);

        ~Node();
        
        // Holds the type of tile it is
        std::shared_ptr<Tile> data;
        
        // Holds link to next node
        std::shared_ptr<Node> next; 
};

#endif // NODE_H
