#pragma once

#include "base_node.h"
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace image_processor {

    /**
     * @brief Class for managing a graph of connected processing nodes
     *
     * This class manages a collection of nodes and their connections,
     * allowing for the creation of complex image processing pipelines.
     */
    class NodeGraph {
    public:
        /**
         * @brief Constructor
         */
        NodeGraph();

        /**
         * @brief Destructor
         */
        ~NodeGraph();

        /**
         * @brief Add a node to the graph
         * @param node Pointer to the node to add
         * @return True if the node was added successfully, false otherwise
         */
        bool addNode(BaseNode* node);

        /**
         * @brief Remove a node from the graph
         * @param nodeId The ID of the node to remove
         * @return True if the node was removed successfully, false otherwise
         */
        bool removeNode(int nodeId);

        /**
         * @brief Get a node by its ID
         * @param nodeId The ID of the node to get
         * @return Pointer to the node, or nullptr if not found
         */
        BaseNode* getNode(int nodeId) const;

        /**
         * @brief Get all nodes in the graph
         * @return Vector of pointers to all nodes
         */
        std::vector<BaseNode*> getAllNodes() const;

        /**
         * @brief Connect two nodes
         * @param sourceNodeId The ID of the source node
         * @param outputIndex The index of the output on the source node
         * @param targetNodeId The ID of the target node
         * @param inputIndex The index of the input on the target node
         * @return True if the connection was made successfully, false otherwise
         */
        bool connectNodes(int sourceNodeId, int outputIndex, int targetNodeId, int inputIndex);

        /**
         * @brief Disconnect two nodes
         * @param sourceNodeId The ID of the source node
         * @param outputIndex The index of the output on the source node
         * @param targetNodeId The ID of the target node
         * @param inputIndex The index of the input on the target node
         * @return True if the connection was removed successfully, false otherwise
         */
        bool disconnectNodes(int sourceNodeId, int outputIndex, int targetNodeId, int inputIndex);

        /**
         * @brief Process the entire graph
         *
         * This method processes all nodes in the graph in the correct order,
         * starting from input nodes and following the connections to output nodes.
         */
        void processGraph();

        /**
         * @brief Clear the graph
         *
         * Removes all nodes and connections from the graph.
         */
        void clear();

        /**
         * @brief Check if the graph contains a node
         * @param nodeId The ID of the node to check
         * @return True if the graph contains the node, false otherwise
         */
        bool containsNode(int nodeId) const;

        /**
         * @brief Get the number of nodes in the graph
         * @return The number of nodes
         */
        size_t getNodeCount() const;

        /**
         * @brief Find nodes by type
         * @param typeName The name of the node type to find
         * @return Vector of pointers to nodes of the specified type
         */
        std::vector<BaseNode*> findNodesByType(const std::string& typeName) const;

        /**
         * @brief Find nodes by name
         * @param name The name to search for
         * @return Vector of pointers to nodes with the specified name
         */
        std::vector<BaseNode*> findNodesByName(const std::string& name) const;

        /**
         * @brief Get the input nodes in the graph
         * @return Vector of pointers to input nodes
         */
        std::vector<BaseNode*> getInputNodes() const;

        /**
         * @brief Get the output nodes in the graph
         * @return Vector of pointers to output nodes
         */
        std::vector<BaseNode*> getOutputNodes() const;

        /**
         * @brief Validate the graph
         *
         * Checks if the graph is valid (no cycles, all required inputs connected, etc.)
         *
         * @return True if the graph is valid, false otherwise
         */
        bool validateGraph() const;

    private:
        std::vector<BaseNode*> m_nodes;  // All nodes in the graph

        /**
         * @brief Get the processing order for the nodes
         *
         * Determines the order in which nodes should be processed based on their connections.
         *
         * @return Vector of nodes in processing order
         */
        std::vector<BaseNode*> getProcessingOrder() const;

        /**
         * @brief Check if a node has unprocessed dependencies
         * @param node The node to check
         * @param processedNodes Set of nodes that have already been processed
         * @return True if the node has unprocessed dependencies, false otherwise
         */
        bool hasUnprocessedDependencies(BaseNode* node, const std::unordered_map<int, bool>& processedNodes) const;

        /**
         * @brief Check if the graph contains cycles
         * @return True if the graph contains cycles, false otherwise
         */
        bool containsCycles() const;

        /**
         * @brief Detect cycles using depth-first search
         * @param nodeId The ID of the current node
         * @param visited Map of visited nodes
         * @param recursionStack Map of nodes in the current recursion stack
         * @return True if a cycle was detected, false otherwise
         */
        bool detectCycle(int nodeId, std::unordered_map<int, bool>& visited, std::unordered_map<int, bool>& recursionStack) const;
    };

}
