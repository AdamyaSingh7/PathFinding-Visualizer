#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "include/grid.h"
#include "include/button.h"
#include <optional>
#include <string>
#include <iostream>

constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 800;
constexpr int ROWS = 20;
constexpr int COLS = 20;

enum class Algorithm { ASTAR, DIJKSTRA, BFS, DFS };

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Pathfinding Visualizer");
    window.setFramerateLimit(60);

    Grid grid(ROWS, COLS, WINDOW_WIDTH, WINDOW_HEIGHT);

    sf::Font font;
    if (!font.openFromFile("assets/arial.ttf")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    Algorithm algorithm = Algorithm::ASTAR;
    bool pathNotFound = false;

    // Buttons
    Button aStarBtn("A*", {10, 700}, {80, 30}, font);
    Button dijkstraBtn("Dijkstra", {100, 700}, {110, 30}, font);
    Button bfsBtn("BFS", {220, 700}, {80, 30}, font);
    Button dfsBtn("DFS", {310, 700}, {80, 30}, font);
    Button runBtn("Run", {400, 700}, {80, 30}, font);
    Button resetBtn("Reset", {490, 700}, {100, 30}, font);
    Button quitBtn("Quit", {600, 700}, {80, 30}, font);

    // Set default selected algorithm button
    aStarBtn.setSelected(true);

    while (window.isOpen()) {
        std::optional<sf::Event> maybeEvent;
        while ((maybeEvent = window.pollEvent())) {
            const sf::Event& event = *maybeEvent;

            if (event.is<sf::Event::Closed>()) {
                window.close();
            } else if (event.is<sf::Event::MouseButtonPressed>()) {
                if (auto* mbp = event.getIf<sf::Event::MouseButtonPressed>()) {
                    sf::Vector2i mousePos = { mbp->position.x, mbp->position.y };

                    // Reset button states
                    aStarBtn.setSelected(false);
                    dijkstraBtn.setSelected(false);
                    bfsBtn.setSelected(false);
                    dfsBtn.setSelected(false);

                    // Check which button is clicked
                    if (aStarBtn.isHovered(mousePos)) {
                        algorithm = Algorithm::ASTAR;
                        aStarBtn.setSelected(true);
                        pathNotFound = false;
                    } else if (dijkstraBtn.isHovered(mousePos)) {
                        algorithm = Algorithm::DIJKSTRA;
                        dijkstraBtn.setSelected(true);
                        pathNotFound = false;
                    } else if (bfsBtn.isHovered(mousePos)) {
                        algorithm = Algorithm::BFS;
                        bfsBtn.setSelected(true);
                        pathNotFound = false;
                    } else if (dfsBtn.isHovered(mousePos)) {
                        algorithm = Algorithm::DFS;
                        dfsBtn.setSelected(true);
                        pathNotFound = false;
                    } else if (runBtn.isHovered(mousePos)) {
                        switch (algorithm) {
                            case Algorithm::ASTAR:
                                pathNotFound = !grid.runAStar(window);
                                break;
                            case Algorithm::DIJKSTRA:
                                pathNotFound = !grid.runDijkstra(window);
                                break;
                            case Algorithm::BFS:
                                pathNotFound = !grid.runBFS(window);
                                break;
                            case Algorithm::DFS:
                                pathNotFound = !grid.runDFS(window);
                                break;
                        }
                    } else if (resetBtn.isHovered(mousePos)) {
                        grid.reset();
                        pathNotFound = false;
                    } else if (quitBtn.isHovered(mousePos)) {
                        window.close();
                    } else {
                        grid.handleMouseClick(*mbp, window);
                    }
                }
            } else if (event.is<sf::Event::KeyPressed>()) {
                if (auto* kp = event.getIf<sf::Event::KeyPressed>()) {
                    switch (kp->code) {
                        case sf::Keyboard::Key::R:
                            grid.reset();
                            pathNotFound = false;
                            break;
                        case sf::Keyboard::Key::Q:
                            window.close();
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        window.clear(sf::Color::Black);
        grid.draw(window);

        // Draw buttons
        aStarBtn.draw(window);
        dijkstraBtn.draw(window);
        bfsBtn.draw(window);
        dfsBtn.draw(window);
        runBtn.draw(window);
        resetBtn.draw(window);
        quitBtn.draw(window);

        if (pathNotFound) {
            sf::Text noPathText(font, "No path found", 20);
            noPathText.setPosition({10.f, 10.f});
            noPathText.setFillColor(sf::Color::Red);
            window.draw(noPathText);
        }

        window.display();
    }

    return 0;
}
