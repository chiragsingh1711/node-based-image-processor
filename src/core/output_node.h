#pragma once

#include "base_node.h"
#include <string>
#include <opencv2/opencv.hpp>

namespace image_processor {

    /**
     * @brief Node for retrieving and saving output images from the processing graph
     *
     * The OutputNode serves as a terminal point in the processing graph.
     * It can save images to files and provide access to the final processed image.
     */
    class OutputNode : public BaseNode {
    public:
        /**
         * @brief Constructor for OutputNode
         * @param name The name of the node
         */
        OutputNode(const std::string& name = "Output");

        /**
         * @brief Destructor
         */
        virtual ~OutputNode() = default;

        /**
         * @brief Process the node
         *
         * For OutputNode, this retrieves the input image and stores it internally
         */
        virtual void process() override;

        /**
         * @brief Get the number of inputs this node accepts
         * @return Always returns 1 as this node accepts a single input image
         */
        virtual int getInputCount() const override;

        /**
         * @brief Get the number of outputs this node produces
         * @return Always returns 0 as this is an output node
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
         * @return Empty string as this node has no outputs
         */
        virtual std::string getOutputName(int index) const override;

        /**
         * @brief Save the current image to a file
         * @param filePath The path where the image should be saved
         * @return True if the image was saved successfully, false otherwise
         */
        bool saveImage(const std::string& filePath) const;

        /**
         * @brief Get the current output image
         * @return The current output image
         */
        cv::Mat getImage() const;

        /**
         * @brief Check if the node has a valid image
         * @return True if the node has a valid image, false otherwise
         */
        bool hasValidImage() const;

        /**
         * @brief Override isReady to check if we have a valid input connection
         * @return True if the node has a valid input connection
         */
        virtual bool isReady() const override;

    private:
        cv::Mat m_image;  // The output image
    };

}
