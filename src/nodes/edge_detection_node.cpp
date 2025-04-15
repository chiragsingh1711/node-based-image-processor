#include "edge_detection_node.h"
#include <iostream>

namespace image_processor {

    EdgeDetectionNode::EdgeDetectionNode(const std::string& name, EdgeDetectionType edgeType,
        double threshold1, double threshold2, int apertureSize, bool L2gradient)
        : BaseNode(name),
        m_edgeType(edgeType),
        m_threshold1(threshold1),
        m_threshold2(threshold2),
        m_apertureSize(validateApertureSize(apertureSize)),
        m_L2gradient(L2gradient) {
    }

    void EdgeDetectionNode::process() {
        if (!isReady()) {
            std::cerr << "EdgeDetectionNode::process: Node is not ready to process." << std::endl;
            return;
        }

        auto inputConnection = getInputConnection(0);
        if (inputConnection.first == nullptr) {
            std::cerr << "EdgeDetectionNode::process: No valid input connection." << std::endl;
            return;
        }

        cv::Mat inputImage = inputConnection.first->getOutputValue(inputConnection.second);
        if (inputImage.empty()) {
            std::cerr << "EdgeDetectionNode::process: Received empty image from input." << std::endl;
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

        // Apply the selected edge detection method
        switch (m_edgeType) {
        case EdgeDetectionType::SOBEL: {
            cv::Mat gradX, gradY;
            cv::Sobel(grayImage, gradX, CV_16S, 1, 0, m_apertureSize);
            cv::Sobel(grayImage, gradY, CV_16S, 0, 1, m_apertureSize);
            cv::convertScaleAbs(gradX, gradX);
            cv::convertScaleAbs(gradY, gradY);
            cv::addWeighted(gradX, 0.5, gradY, 0.5, 0, outputImage);
            break;
        }

        case EdgeDetectionType::SCHARR: {
            cv::Mat gradX, gradY;
            cv::Scharr(grayImage, gradX, CV_16S, 1, 0);
            cv::Scharr(grayImage, gradY, CV_16S, 0, 1);
            cv::convertScaleAbs(gradX, gradX);
            cv::convertScaleAbs(gradY, gradY);
            cv::addWeighted(gradX, 0.5, gradY, 0.5, 0, outputImage);
            break;
        }

        case EdgeDetectionType::LAPLACIAN:
            cv::Laplacian(grayImage, outputImage, CV_16S, m_apertureSize);
            cv::convertScaleAbs(outputImage, outputImage);
            break;

        case EdgeDetectionType::CANNY:
            cv::Canny(grayImage, outputImage, m_threshold1, m_threshold2, m_apertureSize, m_L2gradient);
            break;

        default:
            std::cerr << "EdgeDetectionNode::process: Unknown edge detection type." << std::endl;
            outputImage = grayImage.clone();
            break;
        }

        m_outputValues[0] = outputImage;
    }

    int EdgeDetectionNode::getInputCount() const {
        return 1; // One input for the source image
    }

    int EdgeDetectionNode::getOutputCount() const {
        return 1; // One output for the processed image
    }

    std::string EdgeDetectionNode::getInputName(int index) const {
        if (index == 0) {
            return "Image";
        }
        return "";
    }

    std::string EdgeDetectionNode::getOutputName(int index) const {
        if (index == 0) {
            return "Edge Image";
        }
        return "";
    }

    void EdgeDetectionNode::setEdgeType(EdgeDetectionType edgeType) {
        m_edgeType = edgeType;
    }

    EdgeDetectionType EdgeDetectionNode::getEdgeType() const {
        return m_edgeType;
    }

    void EdgeDetectionNode::setThreshold1(double threshold) {
        m_threshold1 = threshold;
    }

    double EdgeDetectionNode::getThreshold1() const {
        return m_threshold1;
    }

    void EdgeDetectionNode::setThreshold2(double threshold) {
        m_threshold2 = threshold;
    }

    double EdgeDetectionNode::getThreshold2() const {
        return m_threshold2;
    }

    void EdgeDetectionNode::setApertureSize(int size) {
        m_apertureSize = validateApertureSize(size);
    }

    int EdgeDetectionNode::getApertureSize() const {
        return m_apertureSize;
    }

    void EdgeDetectionNode::setL2gradient(bool useL2) {
        m_L2gradient = useL2;
    }

    bool EdgeDetectionNode::getL2gradient() const {
        return m_L2gradient;
    }

    int EdgeDetectionNode::validateApertureSize(int size) {
        // Aperture size must be 1, 3, 5, or 7
        if (size != 1 && size != 3 && size != 5 && size != 7) {
            return 3; // Default to 3 if invalid
        }
        return size;
    }

} // namespace image_processor
