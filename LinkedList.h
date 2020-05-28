
/*
 * Linked List
 * 
 * Linked list where all nodes are tiles. Is designed to be used with tile collections only.
 * 
 * Authors: C. Hodgen (s3031209), J. Osrecak (s3782455)
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <memory>

#include "Node.h"
#include "Tile.h"

class LinkedList {
    public:
        // Constructs an empty linked list
        LinkedList();

        // Deconstructs linked List
        ~LinkedList();
        
        // Returns the size of the Linked List
        unsigned int size() const;

        // Clears the linked list (deletes all nodes)
        void clear();

        // Adds a node to the front of the linked list
        void addFront(std::shared_ptr<Tile> data);

        // Returns the tile and removes from the start of the Linked List
        std::shared_ptr<Tile> deleteFront();

        // Add a new node to the back
        void addBack(std::shared_ptr<Tile> data);

        // Returns the tile and removes from the end of the Linked List
        std::shared_ptr<Tile> deleteBack();

        // Returns a tile reference at the index Index must not be greater than size. Index 0 = head
        std::shared_ptr<Tile> get(const unsigned int index) const;

    private:
        // Tracks the head of the list
        std::shared_ptr<Node> head;

        // Tracks the tail of the list
        std::shared_ptr<Node> tail;
        
        // Tracks the length of the list
        unsigned int length;
};

#endif // LINKED_LIST_H
