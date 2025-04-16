#pragma once

#include "base_node.h"
#include <opencv2/opencv.hpp>

namespace image_processor {

    /**
     * @brief Enumeration of available blending modes
     */
    enum class BlendMode {
        NORMAL,     // Simple alpha blending
        ADD,        // Addition blending
        MULTIPLY,   // Multiplication blending
        SCREEN,     // Screen blending
        OVERLAY,    // Overlay blending
        DARKEN,     // Darken blending
        LIGHTEN,    // Lighten blending
        DIFFERENCE  // Difference blending
    };

    /**
     * @brief Node for blending two images together
     *
     * This node takes two input images and blends them together using
     * various blending modes and an alpha factor to control the blend strength.
     */
    class BlendNode : public BaseNode {
    public:
        /**
         * @brief Constructor for BlendNode
         * @param name The name of the node
         * @param blendMode Initial blend mode (default: NORMAL)
         * @param alpha Initial alpha value for blending (default: 0.5)
         */
        BlendNode(const std::string& name = "Blend",
            BlendMode blendMode = BlendMode::NORMAL,
            double alpha = 0.5);

        /**
         * @brief Destructor
         */
        virtual ~BlendNode() = default;

        /**
         * @brief Process the node
         *
         * Blends the two input images together using the selected blend mode
         */
        virtual void process() override;

        /**
         * @brief Get the number of inputs this node accepts
         * @return Always returns 2 as this node accepts two input images
         */
        virtual int getInputCount() const override;

        /**
         * @brief Get the number of outputs this node produces
         * @return Always returns 1 as this node outputs a single blended image
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
         * @brief Set the blend mode
         * @param blendMode The new blend mode
         */
        void setBlendMode(BlendMode blendMode);

        /**
         * @brief Get the current blend mode
         * @return The current blend mode
         */
        BlendMode getBlendMode() const;

        /**
         * @brief Set the alpha value for blending
         * @param alpha The new alpha value (0.0 to 1.0)
         */
        void setAlpha(double alpha);

        /**
         * @brief Get the current alpha value
         * @return The current alpha value
         */
        double getAlpha() const;

    private:
        BlendMode m_blendMode;  // Type of blending to apply
        double m_alpha;         // Alpha value for blending (0.0 to 1.0)

        /**
         * @brief Validate the alpha value to ensure it's in the range [0.0, 1.0]
         * @param alpha The alpha value to validate
         * @return A valid alpha value clamped to [0.0, 1.0]
         */
        double validateAlpha(double alpha);

        /**
         * @brief Apply normal blending (alpha blending)
         * @param src1 First source image
         * @param src2 Second source image
         * @param dst Destination image
         */
        void applyNormalBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst);

        /**
         * @brief Apply addition blending
         * @param src1 First source image
         * @param src2 Second source image
         * @param dst Destination image
         */
        void applyAddBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst);

        /**
         * @brief Apply multiplication blending
         * @param src1 First source image
         * @param src2 Second source image
         * @param dst Destination image
         */
        void applyMultiplyBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst);

        /**
         * @brief Apply screen blending
         * @param src1 First source image
         * @param src2 Second source image
         * @param dst Destination image
         */
        void applyScreenBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst);

        /**
         * @brief Apply overlay blending
         * @param src1 First source image
         * @param src2 Second source image
         * @param dst Destination image
         */
        void applyOverlayBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst);

        /**
         * @brief Apply darken blending
         * @param src1 First source image
         * @param src2 Second source image
         * @param dst Destination image
         */
        void applyDarkenBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst);

        /**
         * @brief Apply lighten blending
         * @param src1 First source image
         * @param src2 Second source image
         * @param dst Destination image
         */
        void applyLightenBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst);

        /**
         * @brief Apply difference blending
         * @param src1 First source image
         * @param src2 Second source image
         * @param dst Destination image
         */
        void applyDifferenceBlend(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst);
    };

} // namespace image_processor
