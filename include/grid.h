#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include "node.h"

class Grid {
public:
    Grid(int rows, int cols, int windowWidth, int windowHeight);

    void draw(sf::RenderWindow& window) const;
    void handleMouseClick(const sf::Event::MouseButtonPressed& mouseEvent, sf::RenderWindow& window);
    void reset();

    bool runAStar(sf::RenderWindow& window);
    bool runDijkstra(sf::RenderWindow& window);
    bool runBFS(sf::RenderWindow& window);
    bool runDFS(sf::RenderWindow& window);

private:
    int rows, cols;
    int cellWidth, cellHeight;
    std::vector<std::vector<Node>> grid;

    sf::Vector2i startPos{-1, -1};
    sf::Vector2i endPos{-1, -1};
    bool placingStart = true;
    bool placingEnd = true;

    sf::Vector2i getCellPositionFromMouse(int x, int y) const;

    // Helper for movement directions (up, left, right, down)
    const std::vector<sf::Vector2i> directions = {
        {0, -1}, {-1, 0}, {1, 0}, {0, 1}
    };

    // Helper to check grid bounds
    bool isValid(int x, int y) const {
        return x >= 0 && x < cols && y >= 0 && y < rows;
    }
};

#endif // GRID_H
