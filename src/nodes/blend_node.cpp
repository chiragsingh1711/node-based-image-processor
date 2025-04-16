#include "blend_node.h"
#include <iostream>
#include <algorithm>

namespace image_processor {

    BlendNode::BlendNode(const std::string& name, BlendMode blendMode, double alpha)
        : BaseNode(name),
        m_blendMode(blendMode),
        m_alpha(validateAlpha(alpha)) {
    }

    void BlendNode::process() {
        if (!isReady()) {
            std::cerr << "BlendNode::process: Node is not ready to process." << std::endl;
            return;
        }

        auto input1 = getInputConnection(0);
        auto input2 = getInputConnection(1);

        if (!input1.first || !input2.first) {
            std::cerr << "BlendNode::process: Missing input connections." << std::endl;
            return;
        }

        cv::Mat inputImage1 = input1.first->getOutputValue(input1.second);
        cv::Mat inputImage2 = input2.first->getOutputValue(input2.second);

        if (inputImage1.empty() || inputImage2.empty()) {
            std::cerr << "BlendNode::process: One or both input images are empty." << std::endl;
            return;
        }

        // Ensure both images have the same size and type
        if (inputImage1.size() != inputImage2.size()) {
            // Resize the second image to match the first
            std::cout << "Images not of same size" << std::endl;
            cv::resize(inputImage2, inputImage2, inputImage1.size());
        }

        // Ensure same number of channels
        if (inputImage1.channels() != inputImage2.channels()) {
            if (inputImage2.channels() == 1) {
                cv::cvtColor(inputImage2, inputImage2, cv::COLOR_GRAY2BGR);
            }
            else {
                cv::cvtColor(inputImage2, inputImage2, cv::COLOR_BGR2GRAY);
            }
        }

        // Convert images to the same type if needed
        int targetType = inputImage1.type();
        if (inputImage2.type() != targetType) {
            inputImage2.convertTo(inputImage2, targetType);
        }

        cv::Mat outputImage;

        // Apply the selected blending mode
        switch (m_blendMode) {
        case BlendMode::NORMAL:
            applyNormalBlend(inputImage1, inputImage2, outputImage);
            break;

        case BlendMode::ADD:
            applyAddBlend(inputImage1, inputImage2, outputImage);
            break;

        case BlendMode::MULTIPLY:
            applyMultiplyBlend(inputImage1, inputImage2, outputImage);
            break;

        case BlendMode::SCREEN:
            applyScreenBlend(inputImage1, inputImage2, outputImage);
            break;

        case BlendMode::OVERLAY:
            applyOverlayBlend(inputImage1, inputImage2, outputImage);
            break;

        case BlendMode::DARKEN:
            applyDarkenBlend(inputImage1, inputImage2, outputImage);
            break;

        case BlendMode::LIGHTEN:
            applyLightenBlend(inputImage1, inputImage2, outputImage);
            break;

        case BlendMode::DIFFERENCE:
            applyDifferenceBlend(inputImage1, inputImage2, outputImage);
            break;

        default:
            std::cerr << "BlendNode::process: Unknown blend mode." << std::endl;
            applyNormalBlend(inputImage1, inputImage2, outputImage);
            break;
        }

        cv::addWeighted(inputImage1, 1.0 - m_alpha, inputImage2, m_alpha, 0.0, outputImage);

        m_outputValues[0] = outputImage;
    }

    int BlendNode::getInputCount() const {
        return 2; // Two inputs for the source images
    }

    int BlendNode::getOutputCount() const {
        return 1; // One output for the blended image
    }

    std::string BlendNode::getInputName(int index) const {
        if (index == 0) {
            return "Base Image";
        }
        else if (index == 1) {
            return "Blend Image";
        }
        return "";
    }

    std::string BlendNode::getOutputName(int index) const {
        if (index == 0) {
            return "Blended Image";
        }
        return "";
    }

    void BlendNode::setBlendMode(BlendMode blendMode) {
        m_blendMode = blendMode;
    }

    BlendMode BlendNode::getBlendMode() const {
        return m_blendMode;
    }

    void BlendNode::setAlpha(double alpha) {
        m_alpha = validateAlpha(alpha);
    }

    double BlendNode::getAlpha() const {
        return m_alpha;
    }

    double BlendNode::validateAlpha(double alpha) {
        return std::max(0.0, std::min(1.0, alpha));
    }

    void BlendNode::applyNormalBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst) {
        // Normal blending: dst = src1 * (1 - alpha) + src2 * alpha
        cv::addWeighted(src1, 1.0 - m_alpha, src2, m_alpha, 0.0, dst);
    }

    void BlendNode::applyAddBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst) {
        // Add blending: dst = src1 + src2 * alpha
        cv::addWeighted(src1, 1.0, src2, m_alpha, 0.0, dst);

        // Ensure values don't exceed the maximum
        cv::min(dst, 255, dst);
    }

    void BlendNode::applyMultiplyBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst) {
        // Multiply blending: dst = src1 * (src2 * alpha + (1 - alpha))

        // Create a temporary image for the blend factor
        cv::Mat blendFactor;
        cv::addWeighted(src2, m_alpha, cv::Scalar::all(255), 1.0 - m_alpha, 0.0, blendFactor);

        // Convert to float for multiplication
        cv::Mat src1Float, blendFactorFloat;
        src1.convertTo(src1Float, CV_32F, 1.0 / 255.0);
        blendFactor.convertTo(blendFactorFloat, CV_32F, 1.0 / 255.0);

        // Multiply
        cv::Mat resultFloat;
        cv::multiply(src1Float, blendFactorFloat, resultFloat);

        // Convert back to original type
        resultFloat.convertTo(dst, src1.type(), 255.0);
    }

    void BlendNode::applyScreenBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst) {
        // Screen blending: dst = 1 - (1 - src1) * (1 - src2 * alpha)

        // Convert to float for calculations
        cv::Mat src1Float, src2Float;
        src1.convertTo(src1Float, CV_32F, 1.0 / 255.0);
        src2.convertTo(src2Float, CV_32F, 1.0 / 255.0);

        // Apply screen formula
        cv::Mat invSrc1, invSrc2, result;
        invSrc1 = 1.0 - src1Float;
        invSrc2 = 1.0 - src2Float * m_alpha - cv::Scalar::all(1.0 - m_alpha);
        cv::multiply(invSrc1, invSrc2, result);
        result = 1.0 - result;

        // Convert back to original type
        result.convertTo(dst, src1.type(), 255.0);
    }

    void BlendNode::applyOverlayBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst) {
        // Overlay blending: combines Multiply and Screen modes
        // If src1 < 0.5: dst = 2 * src1 * src2
        // If src1 >= 0.5: dst = 1 - 2 * (1 - src1) * (1 - src2)

        // Convert to float for calculations
        cv::Mat src1Float, src2Float;
        src1.convertTo(src1Float, CV_32F, 1.0 / 255.0);
        src2.convertTo(src2Float, CV_32F, 1.0 / 255.0);

        // Adjust src2 with alpha
        src2Float = src2Float * m_alpha + cv::Scalar::all(0.5 * (1.0 - m_alpha));

        // Create masks for the two conditions
        cv::Mat mask1, mask2;
        mask1 = src1Float < 0.5;
        mask2 = src1Float >= 0.5;

        // Apply the two formulas
        cv::Mat result1, result2, result;
        cv::multiply(2.0 * src1Float, src2Float, result1);
        cv::multiply(1.0 - 2.0 * (1.0 - src1Float), 1.0 - 2.0 * (1.0 - src2Float), result2);
        result2 = 1.0 - result2;

        // Combine the results using the masks
        result = cv::Mat::zeros(src1Float.size(), CV_32F);
        result1.copyTo(result, mask1);
        result2.copyTo(result, mask2);

        // Convert back to original type
        result.convertTo(dst, src1.type(), 255.0);
    }

    void BlendNode::applyDarkenBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst) {
        // Darken blending: dst = min(src1, src2)

        // Adjust src2 with alpha
        cv::Mat adjustedSrc2;
        cv::addWeighted(src2, m_alpha, src1, 1.0 - m_alpha, 0.0, adjustedSrc2);

        // Apply darken formula
        cv::min(src1, adjustedSrc2, dst);
    }

    void BlendNode::applyLightenBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst) {
        // Lighten blending: dst = max(src1, src2)

        // Adjust src2 with alpha
        cv::Mat adjustedSrc2;
        cv::addWeighted(src2, m_alpha, src1, 1.0 - m_alpha, 0.0, adjustedSrc2);

        // Apply lighten formula
        cv::max(src1, adjustedSrc2, dst);
    }

    void BlendNode::applyDifferenceBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst) {
        // Difference blending: dst = |src1 - src2|

        // Adjust src2 with alpha
        cv::Mat adjustedSrc2;
        cv::addWeighted(src2, m_alpha, src1, 1.0 - m_alpha, 0.0, adjustedSrc2);

        // Apply difference formula
        cv::absdiff(src1, adjustedSrc2, dst);
    }

} // namespace image_processor
