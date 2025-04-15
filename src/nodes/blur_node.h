#pragma once

#include "base_node.h"
#include <opencv2/opencv.hpp>

namespace image_processor {

    /**
     * @brief Enumeration of available blur types
     */
    enum class BlurType {
        BOX,        // Simple box blur
        GAUSSIAN,   // Gaussian blur
        MEDIAN,     // Median blur
        BILATERAL   // Bilateral filter (edge-preserving)
    };

    /**
     * @brief Node for applying blur effects to an image
     *
     * This node applies various types of blur effects to an input image,
     * including box blur, Gaussian blur, median blur, and bilateral filter.
     */
    class BlurNode : public BaseNode {
    public:
        /**
         * @brief Constructor for BlurNode
         * @param name The name of the node
         * @param blurType Initial blur type (default: GAUSSIAN)
         * @param kernelSize Initial kernel size (default: 5)
         * @param sigmaX Initial sigma X value for Gaussian and bilateral blur (default: 0)
         * @param sigmaY Initial sigma Y value for Gaussian blur (default: 0)
         * @param sigmaColor Initial sigma color value for bilateral filter (default: 75)
         * @param sigmaSpace Initial sigma space value for bilateral filter (default: 75)
         */
        BlurNode(const std::string& name = "Blur",
            BlurType blurType = BlurType::GAUSSIAN,
            int kernelSize = 5,
            double sigmaX = 0,
            double sigmaY = 0,
            double sigmaColor = 75,
            double sigmaSpace = 75);

        /**
         * @brief Destructor
         */
        virtual ~BlurNode() = default;

        /**
         * @brief Process the node
         *
         * Applies the selected blur effect to the input image
         */
        virtual void process() override;

        /**
         * @brief Get the number of inputs this node accepts
         * @return Always returns 1 as this node accepts a single input image
         */
        virtual int getInputCount() const override;

        /**
         * @brief Get the number of outputs this node produces
         * @return Always returns 1 as this node outputs a single processed image
         */
        virtual int getOutputCount() const override;

        /**
         * @brief Get the name of a specific input
         * @param index The input index
         * @return The name of the input at the specified index
         */
        virtual std::string getInputName(int index) const override;

        /**
         * @brief Get the name of a specific output
         * @param index The output index
         * @return The name of the output at the specified index
         */
        virtual std::string getOutputName(int index) const override;

        /**
         * @brief Set the blur type
         * @param blurType The new blur type
         */
        void setBlurType(BlurType blurType);

        /**
         * @brief Get the current blur type
         * @return The current blur type
         */
        BlurType getBlurType() const;

        /**
         * @brief Set the kernel size
         * @param kernelSize The new kernel size (must be positive and odd)
         */
        void setKernelSize(int kernelSize);

        /**
         * @brief Get the current kernel size
         * @return The current kernel size
         */
        int getKernelSize() const;

        /**
         * @brief Set the sigma X value (for Gaussian and bilateral blur)
         * @param sigmaX The new sigma X value
         */
        void setSigmaX(double sigmaX);

        /**
         * @brief Get the current sigma X value
         * @return The current sigma X value
         */
        double getSigmaX() const;

        /**
         * @brief Set the sigma Y value (for Gaussian blur)
         * @param sigmaY The new sigma Y value
         */
        void setSigmaY(double sigmaY);

        /**
         * @brief Get the current sigma Y value
         * @return The current sigma Y value
         */
        double getSigmaY() const;

        /**
         * @brief Set the sigma color value (for bilateral filter)
         * @param sigmaColor The new sigma color value
         */
        void setSigmaColor(double sigmaColor);

        /**
         * @brief Get the current sigma color value
         * @return The current sigma color value
         */
        double getSigmaColor() const;

        /**
         * @brief Set the sigma space value (for bilateral filter)
         * @param sigmaSpace The new sigma space value
         */
        void setSigmaSpace(double sigmaSpace);

        /**
         * @brief Get the current sigma space value
         * @return The current sigma space value
         */
        double getSigmaSpace() const;

    private:
        BlurType m_blurType;     // Type of blur to apply
        int m_kernelSize;        // Size of the blur kernel
        double m_sigmaX;         // Sigma X value for Gaussian and bilateral blur
        double m_sigmaY;         // Sigma Y value for Gaussian blur
        double m_sigmaColor;     // Sigma color value for bilateral filter
        double m_sigmaSpace;     // Sigma space value for bilateral filter

        /**
         * @brief Ensure that kernel size is positive and odd
         * @param size The size to validate
         * @return A valid kernel size (positive and odd)
         */
        int validateKernelSize(int size);
    };

} // namespace image_processor
