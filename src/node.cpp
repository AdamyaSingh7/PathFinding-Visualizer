#include "../include/node.h"

Node::Node()
: gridX(0), gridY(0), width(0), height(0), type(NodeType::EMPTY)
{}

Node::Node(int gridX, int gridY, int width, int height)
: gridX(gridX), gridY(gridY), width(width), height(height), type(NodeType::EMPTY)
{
    shape.setSize(sf::Vector2f(width - 1.f, height - 1.f));
    shape.setPosition(sf::Vector2f(gridX * width, gridY * height));
    updateColor();
}

void Node::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

void Node::setType(NodeType newType) {
    type = newType;
    updateColor();
}

NodeType Node::getType() const {
    return type;
}

void Node::updateColor() {
    switch (type) {
        case NodeType::EMPTY:   shape.setFillColor(sf::Color::White); break;
        case NodeType::WALL:    shape.setFillColor(sf::Color::Black); break;
        case NodeType::START:   shape.setFillColor(sf::Color::Green); break;
        case NodeType::END:     shape.setFillColor(sf::Color::Red); break;
        case NodeType::VISITED: shape.setFillColor(sf::Color(100,100,255)); break;
        case NodeType::PATH:    shape.setFillColor(sf::Color::Yellow); break;
    }
}
