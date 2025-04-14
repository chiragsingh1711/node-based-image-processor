#include "output_node.h"
#include <iostream>

namespace image_processor {

    OutputNode::OutputNode(const std::string& name)
        : BaseNode(name) {
    }

    void OutputNode::process() {
        if (!isReady()) {
            std::cerr << "OutputNode::process: Node is not ready to process." << std::endl;
            return;
        }

        auto inputConnection = getInputConnection(0);
        if (inputConnection.first == nullptr) {
            std::cerr << "OutputNode::process: No valid input connection." << std::endl;
            return;
        }

        cv::Mat inputImage = inputConnection.first->getOutputValue(inputConnection.second);
        if (inputImage.empty()) {
            std::cerr << "OutputNode::process: Received empty image from input." << std::endl;
            return;
        }

        m_image = inputImage.clone();
    }

    int OutputNode::getInputCount() const {
        return 1; // Output nodes have one input
    }

    int OutputNode::getOutputCount() const {
        return 0; // Output nodes have no outputs
    }

    std::string OutputNode::getInputName(int index) const {
        if (index == 0) {
            return "Image";
        }
        return "";
    }

    std::string OutputNode::getOutputName(int index) const {
        return ""; // No outputs
    }

    bool OutputNode::saveImage(const std::string& filePath) const {
        if (!hasValidImage()) {
            std::cerr << "OutputNode::saveImage: No valid image to save." << std::endl;
            return false;
        }

        bool success = cv::imwrite(filePath, m_image);
        if (!success) {
            std::cerr << "OutputNode::saveImage: Failed to save image to " << filePath << std::endl;
        }

        return success;
    }

    cv::Mat OutputNode::getImage() const {
        return m_image;
    }

    bool OutputNode::hasValidImage() const {
        return !m_image.empty();
    }

    bool OutputNode::isReady() const {
        // An output node is ready if it has a valid input connection
        return getInputConnection(0).first != nullptr;
    }

}
