#include "input_node.h"
#include <iostream>

namespace image_processor {

    InputNode::InputNode(const std::string& name)
        : BaseNode(name), m_currentImagePath("") {
    }

    void InputNode::process() {
        if (!hasValidImage()) {
            std::cerr << "InputNode::process: No valid image available." << std::endl;
            return;
        }

        // Simply pass the input image to the output
        m_outputValues[0] = m_image.clone();
    }

    int InputNode::getInputCount() const {
        return 0; // Input nodes have no inputs
    }

    int InputNode::getOutputCount() const {
        return 1; // Input nodes have one output (the image)
    }

    std::string InputNode::getInputName(int index) const {
        return ""; // No inputs
    }

    std::string InputNode::getOutputName(int index) const {
        if (index == 0) {
            return "Image";
        }
        return "";
    }

    bool InputNode::loadImage(const std::string& filePath) {
        cv::Mat loadedImage = cv::imread(filePath, cv::IMREAD_UNCHANGED);
        if (loadedImage.empty()) {
            std::cerr << "InputNode::loadImage: Failed to load image from " << filePath << std::endl;
            return false;
        }

        m_image = loadedImage;
        m_currentImagePath = filePath;

        // Automatically process the node to update the output
        process();

        return true;
    }

    void InputNode::setImage(const cv::Mat& image) {
        if (image.empty()) {
            std::cerr << "InputNode::setImage: Attempted to set empty image." << std::endl;
            return;
        }

        m_image = image.clone();
        m_currentImagePath = ""; // Direct image input, no file path

        // Automatically process the node to update the output
        process();
    }

    cv::Mat InputNode::getImage() const {
        return m_image;
    }

    bool InputNode::hasValidImage() const {
        return !m_image.empty();
    }

    bool InputNode::isReady() const {
        // An input node is ready if it has a valid image
        return hasValidImage();
    }

}
