#include "Graph.h"
#include <stdexcept>

Here is the description for the Graph::Graph() constructor, following the style guide and conventions previously outlined:

/**
 * @brief Constructs a Graph object and initializes it in Explorer Mode.
 * 
 * The constructor sets up the initial state of the Graph, starting with two nodes:
 * - A start node marked as the entry point of the labyrinth.
 * - A first node connected to the start node in the Straight direction.
 * 
 * The start node is set as explored and marked as the starting point. The initial
 * connection between the start node and the first node is added as an edge. The graph
 * is initialized in Explorer Mode, with `currentNode` pointing to the first node and
 * `previousNode` pointing to the start node. The `nextNodeId` counter is incremented
 * to ensure unique node identifiers.
 */
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

/**
 * @brief Sets the operating mode of the Graph and updates internal state accordingly.
 * 
 * This method switches the Graph between Explorer Mode and Solver Mode. In Solver Mode,
 * the Graph prepares for pathfinding by setting the current node to the second node 
 * (node with ID 2), as the first node (start node) does not require directional guidance.
 * The shortest path is calculated, and the first two nodes in the solution path are 
 * removed since:
 * - The first node is the start node, which does not require a direction.
 * - The second node is the current node, which is already reached.
 * 
 * @param mode The new mode to set for the Graph. 
 *             - `Mode::EXPLORER` to explore the labyrinth.
 *             - `Mode::SOLVER` to find and traverse the shortest path.
 */
void Graph::setMode(Mode mode) {
    currentMode = mode;
    if (mode == Mode::SOLVER){
        currentNode = nodes[2];
        findShortestPath();

        solutionStack.pop();
        solutionStack.pop();
    }
}

/**
 * @brief Retrieves the current operating mode of the Graph.
 * 
 * @return The current mode of the Graph, represented as a `Mode` enumeration.
 *         - `Mode::EXPLORER`: The Graph is in exploration mode.
 *         - `Mode::SOLVER`: The Graph is in pathfinding mode.
 */
Mode Graph::getMode() const {
    return currentMode;
}

/**
 * @brief Marks the current node as the goal node and switches to Solver Mode.
 * 
 * This method designates the current node as the goal node by setting its `isGoal` 
 * property to `true`. It also marks the node as explored and transitions the Graph 
 * into Solver Mode to compute the shortest path to the goal node.
 */
void Graph::setGoalNode() {
    currentNode->isGoal = true;
    currentNode->explored = true;
    setMode(Mode::SOLVER);
}

/**
 * @brief Creates and connects neighboring nodes for a given node based on its crossing type.
 * 
 * This method generates neighboring nodes for the specified node, depending on the 
 * `CrossingType`. New nodes are created and connected to the given node with appropriate 
 * directional edges. Each created node is stored in the `nodes` map with a unique ID.
 * 
 * - For `CrossingType::X`: Adds neighbors in the Right, Left, and Straight directions.
 * - For `CrossingType::T1`: Adds neighbors in the Right and Straight directions.
 * - For `CrossingType::T2`: Adds neighbors in the Left and Straight directions.
 * - For `CrossingType::T3`: Adds neighbors in the Right and Left directions.
 * - For `CrossingType::DEAD_END`: No neighbors are added.
 * 
 * @param node A shared pointer to the node for which neighbors will be created.
 *             Must not be null.
 * @param type The crossing type of the node, determining the neighbors to create.
 * 
 * @throws std::invalid_argument If the provided node is null.
 */
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

/**
 * @brief Assigns a neighbor to a node in a specified direction.
 * 
 * This method sets a neighboring node for the given node in the specified direction.
 * The neighbor is stored in the `neighbors` array of the node, indexed by the 
 * integer representation of the `Direction` enumeration.
 * 
 * @param node A shared pointer to the node for which the neighbor will be set.
 *             Must not be null.
 * @param neighbor A shared pointer to the neighboring node to assign.
 * @param direction The direction in which the neighbor is located relative to the node.
 * 
 * @throws std::invalid_argument If the provided `node` is null.
 */
void Graph::setNeighbor(std::shared_ptr<Node> node, std::shared_ptr<Node> neighbor, Direction direction) {
    if (!node) {
        throw std::invalid_argument("Node cannot be null.");
    }
    node->neighbors[static_cast<int>(direction)] = neighbor;
}

/**
 * @brief Adds a bidirectional edge between two nodes in the specified direction.
 * 
 * This method establishes a two-way connection between the given nodes. It sets the 
 * specified `nextNode` as a neighbor of the `fromNode` in the given direction, 
 * and vice versa, setting the `fromNode` as a neighbor of the `nextNode` in the 
 * opposite direction (Back).
 * 
 * @param fromNode A shared pointer to the starting node of the edge.
 *                 Must not be null.
 * @param nextNode A shared pointer to the destination node of the edge.
 *                 Must not be null.
 * @param direction The direction from `fromNode` to `nextNode` (e.g., Right, Left, Straight).
 * 
 * @throws std::invalid_argument If either of the provided nodes is null.
 */
void Graph::addEdge(std::shared_ptr<Node> fromNode, std::shared_ptr<Node> nextNode, Direction direction) {
    if (!fromNode || !nextNode) {
        throw std::invalid_argument("Nodes cannot be null.");
    }

    setNeighbor(fromNode, nextNode, direction);

    setNeighbor(nextNode, fromNode, Direction::Back);
}

/**
 * @brief Adds a crossing to the graph or gives directions based on the current mode.
 * 
 * This method handles the addition of a new crossing to the graph. The behavior differs
 * depending on the current mode of the graph:
 * - In Explorer Mode, the method calls `addCrossingExploring` to add a crossing based
 *   on the specified `CrossingType`.
 * - In Solver Mode, the method retrieves the direction of the shortest path using 
 *   `getDirection`.
 * 
 * @param crossingType The type of crossing to be added to the graph (e.g., X, T1, T2, T3, DEAD_END).
 * 
 * @return The direction for the dezibot.
 */
Direction Graph::addCrossing(CrossingType crossingType) {
    if (currentMode == Mode::EXPLORER)
    {
        return addCrossingExploring(crossingType);
    } else {
        return getDirection();
    }    
}

/**
 * @brief Adds a crossing while exploring the graph and determines the direction to proceed.
 * 
 * This method handles the addition of a crossing during the exploration phase. It first 
 * checks if the current node has been explored. If not, it sets the crossing type, creates 
 * neighbors based on the crossing type, and marks the current node as explored. Afterward, 
 * the method identifies the leftmost neighbor and determines the direction to that neighbor.
 * 
 * - If the crossing type is `DEAD_END`, the function returns `Direction::Back`.
 * - If the crossing type is `T1`, the function returns `Direction::Straight`.
 * - Otherwise, the function returns `Direction::Left`.
 * 
 * @param crossingType The type of crossing encountered (e.g., X, T1, T2, T3, DEAD_END).
 * 
 * @return The direction to proceed based on the crossing type and the leftmost neighbor.
 *         - `Direction::Back`: If the crossing type is `DEAD_END`.
 *         - `Direction::Straight`: If the crossing type is `T1`.
 *         - `Direction::Left`: For all other crossing types.
 * 
 * @throws std::invalid_argument If the current node is null.
 */
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

/**
 * @brief Retrieves the leftmost neighbor of the current node based on the direction 
 *        from which the current node is being entered.
 * 
 * This method determines the direction of the previous node relative to the current node 
 * and then finds the leftmost neighbor of the current node in the appropriate direction. 
 * The leftmost neighbor is selected based on the direction from which the current node 
 * is being entered (e.g., Left, Straight, Right, or Back).
 * 
 * The method follows this order to determine the leftmost neighbor:
 * - If entering from Left, it checks Straight, then Right, then Back, and finally Left.
 * - If entering from Straight, it checks Right, then Back, then Left, and finally Straight.
 * - If entering from Right, it checks Back, then Left, then Straight, and finally Right.
 * - If entering from Back (or any undefined direction), it checks Left, then Straight, then Right, and finally Back.
 * 
 * @param currentNode A shared pointer to the current node. Must not be null.
 * @param previousNode A shared pointer to the previous node. Must not be null.
 * 
 * @return A shared pointer to the leftmost neighbor of the current node based on the 
 *         direction of entry.
 * 
 * @throws std::invalid_argument If either the `currentNode` or `previousNode` is null.
 */
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

/**
 * @brief Finds the shortest path from the start node to the goal node using Breadth-First Search (BFS).
 * 
 * This method uses the BFS algorithm to compute the shortest path in the graph from the start node 
 * to the goal node. It traverses the graph level by level, marking nodes as visited, and tracks the 
 * parent node for each visited node to reconstruct the shortest path once the goal node is reached.
 * The computed path is stored in the `solutionStack` in reverse order, starting from the goal node.
 * 
 * If the graph is empty, or if the start or goal node is missing, an exception will be thrown.
 * After the BFS search, the function reconstructs the path and ensures that the path is valid by 
 * checking that it starts with the start node and ends with the goal node.
 * 
 * @throws std::runtime_error If the graph is empty, or if the start or goal node is not found.
 * @throws std::runtime_error If no valid path exists between the start and goal nodes.
 */
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

/**
 * @brief Retrieves the next direction to move in the shortest path from the current node.
 * 
 * This method pops the next node ID from the solution stack and searches for it in the current 
 * node's neighbors. If the next node is found, the current node is updated to the neighbor, 
 * and the direction to the neighbor is returned. The direction corresponds to the direction 
 * to move toward the next node in the solution path.
 * 
 * If the solution stack is empty or if the next node is not found in the current node's neighbors, 
 * a runtime error is thrown.
 * 
 * @return Direction The direction to move towards the next node in the solution path.
 * 
 * @throws std::runtime_error If the solution stack is empty or the next node is not found in 
 * the current node's neighbors.
 */
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