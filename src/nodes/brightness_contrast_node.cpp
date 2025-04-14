#include "brightness_contrast_node.h"
#include <iostream>

namespace image_processor {

    BrightnessContrastNode::BrightnessContrastNode(const std::string& name, float alpha, float beta)
        : BaseNode(name), m_alpha(alpha), m_beta(beta) {
    }

    void BrightnessContrastNode::process() {
        if (!isReady()) {
            std::cerr << "BrightnessContrastNode::process: Node is not ready to process." << std::endl;
            return;
        }

        auto inputConnection = getInputConnection(0);
        if (inputConnection.first == nullptr) {
            std::cerr << "BrightnessContrastNode::process: No valid input connection." << std::endl;
            return;
        }

        cv::Mat inputImage = inputConnection.first->getOutputValue(inputConnection.second);
        if (inputImage.empty()) {
            std::cerr << "BrightnessContrastNode::process: Received empty image from input." << std::endl;
            return;
        }

        cv::Mat outputImage;

        // Apply brightness and contrast adjustment
        // Formula: output = alpha * input + beta
        inputImage.convertTo(outputImage, -1, m_alpha, m_beta);

        m_outputValues[0] = outputImage;
    }

    int BrightnessContrastNode::getInputCount() const {
        return 1; // One input for the source image
    }

    int BrightnessContrastNode::getOutputCount() const {
        return 1; // One output for the processed image
    }

    std::string BrightnessContrastNode::getInputName(int index) const {
        if (index == 0) {
            return "Image";
        }
        return "";
    }

    std::string BrightnessContrastNode::getOutputName(int index) const {
        if (index == 0) {
            return "Adjusted Image";
        }
        return "";
    }

    void BrightnessContrastNode::setContrast(float alpha) {
        m_alpha = alpha;
    }

    float BrightnessContrastNode::getContrast() const {
        return m_alpha;
    }

    void BrightnessContrastNode::setBrightness(float beta) {
        m_beta = beta;
    }

    float BrightnessContrastNode::getBrightness() const {
        return m_beta;
    }

} // namespace image_processor
