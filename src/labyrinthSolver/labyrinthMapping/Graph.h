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

struct Node;

enum class Mode {
    EXPLORER,
    SOLVER
};

enum class Direction {
    Straight = 0,
    Back,
    Left,
    Right
};

enum class CrossingType {
    X,
    T1,
    T2,
    T3,
    DEAD_END
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
    Graph();

    void setMode(Mode mode);
    Mode getMode() const;
    void setGoalNode();

    Direction addCrossing(CrossingType crossingType);

private:
    int nextNodeId;
    Mode currentMode;
    std::shared_ptr<Node> currentNode;
    std::shared_ptr<Node> previousNode;
    std::unordered_map<int, std::shared_ptr<Node>> nodes;

    std::stack<int> solutionStack;

    void createNeighbours(std::shared_ptr<Node> node, CrossingType type);
    void setNeighbor(std::shared_ptr<Node> node, std::shared_ptr<Node> neighbor, Direction direction);
    void addEdge(std::shared_ptr<Node> fromNode, std::shared_ptr<Node> nextNode, Direction direction);
    std::shared_ptr<Node> getLeftMostNeighbor(const std::shared_ptr<Node>& node);
    Direction addCrossingExploring(CrossingType crossingType);
    

    std::shared_ptr<Node> getLeftMostNeighborPrev(const std::shared_ptr<Node>& currentNode, const std::shared_ptr<Node>& previousNode);
    void findShortestPath();
    Direction getDirection();

};

#endif // GRAPH_H
