#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <memory>

namespace image_processor {

    /**
     * @brief Class for handling image data
     *
     * This class wraps OpenCV's Mat class to provide additional functionality
     * and easier management of image data within our processing system.
     */
    class Image {
    public:
        /**
         * @brief Default constructor
         * Creates an empty image
         */
        Image();

        /**
         * @brief Constructor with dimensions and type
         * @param width The width of the image
         * @param height The height of the image
         * @param type The OpenCV type of the image (e.g., CV_8UC3 for BGR)
         */
        Image(int width, int height, int type);

        /**
         * @brief Constructor from OpenCV Mat
         * @param mat The OpenCV Mat to wrap
         */
        Image(const cv::Mat& mat);

        /**
         * @brief Copy constructor
         * @param other The Image to copy
         */
        Image(const Image& other);

        /**
         * @brief Move constructor
         * @param other The Image to move
         */
        Image(Image&& other) noexcept;

        /**
         * @brief Destructor
         */
        ~Image() = default;

        /**
         * @brief Copy assignment operator
         * @param other The Image to copy
         * @return Reference to this Image
         */
        Image& operator=(const Image& other);

        /**
         * @brief Move assignment operator
         * @param other The Image to move
         * @return Reference to this Image
         */
        Image& operator=(Image&& other) noexcept;

        /**
         * @brief Load an image from a file
         * @param filePath The path to the image file
         * @return True if the image was loaded successfully, false otherwise
         */
        bool load(const std::string& filePath);

        /**
         * @brief Save the image to a file
         * @param filePath The path where the image should be saved
         * @return True if the image was saved successfully, false otherwise
         */
        bool save(const std::string& filePath) const;

        /**
         * @brief Get the width of the image
         * @return The width in pixels
         */
        int getWidth() const;

        /**
         * @brief Get the height of the image
         * @return The height in pixels
         */
        int getHeight() const;

        /**
         * @brief Get the number of channels in the image
         * @return The number of channels
         */
        int getChannels() const;

        /**
         * @brief Get the OpenCV type of the image
         * @return The OpenCV type
         */
        int getType() const;

        /**
         * @brief Check if the image is empty
         * @return True if the image is empty, false otherwise
         */
        bool isEmpty() const;

        /**
         * @brief Get the underlying OpenCV Mat
         * @return The OpenCV Mat
         */
        cv::Mat getMat() const;

        /**
         * @brief Set the underlying OpenCV Mat
         * @param mat The new OpenCV Mat
         */
        void setMat(const cv::Mat& mat);

        /**
         * @brief Clone the image
         * @return A new Image that is a deep copy of this one
         */
        Image clone() const;

        /**
         * @brief Convert the image to grayscale
         * @return A new Image in grayscale
         */
        Image toGrayscale() const;

        /**
         * @brief Resize the image
         * @param width The new width
         * @param height The new height
         * @param interpolation The interpolation method to use
         * @return A new Image with the specified dimensions
         */
        Image resize(int width, int height, int interpolation = cv::INTER_LINEAR) const;

        /**
         * @brief Get a specific channel from the image
         * @param channelIndex The index of the channel to extract
         * @return A new Image containing only the specified channel
         */
        Image getChannel(int channelIndex) const;

        /**
         * @brief Split the image into its channels
         * @return A vector of Images, one for each channel
         */
        std::vector<Image> split() const;

        /**
         * @brief Merge multiple single-channel images into a multi-channel image
         * @param channels The vector of single-channel images to merge
         * @return A new Image with multiple channels
         */
        static Image merge(const std::vector<Image>& channels);

    private:
        cv::Mat m_mat;  // The underlying OpenCV Mat
    };

}
