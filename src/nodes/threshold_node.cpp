#include "threshold_node.h"
#include <iostream>

namespace image_processor {

    ThresholdNode::ThresholdNode(const std::string& name, ThresholdType thresholdType,
        double threshold, double maxValue, int blockSize, double C)
        : BaseNode(name),
        m_thresholdType(thresholdType),
        m_threshold(threshold),
        m_maxValue(maxValue),
        m_blockSize(validateBlockSize(blockSize)),
        m_C(C) {
    }

    void ThresholdNode::process() {
        if (!isReady()) {
            std::cerr << "ThresholdNode::process: Node is not ready to process." << std::endl;
            return;
        }

        auto inputConnection = getInputConnection(0);
        if (inputConnection.first == nullptr) {
            std::cerr << "ThresholdNode::process: No valid input connection." << std::endl;
            return;
        }

        cv::Mat inputImage = inputConnection.first->getOutputValue(inputConnection.second);
        if (inputImage.empty()) {
            std::cerr << "ThresholdNode::process: Received empty image from input." << std::endl;
            return;
        }

        cv::Mat outputImage;
        cv::Mat grayImage;

        // Convert to grayscale if the image has multiple channels
        if (inputImage.channels() > 1) {
            cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
        }
        else {
            grayImage = inputImage.clone();
        }

        // Apply the selected thresholding method
        switch (m_thresholdType) {
        case ThresholdType::BINARY:
            cv::threshold(grayImage, outputImage, m_threshold, m_maxValue, cv::THRESH_BINARY);
            break;

        case ThresholdType::BINARY_INV:
            cv::threshold(grayImage, outputImage, m_threshold, m_maxValue, cv::THRESH_BINARY_INV);
            break;

        case ThresholdType::TRUNC:
            cv::threshold(grayImage, outputImage, m_threshold, m_maxValue, cv::THRESH_TRUNC);
            break;

        case ThresholdType::TOZERO:
            cv::threshold(grayImage, outputImage, m_threshold, m_maxValue, cv::THRESH_TOZERO);
            break;

        case ThresholdType::TOZERO_INV:
            cv::threshold(grayImage, outputImage, m_threshold, m_maxValue, cv::THRESH_TOZERO_INV);
            break;

        case ThresholdType::OTSU:
            cv::threshold(grayImage, outputImage, m_threshold, m_maxValue, cv::THRESH_BINARY | cv::THRESH_OTSU);
            break;

        case ThresholdType::ADAPTIVE_MEAN:
            cv::adaptiveThreshold(grayImage, outputImage, m_maxValue, cv::ADAPTIVE_THRESH_MEAN_C,
                cv::THRESH_BINARY, m_blockSize, m_C);
            break;

        case ThresholdType::ADAPTIVE_GAUSSIAN:
            cv::adaptiveThreshold(grayImage, outputImage, m_maxValue, cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                cv::THRESH_BINARY, m_blockSize, m_C);
            break;

        default:
            std::cerr << "ThresholdNode::process: Unknown threshold type." << std::endl;
            outputImage = grayImage.clone();
            break;
        }

        m_outputValues[0] = outputImage;
    }

    int ThresholdNode::getInputCount() const {
        return 1; // One input for the source image
    }

    int ThresholdNode::getOutputCount() const {
        return 1; // One output for the processed image
    }

    std::string ThresholdNode::getInputName(int index) const {
        if (index == 0) {
            return "Image";
        }
        return "";
    }

    std::string ThresholdNode::getOutputName(int index) const {
        if (index == 0) {
            return "Thresholded Image";
        }
        return "";
    }

    void ThresholdNode::setThresholdType(ThresholdType thresholdType) {
        m_thresholdType = thresholdType;
    }

    ThresholdType ThresholdNode::getThresholdType() const {
        return m_thresholdType;
    }

    void ThresholdNode::setThreshold(double threshold) {
        m_threshold = threshold;
    }

    double ThresholdNode::getThreshold() const {
        return m_threshold;
    }

    void ThresholdNode::setMaxValue(double maxValue) {
        m_maxValue = maxValue;
    }

    double ThresholdNode::getMaxValue() const {
        return m_maxValue;
    }

    void ThresholdNode::setBlockSize(int blockSize) {
        m_blockSize = validateBlockSize(blockSize);
    }

    int ThresholdNode::getBlockSize() const {
        return m_blockSize;
    }

    void ThresholdNode::setC(double C) {
        m_C = C;
    }

    double ThresholdNode::getC() const {
        return m_C;
    }

    int ThresholdNode::validateBlockSize(int size) {
        // Block size must be positive
        if (size <= 0) {
            size = 3;
        }

        // Block size must be odd for adaptive methods
        if (size % 2 == 0) {
            size++;
        }

        return size;
    }

} // namespace image_processor