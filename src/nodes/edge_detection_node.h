#pragma once

#include "base_node.h"
#include <opencv2/opencv.hpp>

namespace image_processor {

    /**
     * @brief Enumeration of available edge detection types
     */
    enum class EdgeDetectionType {
        SOBEL,      // Sobel operator
        SCHARR,     // Scharr operator (more accurate than Sobel)
        LAPLACIAN,  // Laplacian operator
        CANNY       // Canny edge detector
    };

    /**
     * @brief Node for applying edge detection to an image
     *
     * This node applies various edge detection algorithms to an input image,
     * including Sobel, Scharr, Laplacian, and Canny edge detectors.
     */
    class EdgeDetectionNode : public BaseNode {
    public:
        /**
         * @brief Constructor for EdgeDetectionNode
         * @param name The name of the node
         * @param edgeType Initial edge detection type (default: CANNY)
         * @param threshold1 First threshold for Canny edge detector (default: 100)
         * @param threshold2 Second threshold for Canny edge detector (default: 200)
         * @param apertureSize Aperture size for gradient operators (default: 3)
         * @param L2gradient Flag indicating whether to use L2 norm for Canny (default: false)
         */
        EdgeDetectionNode(const std::string& name = "Edge Detection",
            EdgeDetectionType edgeType = EdgeDetectionType::CANNY,
            double threshold1 = 100,
            double threshold2 = 200,
            int apertureSize = 3,
            bool L2gradient = false);

        /**
         * @brief Destructor
         */
        virtual ~EdgeDetectionNode() = default;

        /**
         * @brief Process the node
         *
         * Applies the selected edge detection algorithm to the input image
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
         * @brief Set the edge detection type
         * @param edgeType The new edge detection type
         */
        void setEdgeType(EdgeDetectionType edgeType);

        /**
         * @brief Get the current edge detection type
         * @return The current edge detection type
         */
        EdgeDetectionType getEdgeType() const;

        /**
         * @brief Set the first threshold for Canny edge detector
         * @param threshold The new threshold value
         */
        void setThreshold1(double threshold);

        /**
         * @brief Get the current first threshold for Canny
         * @return The current first threshold
         */
        double getThreshold1() const;

        /**
         * @brief Set the second threshold for Canny edge detector
         * @param threshold The new threshold value
         */
        void setThreshold2(double threshold);

        /**
         * @brief Get the current second threshold for Canny
         * @return The current second threshold
         */
        double getThreshold2() const;

        /**
         * @brief Set the aperture size for gradient operators
         * @param size The new aperture size (must be 1, 3, 5, or 7)
         */
        void setApertureSize(int size);

        /**
         * @brief Get the current aperture size
         * @return The current aperture size
         */
        int getApertureSize() const;

        /**
         * @brief Set the L2gradient flag for Canny edge detector
         * @param useL2 Whether to use L2 norm (true) or L1 norm (false)
         */
        void setL2gradient(bool useL2);

        /**
         * @brief Get the current L2gradient flag
         * @return The current L2gradient flag
         */
        bool getL2gradient() const;

    private:
        EdgeDetectionType m_edgeType;  // Type of edge detection to apply
        double m_threshold1;           // First threshold for Canny
        double m_threshold2;           // Second threshold for Canny
        int m_apertureSize;            // Aperture size for gradient operators
        bool m_L2gradient;             // Whether to use L2 norm for Canny

        /**
         * @brief Validate the aperture size for gradient operators
         * @param size The size to validate
         * @return A valid aperture size (1, 3, 5, or 7)
         */
        int validateApertureSize(int size);
    };

} // namespace image_processor
