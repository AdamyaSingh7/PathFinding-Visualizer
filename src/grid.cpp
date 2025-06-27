#include "../include/grid.h"
#include <queue>
#include <cmath>
#include <thread>
#include <chrono>
#include <stack>

Grid::Grid(int rows, int cols, int windowWidth, int windowHeight)
: rows(rows)
, cols(cols)
, cellWidth(windowWidth / cols)
, cellHeight(windowHeight / rows)
{
    grid.resize(rows, std::vector<Node>(cols));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            grid[r][c] = Node(c, r, cellWidth, cellHeight);
        }
    }
}

void Grid::draw(sf::RenderWindow& window) const {
    for (const auto& row : grid) {
        for (const auto& node : row) {
            node.draw(window);
        }
    }
}
void delay(int milliseconds);

void Grid::handleMouseClick(const sf::Event::MouseButtonPressed& mouseEvent, sf::RenderWindow&) {
    int x = mouseEvent.position.x;
    int y = mouseEvent.position.y;
    auto pos = getCellPositionFromMouse(x, y);

    if (pos.x < 0 || pos.x >= cols || pos.y < 0 || pos.y >= rows)
        return;

    Node& node = grid[pos.y][pos.x];

    if (mouseEvent.button == sf::Mouse::Button::Left) {
        if (placingStart) {
            node.setType(NodeType::START);
            startPos = pos;
            placingStart = false;
        } else if (placingEnd) {
            node.setType(NodeType::END);
            endPos = pos;
            placingEnd = false;
        } else {
            if (node.getType() == NodeType::EMPTY)
                node.setType(NodeType::WALL);
        }
    } else if (mouseEvent.button == sf::Mouse::Button::Right) {
        NodeType type = node.getType();
        if (type == NodeType::START) {
            placingStart = true;
            startPos = {-1, -1};
        } else if (type == NodeType::END) {
            placingEnd = true;
            endPos = {-1, -1};
        }
        node.setType(NodeType::EMPTY);
    }
}

void Grid::reset() {
    for (auto& row : grid)
        for (auto& node : row)
            node.setType(NodeType::EMPTY);

    startPos = {-1, -1};
    endPos = {-1, -1};
    placingStart = true;
    placingEnd = true;
}

sf::Vector2i Grid::getCellPositionFromMouse(int x, int y) const {
    return { x / cellWidth, y / cellHeight };
}

float heuristic(const sf::Vector2i& a, const sf::Vector2i& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y); // Manhattan distance
}

bool Grid::runAStar(sf::RenderWindow& window) {
    if (startPos.x == -1 || endPos.x == -1)
        return false;

    auto cmp = [](Node* a, Node* b) { return a->getF() > b->getF(); };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> openSet(cmp);

    // Reset nodes
    for (auto& row : grid) {
        for (auto& node : row) {
            node.resetPathfindingData();
            if (node.getType() == NodeType::VISITED || node.getType() == NodeType::PATH)
                node.setType(NodeType::EMPTY);
        }
    }

    Node& start = grid[startPos.y][startPos.x];
    Node& end = grid[endPos.y][endPos.x];

    start.setG(0);
    start.setH(heuristic(startPos, endPos));
    openSet.push(&start);

    while (!openSet.empty()) {
        Node* current = openSet.top();
        openSet.pop();

        sf::Vector2i curPos(current->getGridX(), current->getGridY());

        if (curPos == endPos) {
            Node* pathNode = current->getParent();
            while (pathNode && pathNode != &start) {
                pathNode->setType(NodeType::PATH);
                pathNode = pathNode->getParent();
                draw(window);
                window.display();
                delay(30);
            }
            return true;
        }

        if (current != &start && current->getType() != NodeType::END)
            current->setType(NodeType::VISITED);

        for (const auto& dir : directions) {
            int nx = curPos.x + dir.x;
            int ny = curPos.y + dir.y;
            if (!isValid(nx, ny)) continue;

            Node& neighbor = grid[ny][nx];
            if (neighbor.getType() == NodeType::WALL) continue;

            float tentativeG = current->getG() + 1;
            if (neighbor.getParent() == nullptr || tentativeG < neighbor.getG()) {
                neighbor.setParent(current);
                neighbor.setG(tentativeG);
                neighbor.setH(heuristic({nx, ny}, endPos));
                openSet.push(&neighbor);
            }
        }

        draw(window);
        window.display();
        delay(20);
    }

    return false;
}

bool Grid::runDijkstra(sf::RenderWindow& window) {
    // 1) Make sure start/end are placed
    if (startPos.x == -1 || endPos.x == -1)
        return false;

    // 2) Clear old VISITED/PATH coloring (but keep walls, start, end)
    for (auto& row : grid) {
        for (auto& node : row) {
            if (node.getType() == NodeType::VISITED || node.getType() == NodeType::PATH)
                node.setType(NodeType::EMPTY);
        }
    }

    // 3) Setup local structures
    const float INF = std::numeric_limits<float>::infinity();
    int H = rows, W = cols;
    std::vector<std::vector<float>> dist(H, std::vector<float>(W, INF));
    std::vector<std::vector<bool>> done(H, std::vector<bool>(W, false));
    std::vector<std::vector<Node*>> prev(H, std::vector<Node*>(W, nullptr));

    using PQNode = std::pair<float, std::pair<int,int>>;
    std::priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>> pq;

    // 4) Initialize
    dist[startPos.y][startPos.x] = 0;
    pq.push({0, { startPos.x, startPos.y }});

    // 5) Main Dijkstra loop
    while (!pq.empty()) {
        auto [d, pos] = pq.top();
        pq.pop();

        int x = pos.first, y = pos.second;
        if (done[y][x])
            continue;
        done[y][x] = true;

        Node& curNode = grid[y][x];
        // Mark visited (but leave start/end green/red)
        if ((y != startPos.y || x != startPos.x) && (y != endPos.y || x != endPos.x))
            curNode.setType(NodeType::VISITED);

        // If we've reached the end, stop relaxing
        if (x == endPos.x && y == endPos.y)
            break;

        // Relax neighbors
        for (auto& dxy : directions) {
            int nx = x + dxy.x, ny = y + dxy.y;
            if (!isValid(nx, ny))
                continue;
            Node& nb = grid[ny][nx];
            if (nb.getType() == NodeType::WALL || done[ny][nx])
                continue;

            float nd = d + 1;  // edge weight = 1
            if (nd < dist[ny][nx]) {
                dist[ny][nx] = nd;
                prev[ny][nx] = &curNode;
                pq.push({nd, {nx, ny}});
            }
        }

        // animate one step
        window.clear(sf::Color::Black);
        draw(window);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    // 6) Check if end was reached
    if (!done[endPos.y][endPos.x]) {
        return false;  // no path
    }

    // 7) Reconstruct path via prev[][], coloring in PATH (yellow)
    Node* p = prev[endPos.y][endPos.x];
    while (p && p != &grid[startPos.y][startPos.x]) {
        p->setType(NodeType::PATH);

        // animate path drawing
        window.clear(sf::Color::Black);
        draw(window);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(30));

        sf::Vector2i pp(p->getGridX(), p->getGridY());
        p = prev[pp.y][pp.x];
    }

    return true;
}


bool Grid::runBFS(sf::RenderWindow& window) {
    if (startPos.x == -1 || endPos.x == -1) return false;

    for (auto& row : grid)
        for (auto& node : row) {
            node.resetPathfindingData();
            if (node.getType() == NodeType::VISITED || node.getType() == NodeType::PATH)
                node.setType(NodeType::EMPTY);
        }

    std::queue<Node*> q;
    Node& start = grid[startPos.y][startPos.x];
    Node& end = grid[endPos.y][endPos.x];
    q.push(&start);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        sf::Vector2i curPos(current->getGridX(), current->getGridY());

        if (curPos == endPos) {
            Node* pathNode = current->getParent();
            while (pathNode && pathNode != &start) {
                pathNode->setType(NodeType::PATH);
                pathNode = pathNode->getParent();
                draw(window);
                window.display();
                delay(30);
            }
            return true;
        }

        if (current != &start && current->getType() != NodeType::END)
            current->setType(NodeType::VISITED);

        for (const auto& dir : directions) {
            int nx = curPos.x + dir.x;
            int ny = curPos.y + dir.y;
            if (!isValid(nx, ny)) continue;

            Node& neighbor = grid[ny][nx];
            if (neighbor.getType() == NodeType::WALL || neighbor.getParent()) continue;

            neighbor.setParent(current);
            q.push(&neighbor);
        }

        draw(window);
        window.display();
        delay(20);
    }

    return false;
}

bool Grid::runDFS(sf::RenderWindow& window) {
    if (startPos.x == -1 || endPos.x == -1) return false;

    for (auto& row : grid)
        for (auto& node : row) {
            node.resetPathfindingData();
            if (node.getType() == NodeType::VISITED || node.getType() == NodeType::PATH)
                node.setType(NodeType::EMPTY);
        }

    std::stack<Node*> s;
    Node& start = grid[startPos.y][startPos.x];
    Node& end = grid[endPos.y][endPos.x];
    s.push(&start);

    while (!s.empty()) {
        Node* current = s.top();
        s.pop();

        sf::Vector2i curPos(current->getGridX(), current->getGridY());

        if (curPos == endPos) {
            Node* pathNode = current->getParent();
            while (pathNode && pathNode != &start) {
                pathNode->setType(NodeType::PATH);
                pathNode = pathNode->getParent();
                draw(window);
                window.display();
                delay(30);
            }
            return true;
        }

        if (current != &start && current->getType() != NodeType::END)
            current->setType(NodeType::VISITED);

        for (const auto& dir : directions) {
            int nx = curPos.x + dir.x;
            int ny = curPos.y + dir.y;
            if (!isValid(nx, ny)) continue;

            Node& neighbor = grid[ny][nx];
            if (neighbor.getType() == NodeType::WALL || neighbor.getParent()) continue;

            neighbor.setParent(current);
            s.push(&neighbor);
        }

        draw(window);
        window.display();
        delay(20);
    }

    return false;
}

void delay(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
