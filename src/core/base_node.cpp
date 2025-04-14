#include "base_node.h"
#include <stdexcept>

namespace image_processor {

    // Initialize static member for unique ID generation
    int BaseNode::s_nextId = 0;

    BaseNode::BaseNode(const std::string& name)
        : m_name(name), m_id(s_nextId++) {}

    std::string BaseNode::getName() const {
        return m_name;
    }

    void BaseNode::setName(const std::string& name) {
        m_name = name;
    }

    int BaseNode::getId() const {
        return m_id;
    }

    bool BaseNode::connectOutput(int outputIndex, BaseNode* targetNode, int inputIndex) {
        if (outputIndex < 0 || outputIndex >= getOutputCount() ||
            inputIndex < 0 || inputIndex >= targetNode->getInputCount()) {
            return false;
        }

        // Check if the input is already connected
        if (targetNode->getInputConnection(inputIndex).first != nullptr) {
            return false;
        }

        m_outputs[outputIndex].push_back(std::make_pair(targetNode, inputIndex));
        targetNode->m_inputs[inputIndex] = std::make_pair(this, outputIndex);
        return true;
    }

    bool BaseNode::disconnectOutput(int outputIndex, BaseNode* targetNode, int inputIndex) {
        if (m_outputs.find(outputIndex) == m_outputs.end()) {
            return false;
        }

        auto& connections = m_outputs[outputIndex];
        auto it = std::find(connections.begin(), connections.end(), std::make_pair(targetNode, inputIndex));
        if (it == connections.end()) {
            return false;
        }

        connections.erase(it);
        targetNode->m_inputs.erase(inputIndex);
        return true;
    }

    std::vector<std::pair<BaseNode*, int>> BaseNode::getConnectedNodes(int outputIndex) const {
        if (m_outputs.find(outputIndex) != m_outputs.end()) {
            return m_outputs.at(outputIndex);
        }
        return {};
    }

    std::pair<BaseNode*, int> BaseNode::getInputConnection(int inputIndex) const {
        if (m_inputs.find(inputIndex) != m_inputs.end()) {
            return m_inputs.at(inputIndex);
        }
        return { nullptr, -1 };
    }

    bool BaseNode::isReady() const {
        for (int i = 0; i < getInputCount(); ++i) {
            if (getInputConnection(i).first == nullptr) {
                return false;
            }
        }
        return true;
    }

    bool BaseNode::setInputValue(int inputIndex, const cv::Mat& value) {
        return false;
    }

    cv::Mat BaseNode::getOutputValue(int outputIndex) const {
        if (m_outputValues.find(outputIndex) != m_outputValues.end()) {
            return m_outputValues.at(outputIndex);
        }
        return cv::Mat();
    }

}