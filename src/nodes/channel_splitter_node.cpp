#include "channel_splitter_node.h"
#include <iostream>

namespace image_processor {

    ChannelSplitterNode::ChannelSplitterNode(const std::string& name) : BaseNode
    (name), m_channelCount(0) {
        int m_channelCount;
    }

    void ChannelSplitterNode::process() {
        if (!isReady()) {
            std::cerr << "ChannelSplitterNode::process: Node is not ready to process." << std::endl;
            return;
        }

        auto inputConnection = getInputConnection(0);
        if (inputConnection.first == nullptr) {
            std::cerr << "ChannelSplitterNode::process: No valid input connection." << std::endl;
            return;
        }

        cv::Mat inputImage = inputConnection.first->getOutputValue(inputConnection.second);
        if (inputImage.empty()) {
            std::cerr << "ChannelSplitterNode::process: Received empty image from input." << std::endl;
            return;
        }

        std::vector<cv::Mat> channels;
        cv::split(inputImage, channels);
        m_channelCount = channels.size();

        // Create 3-channel outputs for color visualization
        for (int i = 0; i < m_channelCount; ++i) {
            std::vector<cv::Mat> outputChannels;

            // For BGR images, create color-specific outputs
            if (m_channelCount == 3) {
                outputChannels = {
                    (i == 0) ? channels[i] : cv::Mat::zeros(inputImage.size(), CV_8UC1),  // Blue
                    (i == 1) ? channels[i] : cv::Mat::zeros(inputImage.size(), CV_8UC1),  // Green
                    (i == 2) ? channels[i] : cv::Mat::zeros(inputImage.size(), CV_8UC1)   // Red
                };
            }
            else {
                // For other channel counts, show single channel in first position
                outputChannels.resize(3, cv::Mat::zeros(inputImage.size(), CV_8UC1));
                outputChannels[0] = channels[i];
            }

            cv::Mat colorOutput;
            cv::merge(outputChannels, colorOutput);
            m_outputValues[i] = colorOutput;
        }
    }


    int ChannelSplitterNode::getInputCount() const {
        return 1; // One input for the source image
    }

    int ChannelSplitterNode::getOutputCount() const {
        return m_channelCount > 0 ? m_channelCount : 3;
    }

    std::string ChannelSplitterNode::getInputName(int index) const {
        if (index == 0) {
            return "Image";
        }
        return "";
    }

    std::string ChannelSplitterNode::getOutputName(int index) const {
        if (index >= 0 && index < m_channelCount) {
            switch (index) {
            case 0: return "Blue Channel";
            case 1: return "Green Channel";
            case 2: return "Red Channel";
            default: return "Channel " + std::to_string(index);
            }
        }
        return "";
    }

    int ChannelSplitterNode::getChannelCount() const {
        return m_channelCount;
    }

}
