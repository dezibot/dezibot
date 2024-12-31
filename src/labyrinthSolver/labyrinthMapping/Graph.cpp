#include "Graph.h"
#include <stdexcept>


Graph::Graph() : nextNodeId(1), currentMode(Mode::EXPLORER) {
    auto startNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);
    startNode->isStart = true;
    startNode->explored = true;
    auto firstNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);
    
    addEdge(startNode, firstNode, Direction::Straight);
    nodes[startNode->id] = startNode;
    nodes[firstNode->id] = firstNode;
    currentNode = firstNode;
    previousNode =startNode;

}


void Graph::setMode(Mode mode) {
    currentMode = mode;
    if (mode == Mode::SOLVER){
        currentNode = nodes[2];
        findShortestPath();

        solutionStack.pop();
        solutionStack.pop();
    }
}

Mode Graph::getMode() const {
    return currentMode;
}

void Graph::setGoalNode() {
    currentNode->isGoal = true;
    currentNode->explored = true;
    setMode(Mode::SOLVER);
}

void Graph::createNeighbours(std::shared_ptr<Node> node, CrossingType type) {
    if (!node) {
        throw std::invalid_argument("Node cannot be null.");
    }

    switch (type) {
        case CrossingType::X: {
            auto rightNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);
            auto leftNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);
            auto straightNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);

            nodes[rightNode->id] = rightNode;
            nodes[leftNode->id] = leftNode;
            nodes[straightNode->id] = straightNode;

            addEdge(node, rightNode, Direction::Right);
            addEdge(node, leftNode, Direction::Left);
            addEdge(node, straightNode, Direction::Straight);
            break;
        }
        case CrossingType::T1: {
            auto rightNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);
            auto straightNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);

            nodes[rightNode->id] = rightNode;
            nodes[straightNode->id] = straightNode;

            addEdge(node, rightNode, Direction::Right);
            addEdge(node, straightNode, Direction::Straight);
            break;
        }
        case CrossingType::T2: {
            auto leftNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);
            auto straightNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);

            nodes[leftNode->id] = leftNode;
            nodes[straightNode->id] = straightNode;

            addEdge(node, leftNode, Direction::Left);
            addEdge(node, straightNode, Direction::Straight);
            break;
        }
        case CrossingType::T3: {
            auto rightNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);
            auto leftNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);

            nodes[leftNode->id] = leftNode;
            nodes[rightNode->id] = rightNode;

            addEdge(node, rightNode, Direction::Right);
            addEdge(node, leftNode, Direction::Left);
            break;
        }
        case CrossingType::DEAD_END:
            break;
    }
}

void Graph::setNeighbor(std::shared_ptr<Node> node, std::shared_ptr<Node> neighbor, Direction direction) {
    if (!node) {
        throw std::invalid_argument("Node cannot be null.");
    }
    node->neighbors[static_cast<int>(direction)] = neighbor;
}


void Graph::addEdge(std::shared_ptr<Node> fromNode, std::shared_ptr<Node> nextNode, Direction direction) {
    if (!fromNode || !nextNode) {
        throw std::invalid_argument("Nodes cannot be null.");
    }

    setNeighbor(fromNode, nextNode, direction);

    setNeighbor(nextNode, fromNode, Direction::Back);
}

Direction Graph::addCrossing(CrossingType crossingType) {
    if (currentMode == Mode::EXPLORER)
    {
        return addCrossingExploring(crossingType);
    } else {
        return getDirection();
    }    
}

Direction Graph::addCrossingExploring(CrossingType crossingType) {
    if (!currentNode) {
        throw std::invalid_argument("Current node cannot be null.");
    }

    if (!currentNode->explored){
        currentNode->crossingType = crossingType;
        createNeighbours(currentNode, crossingType);
        currentNode->explored = true;
    }

    auto leftmostNeighbor = getLeftMostNeighborPrev(currentNode, previousNode);
    int dir = 0;
    for (dir; dir < currentNode->neighbors.size(); ++dir) {
        if (currentNode->neighbors[dir] == leftmostNeighbor) {
            break;
        }
    }

    previousNode = currentNode;
    currentNode = leftmostNeighbor;

    if (crossingType == CrossingType::DEAD_END){
        return Direction::Back;
    }else if(crossingType == CrossingType::T1){
        return Direction::Straight;
    }else {
        return Direction::Left;
    }
}


std::shared_ptr<Node> Graph::getLeftMostNeighborPrev(const std::shared_ptr<Node>& currentNode, const std::shared_ptr<Node>& previousNode) {
    if (!currentNode || !previousNode) {
        throw std::invalid_argument("Nodes cannot be null.");
    }

    Direction comingFrom = Direction::Back;
    for (int dir = 0; dir < currentNode->neighbors.size(); ++dir) {
        if (currentNode->neighbors[dir] == previousNode) {
            comingFrom = static_cast<Direction>(dir);
            break;
        }
    }

    switch (comingFrom) {
        case Direction::Left:
            if (currentNode->neighbors[static_cast<int>(Direction::Straight)]) {
                return currentNode->neighbors[static_cast<int>(Direction::Straight)];
            }
            if (currentNode->neighbors[static_cast<int>(Direction::Right)]) {
                return currentNode->neighbors[static_cast<int>(Direction::Right)];
            }
            if (currentNode->neighbors[static_cast<int>(Direction::Back)]) {
                return currentNode->neighbors[static_cast<int>(Direction::Back)];
            }
            return currentNode->neighbors[static_cast<int>(Direction::Left)];

        case Direction::Straight:
            if (currentNode->neighbors[static_cast<int>(Direction::Right)]) {
                return currentNode->neighbors[static_cast<int>(Direction::Right)];
            }
            if (currentNode->neighbors[static_cast<int>(Direction::Back)]) {
                return currentNode->neighbors[static_cast<int>(Direction::Back)];
            }
            if (currentNode->neighbors[static_cast<int>(Direction::Left)]) {
                return currentNode->neighbors[static_cast<int>(Direction::Left)];
            }
            return currentNode->neighbors[static_cast<int>(Direction::Straight)];

        case Direction::Right:
            if (currentNode->neighbors[static_cast<int>(Direction::Back)]) {
                return currentNode->neighbors[static_cast<int>(Direction::Back)];
            }
            if (currentNode->neighbors[static_cast<int>(Direction::Left)]) {
                return currentNode->neighbors[static_cast<int>(Direction::Left)];
            }
            if (currentNode->neighbors[static_cast<int>(Direction::Straight)]) {
                return currentNode->neighbors[static_cast<int>(Direction::Straight)];
            }
            return currentNode->neighbors[static_cast<int>(Direction::Right)];

        case Direction::Back:
        default:
            if (currentNode->neighbors[static_cast<int>(Direction::Left)]) {
                return currentNode->neighbors[static_cast<int>(Direction::Left)];
            }
            if (currentNode->neighbors[static_cast<int>(Direction::Straight)]) {
                return currentNode->neighbors[static_cast<int>(Direction::Straight)];
            }
            if (currentNode->neighbors[static_cast<int>(Direction::Right)]) {
                return currentNode->neighbors[static_cast<int>(Direction::Right)];
            }
            return currentNode->neighbors[static_cast<int>(Direction::Back)];
    }
}

void Graph::findShortestPath() {
    if (nodes.empty()) {
        throw std::runtime_error("Graph is empty.");
    }

    while (!solutionStack.empty()) {
        solutionStack.pop();
    }

    std::shared_ptr<Node> startNode = nullptr, goalNode = nullptr;
    for (const auto& pair : nodes) {
        if (pair.second->isStart) {
            startNode = pair.second;
        }
        if (pair.second->isGoal) {
            goalNode = pair.second;
        }
    }

    if (!startNode) {
        throw std::runtime_error("Start node not found.");
    }
    if (!goalNode) {
        throw std::runtime_error("Goal node not found.");
    }

    std::queue<std::shared_ptr<Node>> toVisit;
    std::unordered_map<int, int> parent; 
    std::unordered_map<int, bool> visited; 

    toVisit.push(startNode);
    visited[startNode->id] = true;
    parent[startNode->id] = -1; 

    while (!toVisit.empty()) {
        auto currentNode = toVisit.front();
        toVisit.pop();

        if (currentNode->isGoal) {
            break;
        }

        for (const auto& neighbor : currentNode->neighbors) {
            if (neighbor && !visited[neighbor->id]) {
                visited[neighbor->id] = true;
                parent[neighbor->id] = currentNode->id;
                toVisit.push(neighbor);
            }
        }
    }

    for (int currentId = goalNode->id; currentId != -1; currentId = parent[currentId]) {
        solutionStack.push(currentId);
    }

    if (solutionStack.top() != startNode->id || solutionStack.empty()) {
        throw std::runtime_error("No valid path found between start and goal nodes.");
    }
}

Direction Graph::getDirection() {
    if (solutionStack.empty()) {
        throw std::runtime_error("Solution stack is empty. No further directions available.");
    }

    int nextNodeId = solutionStack.top();
    solutionStack.pop();

    for (int dir = 0; dir < currentNode->neighbors.size(); ++dir) {
        auto neighbor = currentNode->neighbors[dir];
        if (neighbor && neighbor->id == nextNodeId) {
            currentNode = neighbor;
            return static_cast<Direction>(dir);
        }
    }

    throw std::runtime_error("Next node from the solution stack not found in currentNode's neighbors.");
}