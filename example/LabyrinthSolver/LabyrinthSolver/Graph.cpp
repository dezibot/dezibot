#include "Graph.h"
#include <stdexcept>
#include <Dezibot.h>

extern Dezibot dezibot;


Graph::Graph() : nextNodeId(1), currentMode(Mode::EXPLORER) {
    auto startNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);
    startNode->isStart = true;
    startNode->explored = true;
    auto firstNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);
    
    addEdge(startNode, firstNode,DirectionLabyrinth::Straight);
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

            addEdge(node, rightNode,DirectionLabyrinth::Right);
            addEdge(node, leftNode,DirectionLabyrinth::Left);
            addEdge(node, straightNode,DirectionLabyrinth::Straight);
            break;
        }
        case CrossingType::T1: {
            auto rightNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);
            auto straightNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);

            nodes[rightNode->id] = rightNode;
            nodes[straightNode->id] = straightNode;

            addEdge(node, rightNode,DirectionLabyrinth::Right);
            addEdge(node, straightNode,DirectionLabyrinth::Straight);
            break;
        }
        case CrossingType::T2: {
            auto leftNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);
            auto straightNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);

            nodes[leftNode->id] = leftNode;
            nodes[straightNode->id] = straightNode;

            addEdge(node, leftNode,DirectionLabyrinth::Left);
            addEdge(node, straightNode,DirectionLabyrinth::Straight);
            break;
        }
        case CrossingType::T3: {
            auto rightNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);
            auto leftNode = std::make_shared<Node>(nextNodeId++, CrossingType::DEAD_END);

            nodes[leftNode->id] = leftNode;
            nodes[rightNode->id] = rightNode;

            addEdge(node, rightNode,DirectionLabyrinth::Right);
            addEdge(node, leftNode,DirectionLabyrinth::Left);
            break;
        }
        case CrossingType::DEAD_END:
            break;
    }
}

void Graph::setNeighbor(std::shared_ptr<Node> node, std::shared_ptr<Node> neighbor, DirectionLabyrinth direction) {
    if (!node) {
        throw std::invalid_argument("Node cannot be null.");
    }
    node->neighbors[static_cast<int>(direction)] = neighbor;
}


void Graph::addEdge(std::shared_ptr<Node> fromNode, std::shared_ptr<Node> nextNode, DirectionLabyrinth direction) {
    if (!fromNode || !nextNode) {
        throw std::invalid_argument("Nodes cannot be null.");
    }

    setNeighbor(fromNode, nextNode, direction);

    setNeighbor(nextNode, fromNode,DirectionLabyrinth::Back);
}

DirectionLabyrinth Graph::addCrossing(CrossingType crossingType) {
    if (currentMode == Mode::EXPLORER)
    {
        return addCrossingExploring(crossingType);
    } else {
        return getDirection();
    }    
}

DirectionLabyrinth Graph::addCrossingExploring(CrossingType crossingType) {
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
        return DirectionLabyrinth::Back;
    }else if(crossingType == CrossingType::T1){
        return DirectionLabyrinth::Straight;
    }else {
        return DirectionLabyrinth::Left;
    }
}


std::shared_ptr<Node> Graph::getLeftMostNeighborPrev(const std::shared_ptr<Node>& currentNode, const std::shared_ptr<Node>& previousNode) {
    if (!currentNode || !previousNode) {
        throw std::invalid_argument("Nodes cannot be null.");
    }

    DirectionLabyrinth comingFrom =DirectionLabyrinth::Back;
    for (int dir = 0; dir < currentNode->neighbors.size(); ++dir) {
        if (currentNode->neighbors[dir] == previousNode) {
            comingFrom = static_cast<DirectionLabyrinth>(dir);
            break;
        }
    }

    switch (comingFrom) {
        case DirectionLabyrinth::Left:
            if (currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Straight)]) {
                return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Straight)];
            }
            if (currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Right)]) {
                return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Right)];
            }
            if (currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Back)]) {
                return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Back)];
            }
            return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Left)];

        case DirectionLabyrinth::Straight:
            if (currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Right)]) {
                return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Right)];
            }
            if (currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Back)]) {
                return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Back)];
            }
            if (currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Left)]) {
                return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Left)];
            }
            return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Straight)];

        case DirectionLabyrinth::Right:
            if (currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Back)]) {
                return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Back)];
            }
            if (currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Left)]) {
                return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Left)];
            }
            if (currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Straight)]) {
                return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Straight)];
            }
            return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Right)];

        case DirectionLabyrinth::Back:
        default:
            if (currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Left)]) {
                return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Left)];
            }
            if (currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Straight)]) {
                return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Straight)];
            }
            if (currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Right)]) {
                return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Right)];
            }
            return currentNode->neighbors[static_cast<int>(DirectionLabyrinth::Back)];
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
        Serial.print("CurrentID of Node Beeing Pushed: ");
        Serial.println(currentId);        
        dezibot.display.print("; ");
        dezibot.display.println(currentId);
        solutionStack.push(currentId);
    }

    if (solutionStack.top() != startNode->id || solutionStack.empty()) {
        throw std::runtime_error("No valid path found between start and goal nodes.");
    }
}

DirectionLabyrinth Graph::getDirection() {
    if (solutionStack.empty()) {
        throw std::runtime_error("Solution stack is empty. No further directions available.");
    }

    int nextNodeId = solutionStack.top();
    solutionStack.pop();

    for (int dir = 0; dir < currentNode->neighbors.size(); ++dir) {
        auto neighbor = currentNode->neighbors[dir];
        if (neighbor && neighbor->id == nextNodeId) {
            Serial.print("CurrentNodeType ");
            Serial.println(static_cast<int>(currentNode->crossingType));
            dezibot.display.print("; ");
            dezibot.display.println(static_cast<int>(currentNode->crossingType));
            currentNode = neighbor;
            return static_cast<DirectionLabyrinth>(dir);
        }
    }

    throw std::runtime_error("Next node from the solution stack not found in currentNode's neighbors.");
}