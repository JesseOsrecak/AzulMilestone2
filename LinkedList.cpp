
#include <memory>

#include "LinkedList.h"
#include "Tile.h"

using std::make_shared;
using std::move;
using std::shared_ptr;

LinkedList::LinkedList() :
    head(nullptr),
    tail(nullptr),
    length(0)
{}

LinkedList::~LinkedList() {
    clear();
}

unsigned int LinkedList::size() const {
    return length;
}

void LinkedList::clear() {
    head = nullptr;
    tail = nullptr;
    length = 0;
}

shared_ptr<Tile> LinkedList::get(const unsigned int index) const {
    unsigned int counter = 0;
    shared_ptr<Node> current = head;
    shared_ptr<Tile> result = nullptr;

    if (index < size()) {
        while(counter < index) {
            ++counter;
            current = current->next;
        }

        result = current->data;
    }

    return result;
}

shared_ptr<Tile> LinkedList::deleteBack() {
    shared_ptr<Tile> result = nullptr;

    if (head) {
        shared_ptr<Node> curr = head;
        shared_ptr<Node> prev = nullptr;

        while (curr->next) {
            prev = curr;
            curr = curr->next;
        }

        result = curr->data;

        if (prev) {
            prev->next = nullptr;
        }

        --length;
    }

    return result;
}

void LinkedList::addFront(shared_ptr<Tile> data) {
    shared_ptr<Node> newNode = make_shared<Node>(move(data), head);
    head = newNode;
    ++length;

    if (length == 1) {
        tail = head;
    }
}

shared_ptr<Tile> LinkedList::deleteFront() {
    shared_ptr<Tile> result = head->data;
    head = head->next;

    if(head == nullptr) {
        tail = nullptr;
    }

    --length;

    return result;
}

void LinkedList::addBack(shared_ptr<Tile> data) {
    shared_ptr<Node> newNode = make_shared<Node>(move(data), nullptr);

    if (!tail) {
        tail = newNode;
        head = tail;
    } else {
        tail->next = newNode;
        tail = newNode;
    }

    ++length;
}
