#include "blur_node.h"
#include <iostream>

namespace image_processor {

    BlurNode::BlurNode(const std::string& name, BlurType blurType, int kernelSize,
        double sigmaX, double sigmaY, double sigmaColor, double sigmaSpace)
        : BaseNode(name),
        m_blurType(blurType),
        m_kernelSize(validateKernelSize(kernelSize)),
        m_sigmaX(sigmaX),
        m_sigmaY(sigmaY),
        m_sigmaColor(sigmaColor),
        m_sigmaSpace(sigmaSpace) {
    }

    void BlurNode::process() {
        if (!isReady()) {
            std::cerr << "BlurNode::process: Node is not ready to process." << std::endl;
            return;
        }

        auto inputConnection = getInputConnection(0);
        if (inputConnection.first == nullptr) {
            std::cerr << "BlurNode::process: No valid input connection." << std::endl;
            return;
        }

        cv::Mat inputImage = inputConnection.first->getOutputValue(inputConnection.second);
        if (inputImage.empty()) {
            std::cerr << "BlurNode::process: Received empty image from input." << std::endl;
            return;
        }

        cv::Mat outputImage;

        // Apply the selected blur effect
        switch (m_blurType) {
        case BlurType::BOX:
            cv::blur(inputImage, outputImage, cv::Size(m_kernelSize, m_kernelSize));
            break;

        case BlurType::GAUSSIAN:
            cv::GaussianBlur(inputImage, outputImage, cv::Size(m_kernelSize, m_kernelSize), m_sigmaX, m_sigmaY);
            break;

        case BlurType::MEDIAN:
            cv::medianBlur(inputImage, outputImage, m_kernelSize);
            break;

        case BlurType::BILATERAL:
            cv::bilateralFilter(inputImage, outputImage, m_kernelSize, m_sigmaColor, m_sigmaSpace);
            break;

        default:
            std::cerr << "BlurNode::process: Unknown blur type." << std::endl;
            outputImage = inputImage.clone();
            break;
        }

        m_outputValues[0] = outputImage;
    }

    int BlurNode::getInputCount() const {
        return 1; // One input for the source image
    }

    int BlurNode::getOutputCount() const {
        return 1; // One output for the processed image
    }

    std::string BlurNode::getInputName(int index) const {
        if (index == 0) {
            return "Image";
        }
        return "";
    }

    std::string BlurNode::getOutputName(int index) const {
        if (index == 0) {
            return "Blurred Image";
        }
        return "";
    }

    void BlurNode::setBlurType(BlurType blurType) {
        m_blurType = blurType;
    }

    BlurType BlurNode::getBlurType() const {
        return m_blurType;
    }

    void BlurNode::setKernelSize(int kernelSize) {
        m_kernelSize = validateKernelSize(kernelSize);
    }

    int BlurNode::getKernelSize() const {
        return m_kernelSize;
    }

    void BlurNode::setSigmaX(double sigmaX) {
        m_sigmaX = sigmaX;
    }

    double BlurNode::getSigmaX() const {
        return m_sigmaX;
    }

    void BlurNode::setSigmaY(double sigmaY) {
        m_sigmaY = sigmaY;
    }

    double BlurNode::getSigmaY() const {
        return m_sigmaY;
    }

    void BlurNode::setSigmaColor(double sigmaColor) {
        m_sigmaColor = sigmaColor;
    }

    double BlurNode::getSigmaColor() const {
        return m_sigmaColor;
    }

    void BlurNode::setSigmaSpace(double sigmaSpace) {
        m_sigmaSpace = sigmaSpace;
    }

    double BlurNode::getSigmaSpace() const {
        return m_sigmaSpace;
    }

    int BlurNode::validateKernelSize(int size) {
        // Kernel size must be positive
        if (size <= 0) {
            size = 1;
        }

        // Kernel size must be odd for most blur operations
        if (size % 2 == 0) {
            size++;
        }

        return size;
    }

} // namespace image_processor
