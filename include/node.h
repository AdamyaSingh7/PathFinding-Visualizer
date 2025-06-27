#ifndef NODE_H
#define NODE_H

#include <SFML/Graphics.hpp>

enum class NodeType {
    EMPTY,
    WALL,
    START,
    END,
    VISITED,
    PATH
};

class Node {
public:
    Node();
    Node(int gridX, int gridY, int width, int height);

    // draw must be const so it can be called on const references
    void draw(sf::RenderWindow& window) const;
    void setType(NodeType type);
    NodeType getType() const;
    void setG(float val) { g = val; updateF(); }
    void setH(float val) { h = val; updateF(); }
    void setParent(Node* p) { parent = p; }
    float getF() const { return f; }
    float getG() const { return g; }
    float getH() const { return h; }
    Node* getParent() const { return parent; }
    void resetPathfindingData() {
        f = g = h = 0;
        parent = nullptr;
    }
    int getGridX() const { return gridX; }
    int getGridY() const { return gridY; }


private:
    int gridX, gridY;
    int width, height;
    NodeType type;
    sf::RectangleShape shape;
    float f = 0, g = 0, h = 0;
    Node* parent = nullptr;
    void updateColor();
    void updateF() { f = g + h; }
};

#endif // NODE_H
