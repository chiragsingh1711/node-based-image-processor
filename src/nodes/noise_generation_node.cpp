#include "noise_generation_node.h"
#include <iostream>
#include <chrono>

namespace image_processor {

    NoiseGenerationNode::NoiseGenerationNode(const std::string& name, NoiseType noiseType,
        int width, int height, double mean, double stdDev,
        double low, double high, double saltPepperRatio, double density)
        : BaseNode(name),
        m_noiseType(noiseType),
        m_width(width),
        m_height(height),
        m_mean(mean),
        m_stdDev(stdDev),
        m_low(low),
        m_high(high),
        m_saltPepperRatio(saltPepperRatio),
        m_density(density) {
        // Seed the random number generator with the current time
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        m_generator.seed(seed);
    }

    void NoiseGenerationNode::process() {
        if (m_width <= 0 || m_height <= 0) {
            std::cerr << "NoiseGenerationNode: Invalid dimensions" << std::endl;
            return;
        }
        cv::Mat noiseImage(m_height, m_width, CV_8UC3);

        switch (m_noiseType) {
        case NoiseType::GAUSSIAN:
            generateGaussianNoise(noiseImage);
            break;
        case NoiseType::UNIFORM:
            generateUniformNoise(noiseImage);
            break;
        case NoiseType::SALT_PEPPER:
            generateSaltPepperNoise(noiseImage);
            break;
        default:
            std::cerr << "NoiseGenerationNode::process: Unknown noise type." << std::endl;
            noiseImage = cv::Mat::zeros(m_height, m_width, CV_8UC3);
            break;
        }

        m_outputValues[0] = noiseImage;
    }

    int NoiseGenerationNode::getInputCount() const {
        return 0; // No inputs for this node
    }

    int NoiseGenerationNode::getOutputCount() const {
        return 1; // One output for the noise image
    }

    std::string NoiseGenerationNode::getInputName(int index) const {
        return ""; // No inputs
    }

    std::string NoiseGenerationNode::getOutputName(int index) const {
        if (index == 0) {
            return "Noise Image";
        }
        return "";
    }

    void NoiseGenerationNode::setNoiseType(NoiseType noiseType) {
        m_noiseType = noiseType;
    }

    NoiseType NoiseGenerationNode::getNoiseType() const {
        return m_noiseType;
    }

    void NoiseGenerationNode::setDimensions(int width, int height) {
        m_width = width;
        m_height = height;
    }

    std::pair<int, int> NoiseGenerationNode::getDimensions() const {
        return { m_width, m_height };
    }

    void NoiseGenerationNode::setGaussianParameters(double mean, double stdDev) {
        m_mean = mean;
        m_stdDev = stdDev;
    }

    std::pair<double, double> NoiseGenerationNode::getGaussianParameters() const {
        return { m_mean, m_stdDev };
    }

    void NoiseGenerationNode::setUniformParameters(double low, double high) {
        m_low = low;
        m_high = high;
    }

    std::pair<double, double> NoiseGenerationNode::getUniformParameters() const {
        return { m_low, m_high };
    }

    void NoiseGenerationNode::setSaltPepperParameters(double ratio, double density) {
        m_saltPepperRatio = ratio;
        m_density = density;
    }

    std::pair<double, double> NoiseGenerationNode::getSaltPepperParameters() const {
        return { m_saltPepperRatio, m_density };
    }

    void NoiseGenerationNode::generateGaussianNoise(cv::Mat& output) {
        std::normal_distribution<double> distribution(m_mean, m_stdDev);

        for (int y = 0; y < output.rows; ++y) {
            for (int x = 0; x < output.cols; ++x) {
                double noise = distribution(m_generator);
                output.at<uchar>(y, x) = cv::saturate_cast<uchar>(noise * 255.0);
            }
        }
    }

    void NoiseGenerationNode::generateUniformNoise(cv::Mat& output) {
        std::uniform_real_distribution<double> distribution(m_low, m_high);

        for (int y = 0; y < output.rows; ++y) {
            for (int x = 0; x < output.cols; ++x) {
                double noise = distribution(m_generator);
                output.at<uchar>(y, x) = cv::saturate_cast<uchar>(noise * 255.0);
            }
        }
    }

    void NoiseGenerationNode::generateSaltPepperNoise(cv::Mat& output) {
        std::uniform_real_distribution<double> distribution(0.0, 1.0);

        for (int y = 0; y < output.rows; ++y) {
            for (int x = 0; x < output.cols; ++x) {
                double random = distribution(m_generator);
                if (random < m_density) {
                    if (random < m_density * m_saltPepperRatio) {
                        output.at<uchar>(y, x) = 255; // Salt
                    }
                    else {
                        output.at<uchar>(y, x) = 0;   // Pepper
                    }
                }
                else {
                    output.at<uchar>(y, x) = 128; // Gray background
                }
            }
        }
    }

} // namespace image_processor
