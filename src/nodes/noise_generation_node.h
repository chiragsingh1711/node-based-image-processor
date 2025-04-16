#pragma once

#include "base_node.h"
#include <opencv2/opencv.hpp>
#include <random>

namespace image_processor {

    /**
     * @brief Enumeration of available noise types
     */
    enum class NoiseType {
        GAUSSIAN,   // Gaussian (normal) distributed noise
        UNIFORM,    // Uniformly distributed noise
        SALT_PEPPER // Salt and pepper noise
    };

    /**
     * @brief Node for generating noise images
     *
     * This node generates various types of noise images that can be used
     * for blending with other images or as standalone effects.
     */
    class NoiseGenerationNode : public BaseNode {
    public:
        /**
         * @brief Constructor for NoiseGenerationNode
         * @param name The name of the node
         * @param noiseType Initial noise type (default: GAUSSIAN)
         * @param width Initial width of the noise image (default: 512)
         * @param height Initial height of the noise image (default: 512)
         * @param mean Mean value for Gaussian noise (default: 0.0)
         * @param stdDev Standard deviation for Gaussian noise (default: 1.0)
         * @param low Low value for uniform noise (default: 0.0)
         * @param high High value for uniform noise (default: 1.0)
         * @param saltPepperRatio Ratio of salt vs pepper noise (default: 0.5)
         * @param density Density of salt and pepper noise (default: 0.05)
         */
        NoiseGenerationNode(const std::string& name = "Noise Generation",
            NoiseType noiseType = NoiseType::GAUSSIAN,
            int width = 512,
            int height = 512,
            double mean = 0.0,
            double stdDev = 1.0,
            double low = 0.0,
            double high = 1.0,
            double saltPepperRatio = 0.5,
            double density = 0.05);

        /**
         * @brief Destructor
         */
        virtual ~NoiseGenerationNode() = default;

        /**
         * @brief Process the node
         *
         * Generates a noise image based on the current settings
         */
        virtual void process() override;

        /**
         * @brief Get the number of inputs this node accepts
         * @return Always returns 0 as this node generates its own output
         */
        virtual int getInputCount() const override;

        /**
         * @brief Get the number of outputs this node produces
         * @return Always returns 1 as this node outputs a single noise image
         */
        virtual int getOutputCount() const override;

        /**
         * @brief Get the name of a specific input
         * @param index The input index
         * @return Empty string as this node has no inputs
         */
        virtual std::string getInputName(int index) const override;

        /**
         * @brief Get the name of a specific output
         * @param index The output index
         * @return The name of the output at the specified index
         */
        virtual std::string getOutputName(int index) const override;

        /**
         * @brief Set the noise type
         * @param noiseType The new noise type
         */
        void setNoiseType(NoiseType noiseType);

        /**
         * @brief Get the current noise type
         * @return The current noise type
         */
        NoiseType getNoiseType() const;

        /**
         * @brief Set the dimensions of the noise image
         * @param width The new width
         * @param height The new height
         */
        void setDimensions(int width, int height);

        /**
         * @brief Get the current dimensions of the noise image
         * @return A pair containing the width and height
         */
        std::pair<int, int> getDimensions() const;

        /**
         * @brief Set the mean and standard deviation for Gaussian noise
         * @param mean The new mean value
         * @param stdDev The new standard deviation value
         */
        void setGaussianParameters(double mean, double stdDev);

        /**
         * @brief Get the current Gaussian noise parameters
         * @return A pair containing the mean and standard deviation
         */
        std::pair<double, double> getGaussianParameters() const;

        /**
         * @brief Set the low and high values for uniform noise
         * @param low The new low value
         * @param high The new high value
         */
        void setUniformParameters(double low, double high);

        /**
         * @brief Get the current uniform noise parameters
         * @return A pair containing the low and high values
         */
        std::pair<double, double> getUniformParameters() const;

        /**
         * @brief Set the salt-pepper ratio and density for salt and pepper noise
         * @param ratio The new salt-pepper ratio
         * @param density The new noise density
         */
        void setSaltPepperParameters(double ratio, double density);

        /**
         * @brief Get the current salt and pepper noise parameters
         * @return A pair containing the salt-pepper ratio and density
         */
        std::pair<double, double> getSaltPepperParameters() const;

    private:
        NoiseType m_noiseType;
        int m_width;
        int m_height;
        double m_mean;
        double m_stdDev;
        double m_low;
        double m_high;
        double m_saltPepperRatio;
        double m_density;

        std::mt19937 m_generator;  // Mersenne Twister random number generator

        /**
         * @brief Generate Gaussian noise
         * @param output The output image to fill with noise
         */
        void generateGaussianNoise(cv::Mat& output);

        /**
         * @brief Generate uniform noise
         * @param output The output image to fill with noise
         */
        void generateUniformNoise(cv::Mat& output);

        /**
         * @brief Generate salt and pepper noise
         * @param output The output image to fill with noise
         */
        void generateSaltPepperNoise(cv::Mat& output);
    };

} // namespace image_processor
