/**
 * @file Graph.h
 * @author Max Mager
 * @brief Header file for the Graph class, which provides guidance through a labyrinth.
 * 
 * The Graph of a labyrinth is created by adding crossings to the class in Explorer Mode.
 * In Solver Mode, the class calculates and returns the direction to take the shortest path 
 * through the labyrinth. It is best to set the goal node, as this automatically switches 
 * the class to Solver Mode and guarantees a valid goal node.
 * 
 * @version 1.0.0
 * @date 2024-12-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <queue>
#include <stack>
#include <vector>
#include <unordered_map>
#include <memory>
#include <array>
#include <optional>
#include <limits>
#include "CommonDefinitions.h"

struct Node;

enum class Mode {
    EXPLORER,
    SOLVER
};

enum class DirectionLabyrinth {
    Straight = 0,
    Back,
    Left,
    Right
};

struct Node {
    int id; 
    bool explored;
    CrossingType crossingType;
    std::array<std::shared_ptr<Node>, 4> neighbors; // Fixed neighbors for Straight, Back, Left, Right
    bool isStart;
    bool isGoal;

    Node(int id, CrossingType type)
        : id(id), explored(false), crossingType(type), neighbors{nullptr, nullptr, nullptr, nullptr}, isStart(false), isGoal(false) {}
};

/**
 * @class Graph
 * @brief A class to map out and solve a labyrinth.
 */
class Graph {
public:
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
    Graph();

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
    void setMode(Mode mode);
    /**
     * @brief Retrieves the current operating mode of the Graph.
     * 
     * @return The current mode of the Graph, represented as a `Mode` enumeration.
     *         - `Mode::EXPLORER`: The Graph is in exploration mode.
     *         - `Mode::SOLVER`: The Graph is in pathfinding mode.
     */
    Mode getMode() const;
    /**
     * @brief Marks the current node as the goal node and switches to Solver Mode.
     * 
     * This method designates the current node as the goal node by setting its `isGoal` 
     * property to `true`. It also marks the node as explored and transitions the Graph 
     * into Solver Mode to compute the shortest path to the goal node.
     */
    void setGoalNode();

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
    DirectionLabyrinth addCrossing(CrossingType crossingType);

private:
    int nextNodeId;
    Mode currentMode;
    std::shared_ptr<Node> currentNode;
    std::shared_ptr<Node> previousNode;
    std::unordered_map<int, std::shared_ptr<Node>> nodes;

    std::stack<int> solutionStack;

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
    void createNeighbours(std::shared_ptr<Node> node, CrossingType type);
    
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
    void setNeighbor(std::shared_ptr<Node> node, std::shared_ptr<Node> neighbor, DirectionLabyrinth direction);
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
    void addEdge(std::shared_ptr<Node> fromNode, std::shared_ptr<Node> nextNode, DirectionLabyrinth direction);
    std::shared_ptr<Node> getLeftMostNeighbor(const std::shared_ptr<Node>& node);
    
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
    DirectionLabyrinth addCrossingExploring(CrossingType crossingType);
    
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
    std::shared_ptr<Node> getLeftMostNeighborPrev(const std::shared_ptr<Node>& currentNode, const std::shared_ptr<Node>& previousNode);
    
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
    void findShortestPath();
    
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
     * @return DirectionLabyrinth The direction to move towards the next node in the solution path.
     * 
     * @throws std::runtime_error If the solution stack is empty or the next node is not found in 
     * the current node's neighbors.
     */
    DirectionLabyrinth getDirection();

};

#endif // GRAPH_H
