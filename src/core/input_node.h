#pragma once

#include "base_node.h"
#include <string>
#include <opencv2/opencv.hpp>

namespace image_processor {

    /**
     *
     * The InputNode serves as an entry point for images into the processing graph.
     * It can load images from files or accept direct image data input.
     */
    class InputNode : public BaseNode {
    public:

        InputNode(const std::string& name = "Input");


        virtual ~InputNode() = default;
        virtual void process() override;

        virtual int getInputCount() const override;
        virtual int getOutputCount() const override;

        virtual std::string getInputName(int index) const override;
        virtual std::string getOutputName(int index) const override;

        bool loadImage(const std::string& filePath);
        void setImage(const cv::Mat& image);

        cv::Mat getImage() const;
        bool hasValidImage() const;

        virtual bool isReady() const override;

    private:
        cv::Mat m_image;                     // The input image
        std::string m_currentImagePath;      // Path to the currently loaded image file (if any)
    };

}
