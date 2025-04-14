#include "node_graph.h"
#include "input_node.h"
#include "output_node.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <typeinfo>

namespace image_processor {

    NodeGraph::NodeGraph() {
    }

    NodeGraph::~NodeGraph() {
        clear();
    }

    bool NodeGraph::addNode(BaseNode* node) {
        if (!node) {
            std::cerr << "NodeGraph::addNode: Cannot add null node." << std::endl;
            return false;
        }

        if (containsNode(node->getId())) {
            std::cerr << "NodeGraph::addNode: Node with ID " << node->getId() << " already exists." << std::endl;
            return false;
        }

        m_nodes.push_back(node);
        return true;
    }

    bool NodeGraph::removeNode(int nodeId) {
        auto it = std::find_if(m_nodes.begin(), m_nodes.end(),
            [nodeId](const BaseNode* node) { return node->getId() == nodeId; });

        if (it == m_nodes.end()) {
            std::cerr << "NodeGraph::removeNode: Node with ID " << nodeId << " not found." << std::endl;
            return false;
        }

        // Disconnect this node from all other nodes
        BaseNode* node = *it;

        // Disconnect all outputs
        for (int outputIndex = 0; outputIndex < node->getOutputCount(); ++outputIndex) {
            auto connections = node->getConnectedNodes(outputIndex);
            for (const auto& connection : connections) {
                BaseNode* targetNode = connection.first;
                int inputIndex = connection.second;
                node->disconnectOutput(outputIndex, targetNode, inputIndex);
            }
        }

        // Disconnect all inputs
        for (int inputIndex = 0; inputIndex < node->getInputCount(); ++inputIndex) {
            auto connection = node->getInputConnection(inputIndex);
            if (connection.first) {
                BaseNode* sourceNode = connection.first;
                int outputIndex = connection.second;
                sourceNode->disconnectOutput(outputIndex, node, inputIndex);
            }
        }

        // Remove the node from the graph
        delete node;
        m_nodes.erase(it);

        return true;
    }

    BaseNode* NodeGraph::getNode(int nodeId) const {
        auto it = std::find_if(m_nodes.begin(), m_nodes.end(),
            [nodeId](const BaseNode* node) { return node->getId() == nodeId; });

        if (it == m_nodes.end()) {
            return nullptr;
        }

        return *it;
    }

    std::vector<BaseNode*> NodeGraph::getAllNodes() const {
        return m_nodes;
    }

    bool NodeGraph::connectNodes(int sourceNodeId, int outputIndex, int targetNodeId, int inputIndex) {
        BaseNode* sourceNode = getNode(sourceNodeId);
        BaseNode* targetNode = getNode(targetNodeId);

        if (!sourceNode || !targetNode) {
            std::cerr << "NodeGraph::connectNodes: Source or target node not found." << std::endl;
            return false;
        }

        if (outputIndex < 0 || outputIndex >= sourceNode->getOutputCount() ||
            inputIndex < 0 || inputIndex >= targetNode->getInputCount()) {
            std::cerr << "NodeGraph::connectNodes: Invalid output or input index." << std::endl;
            return false;
        }

        // Check if the input is already connected
        auto existingConnection = targetNode->getInputConnection(inputIndex);
        if (existingConnection.first) {
            std::cerr << "NodeGraph::connectNodes: Input already connected." << std::endl;
            return false;
        }

        // Connect the nodes
        bool success = sourceNode->connectOutput(outputIndex, targetNode, inputIndex);

        // Check if this connection would create a cycle
        if (success && containsCycles()) {
            // If it would create a cycle, disconnect and return false
            sourceNode->disconnectOutput(outputIndex, targetNode, inputIndex);
            std::cerr << "NodeGraph::connectNodes: Cannot create cycle in the graph." << std::endl;
            return false;
        }

        return success;
    }

    bool NodeGraph::disconnectNodes(int sourceNodeId, int outputIndex, int targetNodeId, int inputIndex) {
        BaseNode* sourceNode = getNode(sourceNodeId);
        BaseNode* targetNode = getNode(targetNodeId);

        if (!sourceNode || !targetNode) {
            std::cerr << "NodeGraph::disconnectNodes: Source or target node not found." << std::endl;
            return false;
        }

        return sourceNode->disconnectOutput(outputIndex, targetNode, inputIndex);
    }

    void NodeGraph::processGraph() {
        // Get the processing order
        std::vector<BaseNode*> processingOrder = getProcessingOrder();

        // Process each node in order
        for (BaseNode* node : processingOrder) {
            if (node->isReady()) {
                node->process();
            }
            else {
                std::cerr << "NodeGraph::processGraph: Node " << node->getName() << " (ID: " << node->getId() << ") is not ready to process." << std::endl;
            }
        }
    }

    void NodeGraph::clear() {
        // Delete all nodes
        for (BaseNode* node : m_nodes) {
            delete node;
        }

        m_nodes.clear();
    }

    bool NodeGraph::containsNode(int nodeId) const {
        return std::any_of(m_nodes.begin(), m_nodes.end(),
            [nodeId](const BaseNode* node) { return node->getId() == nodeId; });
    }

    size_t NodeGraph::getNodeCount() const {
        return m_nodes.size();
    }

    std::vector<BaseNode*> NodeGraph::findNodesByType(const std::string& typeName) const {
        std::vector<BaseNode*> result;

        for (BaseNode* node : m_nodes) {
            // Get the type name of the node
            const std::type_info& typeInfo = typeid(*node);
            std::string nodeTypeName = typeInfo.name();

            // Check if the type name contains the search string
            if (nodeTypeName.find(typeName) != std::string::npos) {
                result.push_back(node);
            }
        }

        return result;
    }

    std::vector<BaseNode*> NodeGraph::findNodesByName(const std::string& name) const {
        std::vector<BaseNode*> result;

        for (BaseNode* node : m_nodes) {
            if (node->getName().find(name) != std::string::npos) {
                result.push_back(node);
            }
        }

        return result;
    }

    std::vector<BaseNode*> NodeGraph::getInputNodes() const {
        std::vector<BaseNode*> result;

        for (BaseNode* node : m_nodes) {
            if (dynamic_cast<InputNode*>(node)) {
                result.push_back(node);
            }
        }

        return result;
    }

    std::vector<BaseNode*> NodeGraph::getOutputNodes() const {
        std::vector<BaseNode*> result;

        for (BaseNode* node : m_nodes) {
            if (dynamic_cast<OutputNode*>(node)) {
                result.push_back(node);
            }
        }

        return result;
    }

    bool NodeGraph::validateGraph() const {
        // Check for cycles
        if (containsCycles()) {
            std::cerr << "NodeGraph::validateGraph: Graph contains cycles." << std::endl;
            return false;
        }

        // Check that all required inputs are connected
        for (BaseNode* node : m_nodes) {
            for (int i = 0; i < node->getInputCount(); ++i) {
                auto connection = node->getInputConnection(i);
                if (!connection.first) {
                    std::cerr << "NodeGraph::validateGraph: Node " << node->getName() << " (ID: " << node->getId() << ") has unconnected input " << i << "." << std::endl;
                    return false;
                }
            }
        }

        return true;
    }

    std::vector<BaseNode*> NodeGraph::getProcessingOrder() const {
        std::vector<BaseNode*> result;
        std::unordered_map<int, bool> processedNodes;

        // Initialize all nodes as unprocessed
        for (BaseNode* node : m_nodes) {
            processedNodes[node->getId()] = false;
        }

        // Process nodes until all are processed
        while (result.size() < m_nodes.size()) {
            bool foundNode = false;

            for (BaseNode* node : m_nodes) {
                int nodeId = node->getId();

                // Skip already processed nodes
                if (processedNodes[nodeId]) {
                    continue;
                }

                // Check if all dependencies are processed
                if (!hasUnprocessedDependencies(node, processedNodes)) {
                    // Add the node to the result and mark it as processed
                    result.push_back(node);
                    processedNodes[nodeId] = true;
                    foundNode = true;
                }
            }

            // If no node was found in this iteration, there might be a cycle
            if (!foundNode) {
                std::cerr << "NodeGraph::getProcessingOrder: Could not determine processing order. Graph might contain cycles." << std::endl;
                break;
            }
        }

        return result;
    }

    bool NodeGraph::hasUnprocessedDependencies(BaseNode* node, const std::unordered_map<int, bool>& processedNodes) const {
        for (int i = 0; i < node->getInputCount(); ++i) {
            auto connection = node->getInputConnection(i);
            if (connection.first) {
                int sourceNodeId = connection.first->getId();
                if (!processedNodes.at(sourceNodeId)) {
                    return true;
                }
            }
        }

        return false;
    }

    bool NodeGraph::containsCycles() const {
        std::unordered_map<int, bool> visited;
        std::unordered_map<int, bool> recursionStack;

        // Initialize all nodes as not visited
        for (BaseNode* node : m_nodes) {
            visited[node->getId()] = false;
            recursionStack[node->getId()] = false;
        }

        // Check for cycles starting from each node
        for (BaseNode* node : m_nodes) {
            int nodeId = node->getId();
            if (!visited[nodeId]) {
                if (detectCycle(nodeId, visited, recursionStack)) {
                    return true;
                }
            }
        }

        return false;
    }

    bool NodeGraph::detectCycle(int nodeId, std::unordered_map<int, bool>& visited, std::unordered_map<int, bool>& recursionStack) const {
        BaseNode* node = getNode(nodeId);
        if (!node) {
            return false;
        }

        visited[nodeId] = true;
        recursionStack[nodeId] = true;

        // Check all outgoing connections
        for (int i = 0; i < node->getOutputCount(); ++i) {
            auto connections = node->getConnectedNodes(i);
            for (const auto& connection : connections) {
                BaseNode* targetNode = connection.first;
                int targetNodeId = targetNode->getId();

                // If the target node is not visited, recursively check it
                if (!visited[targetNodeId]) {
                    if (detectCycle(targetNodeId, visited, recursionStack)) {
                        return true;
                    }
                }
                // If the target node is in the recursion stack, there's a cycle
                else if (recursionStack[targetNodeId]) {
                    return true;
                }
            }
        }

        // Remove the node from the recursion stack
        recursionStack[nodeId] = false;
        return false;
    }

}
