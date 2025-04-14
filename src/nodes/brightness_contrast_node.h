#pragma once

#include "base_node.h"
#include <opencv2/opencv.hpp>

namespace image_processor {

    /**
     * @brief Node for adjusting brightness and contrast of an image
     *
     * This node applies brightness and contrast adjustments to an input image.
     * The formula used is: output = alpha * input + beta
     * where alpha controls contrast and beta controls brightness.
     */
    class BrightnessContrastNode : public BaseNode {
    public:
        /**
         * @brief Constructor for BrightnessContrastNode
         * @param name The name of the node
         * @param alpha Initial contrast value (default: 1.0)
         * @param beta Initial brightness value (default: 0.0)
         */
        BrightnessContrastNode(const std::string& name = "Brightness/Contrast",
            float alpha = 1.0f, float beta = 0.0f);

        /**
         * @brief Destructor
         */
        virtual ~BrightnessContrastNode() = default;

        /**
         * @brief Process the node
         *
         * Applies brightness and contrast adjustments to the input image
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
         * @brief Set the contrast value (alpha)
         * @param alpha The new contrast value
         */
        void setContrast(float alpha);

        /**
         * @brief Get the current contrast value (alpha)
         * @return The current contrast value
         */
        float getContrast() const;

        /**
         * @brief Set the brightness value (beta)
         * @param beta The new brightness value
         */
        void setBrightness(float beta);

        /**
         * @brief Get the current brightness value (beta)
         * @return The current brightness value
         */
        float getBrightness() const;

    private:
        float m_alpha;  // Contrast control (1.0 means no change)
        float m_beta;   // Brightness control (0.0 means no change)
    };

} // namespace image_processor
