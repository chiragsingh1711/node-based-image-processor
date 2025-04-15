#pragma once

#include "base_node.h"
#include <opencv2/opencv.hpp>

namespace image_processor {

    /**
     * @brief Enumeration of available thresholding types
     */
    enum class ThresholdType {
        BINARY,
        BINARY_INV,
        TRUNC,
        TOZERO,
        TOZERO_INV,
        OTSU,
        ADAPTIVE_MEAN,
        ADAPTIVE_GAUSSIAN
    };

    /**
     * @brief Node for applying thresholding to an image
     *
     * This node applies various types of thresholding to an input image,
     * including simple thresholding and adaptive thresholding methods.
     */
    class ThresholdNode : public BaseNode {
    public:
        /**
         * @brief Constructor for ThresholdNode
         * @param name The name of the node
         * @param thresholdType Initial threshold type (default: BINARY)
         * @param threshold Initial threshold value (default: 128)
         * @param maxValue Maximum value to use with BINARY and BINARY_INV (default: 255)
         * @param blockSize Block size for adaptive methods (default: 11)
         * @param C Constant subtracted from mean or weighted sum for adaptive methods (default: 2)
         */
        ThresholdNode(const std::string& name = "Threshold",
            ThresholdType thresholdType = ThresholdType::BINARY,
            double threshold = 128,
            double maxValue = 255,
            int blockSize = 11,
            double C = 2);

        /**
         * @brief Destructor
         */
        virtual ~ThresholdNode() = default;

        /**
         * @brief Process the node
         *
         * Applies the selected thresholding method to the input image
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
         * @brief Set the thresholding type
         * @param thresholdType The new thresholding type
         */
        void setThresholdType(ThresholdType thresholdType);

        /**
         * @brief Get the current thresholding type
         * @return The current thresholding type
         */
        ThresholdType getThresholdType() const;

        /**
         * @brief Set the threshold value
         * @param threshold The new threshold value
         */
        void setThreshold(double threshold);

        /**
         * @brief Get the current threshold value
         * @return The current threshold value
         */
        double getThreshold() const;

        /**
         * @brief Set the maximum value (for BINARY and BINARY_INV types)
         * @param maxValue The new maximum value
         */
        void setMaxValue(double maxValue);

        /**
         * @brief Get the current maximum value
         * @return The current maximum value
         */
        double getMaxValue() const;

        /**
         * @brief Set the block size (for adaptive methods)
         * @param blockSize The new block size (must be odd)
         */
        void setBlockSize(int blockSize);

        /**
         * @brief Get the current block size
         * @return The current block size
         */
        int getBlockSize() const;

        /**
         * @brief Set the C value (for adaptive methods)
         * @param C The new C value
         */
        void setC(double C);

        /**
         * @brief Get the current C value
         * @return The current C value
         */
        double getC() const;

    private:
        ThresholdType m_thresholdType;  // Type of thresholding to apply
        double m_threshold;             // Threshold value
        double m_maxValue;              // Maximum value for BINARY and BINARY_INV
        int m_blockSize;                // Block size for adaptive methods
        double m_C;                     // Constant for adaptive methods

        /**
         * @brief Ensure that block size is positive and odd
         * @param size The size to validate
         * @return A valid block size (positive and odd)
         */
        int validateBlockSize(int size);
    };

} // namespace image_processor
