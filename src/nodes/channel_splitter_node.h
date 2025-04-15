#pragma once

#include "base_node.h"
#include <opencv2/opencv.hpp>

namespace image_processor {

    /**
     * @brief Node for splitting an image into its individual color channels
     *
     * This node takes a color image as input and splits it into its individual
     * color channels (Blue, Green, Red for BGR images, or other channels depending
     * on the image type).
     */
    class ChannelSplitterNode : public BaseNode {
    public:
        /**
         * @brief Constructor for ChannelSplitterNode
         * @param name The name of the node
         */
        explicit ChannelSplitterNode(const std::string& name = "Channel Splitter");

        /**
         * @brief Destructor
         */
        virtual ~ChannelSplitterNode() = default;

        /**
         * @brief Process the node
         *
         * Splits the input image into its individual color channels
         */
        virtual void process() override;

        /**
         * @brief Get the number of inputs this node accepts
         * @return Always returns 1 as this node accepts a single input image
         */
        virtual int getInputCount() const override;

        /**
         * @brief Get the number of outputs this node produces
         * @return Returns the number of channels in the input image (typically 3 for BGR images)
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
         * @brief Get the number of channels in the last processed image
         * @return The number of channels in the last processed image
         */
        int getChannelCount() const;

    private:
        int m_channelCount;  // Number of channels in the last processed image
    };

} // namespace image_processor
