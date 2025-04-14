#include "image.h"
#include <iostream>

namespace image_processor {

    Image::Image() : m_mat() {
    }

    Image::Image(int width, int height, int type) : m_mat(height, width, type) {
    }

    Image::Image(const cv::Mat& mat) : m_mat(mat.clone()) {
    }

    Image::Image(const Image& other) : m_mat(other.m_mat.clone()) {
    }

    Image::Image(Image&& other) noexcept : m_mat(std::move(other.m_mat)) {
    }

    Image& Image::operator=(const Image& other) {
        if (this != &other) {
            m_mat = other.m_mat.clone();
        }
        return *this;
    }

    Image& Image::operator=(Image&& other) noexcept {
        if (this != &other) {
            m_mat = std::move(other.m_mat);
        }
        return *this;
    }

    bool Image::load(const std::string& filePath) {
        m_mat = cv::imread(filePath, cv::IMREAD_UNCHANGED);
        return !isEmpty();
    }

    bool Image::save(const std::string& filePath) const {
        if (isEmpty()) {
            std::cerr << "Image::save: Cannot save empty image." << std::endl;
            return false;
        }

        try {
            return cv::imwrite(filePath, m_mat);
        }
        catch (const cv::Exception& e) {
            std::cerr << "Image::save: OpenCV exception: " << e.what() << std::endl;
            return false;
        }
    }

    int Image::getWidth() const {
        return m_mat.cols;
    }

    int Image::getHeight() const {
        return m_mat.rows;
    }

    int Image::getChannels() const {
        return m_mat.channels();
    }

    int Image::getType() const {
        return m_mat.type();
    }

    bool Image::isEmpty() const {
        return m_mat.empty();
    }

    cv::Mat Image::getMat() const {
        return m_mat;
    }

    void Image::setMat(const cv::Mat& mat) {
        m_mat = mat.clone();
    }

    Image Image::clone() const {
        return Image(m_mat.clone());
    }

    Image Image::toGrayscale() const {
        if (isEmpty()) {
            return Image();
        }

        cv::Mat grayMat;
        if (getChannels() > 1) {
            cv::cvtColor(m_mat, grayMat, cv::COLOR_BGR2GRAY);
        }
        else {
            grayMat = m_mat.clone();
        }

        return Image(grayMat);
    }

    Image Image::resize(int width, int height, int interpolation) const {
        if (isEmpty()) {
            return Image();
        }

        cv::Mat resizedMat;
        cv::resize(m_mat, resizedMat, cv::Size(width, height), 0, 0, interpolation);

        return Image(resizedMat);
    }

    Image Image::getChannel(int channelIndex) const {
        if (isEmpty()) {
            return Image();
        }

        if (channelIndex < 0 || channelIndex >= getChannels()) {
            std::cerr << "Image::getChannel: Channel index out of range." << std::endl;
            return Image();
        }

        std::vector<cv::Mat> channels;
        cv::split(m_mat, channels);

        return Image(channels[channelIndex]);
    }

    std::vector<Image> Image::split() const {
        if (isEmpty()) {
            return {};
        }

        std::vector<cv::Mat> channels;
        cv::split(m_mat, channels);

        std::vector<Image> result;
        for (const auto& channel : channels) {
            result.push_back(Image(channel));
        }

        return result;
    }

    Image Image::merge(const std::vector<Image>& channels) {
        if (channels.empty()) {
            return Image();
        }

        std::vector<cv::Mat> mats;
        for (const auto& channel : channels) {
            mats.push_back(channel.getMat());
        }

        cv::Mat mergedMat;
        cv::merge(mats, mergedMat);

        return Image(mergedMat);
    }

}
