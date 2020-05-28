
#include <memory>

#include "Node.h"
#include "Tile.h"

using std::shared_ptr;

Node::Node(shared_ptr<Tile> data) :
    data(data),
    next(nullptr)
{}

Node::Node(shared_ptr<Tile> data, shared_ptr<Node> next) :
    data(data),
    next(next)
{}

Node::Node(const Node& other) :
    data(other.data),
    next(other.next)
{}

Node::~Node() {}
