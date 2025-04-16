#include "convolution_filter_node.h"
#include <iostream>

namespace image_processor {

    ConvolutionFilterNode::ConvolutionFilterNode(const std::string& name,
        ConvolutionFilterType filterType,
        int kernelSize)
        : BaseNode(name),
        m_filterType(filterType),
        m_kernelSize(validateKernelSize(kernelSize)),
        m_normalizeKernel(true),
        m_borderType(cv::BORDER_DEFAULT) {
        createPredefinedKernel();
    }

    void ConvolutionFilterNode::process() {
        if (!isReady()) {
            std::cerr << "ConvolutionFilterNode::process: Node is not ready to process." << std::endl;
            return;
        }

        auto inputConnection = getInputConnection(0);
        if (inputConnection.first == nullptr) {
            std::cerr << "ConvolutionFilterNode::process: No valid input connection." << std::endl;
            return;
        }

        cv::Mat inputImage = inputConnection.first->getOutputValue(inputConnection.second);
        if (inputImage.empty()) {
            std::cerr << "ConvolutionFilterNode::process: Received empty image from input." << std::endl;
            return;
        }

        cv::Mat outputImage;

        // Apply the convolution filter
        if (m_kernel.empty()) {
            std::cerr << "ConvolutionFilterNode::process: Kernel is empty." << std::endl;
            outputImage = inputImage.clone();
        }
        else {
            // Process each channel separately for multi-channel images
            if (inputImage.channels() > 1) {
                std::vector<cv::Mat> channels;
                cv::split(inputImage, channels);

                std::vector<cv::Mat> filteredChannels;
                for (const auto& channel : channels) {
                    cv::Mat filteredChannel;
                    cv::filter2D(channel, filteredChannel, -1, m_kernel, cv::Point(-1, -1), 0, m_borderType);
                    filteredChannels.push_back(filteredChannel);
                }

                cv::merge(filteredChannels, outputImage);
            }
            else {
                // Single channel image
                cv::filter2D(inputImage, outputImage, -1, m_kernel, cv::Point(-1, -1), 0, m_borderType);
            }
        }

        m_outputValues[0] = outputImage;
    }

    int ConvolutionFilterNode::getInputCount() const {
        return 1; // One input for the source image
    }

    int ConvolutionFilterNode::getOutputCount() const {
        return 1; // One output for the processed image
    }

    std::string ConvolutionFilterNode::getInputName(int index) const {
        if (index == 0) {
            return "Image";
        }
        return "";
    }

    std::string ConvolutionFilterNode::getOutputName(int index) const {
        if (index == 0) {
            return "Filtered Image";
        }
        return "";
    }

    void ConvolutionFilterNode::setFilterType(ConvolutionFilterType filterType) {
        m_filterType = filterType;
        if (m_filterType != ConvolutionFilterType::CUSTOM) {
            createPredefinedKernel();
        }
    }

    ConvolutionFilterType ConvolutionFilterNode::getFilterType() const {
        return m_filterType;
    }

    void ConvolutionFilterNode::setKernelSize(int size) {
        m_kernelSize = validateKernelSize(size);
        if (m_filterType != ConvolutionFilterType::CUSTOM) {
            createPredefinedKernel();
        }
    }

    int ConvolutionFilterNode::getKernelSize() const {
        return m_kernelSize;
    }

    bool ConvolutionFilterNode::setCustomKernel(const cv::Mat& kernel) {
        if (!validateCustomKernel(kernel)) {
            return false;
        }

        m_kernel = kernel.clone();
        m_filterType = ConvolutionFilterType::CUSTOM;
        m_kernelSize = kernel.rows;
        return true;
    }

    cv::Mat ConvolutionFilterNode::getKernel() const {
        return m_kernel;
    }

    void ConvolutionFilterNode::setNormalizeKernel(bool normalize) {
        m_normalizeKernel = normalize;
        if (m_filterType != ConvolutionFilterType::CUSTOM) {
            createPredefinedKernel();
        }
    }

    bool ConvolutionFilterNode::getNormalizeKernel() const {
        return m_normalizeKernel;
    }

    void ConvolutionFilterNode::setBorderType(int borderType) {
        m_borderType = borderType;
    }

    int ConvolutionFilterNode::getBorderType() const {
        return m_borderType;
    }

    void ConvolutionFilterNode::createPredefinedKernel() {
        switch (m_filterType) {
        case ConvolutionFilterType::IDENTITY: {
            // Identity kernel (no effect)
            m_kernel = cv::Mat::zeros(m_kernelSize, m_kernelSize, CV_32F);
            m_kernel.at<float>(m_kernelSize / 2, m_kernelSize / 2) = 1.0f;
            break;
        }

        case ConvolutionFilterType::BOX_BLUR: {
            // Box blur kernel (all elements equal)
            m_kernel = cv::Mat::ones(m_kernelSize, m_kernelSize, CV_32F);
            if (m_normalizeKernel) {
                m_kernel = m_kernel / (float)(m_kernelSize * m_kernelSize);
            }
            break;
        }

        case ConvolutionFilterType::GAUSSIAN_BLUR: {
            // Gaussian blur kernel
            double sigma = 0.3 * ((m_kernelSize - 1) * 0.5 - 1) + 0.8;
            m_kernel = cv::getGaussianKernel(m_kernelSize, sigma, CV_32F);
            m_kernel = m_kernel * m_kernel.t();
            if (!m_normalizeKernel) {
                // Unnormalize the kernel
                double sum = cv::sum(m_kernel)[0];
                m_kernel = m_kernel * (m_kernelSize * m_kernelSize / sum);
            }
            break;
        }

        case ConvolutionFilterType::SHARPEN: {
            // Sharpen kernel
            m_kernel = cv::Mat::zeros(m_kernelSize, m_kernelSize, CV_32F);
            int center = m_kernelSize / 2;

            // Create a simple Laplacian kernel
            if (m_kernelSize == 3) {
                m_kernel.at<float>(0, 1) = -1.0f;
                m_kernel.at<float>(1, 0) = -1.0f;
                m_kernel.at<float>(1, 1) = 5.0f;
                m_kernel.at<float>(1, 2) = -1.0f;
                m_kernel.at<float>(2, 1) = -1.0f;
            }
            else {
                // For larger kernels, create a more general sharpening filter
                // by combining an identity filter with a negative Laplacian
                cv::Mat identity = cv::Mat::zeros(m_kernelSize, m_kernelSize, CV_32F);
                identity.at<float>(center, center) = 1.0f;

                cv::Mat laplacian = cv::Mat::zeros(m_kernelSize, m_kernelSize, CV_32F);
                laplacian.at<float>(center, center) = (float)(m_kernelSize * m_kernelSize - 1);

                for (int i = 0; i < m_kernelSize; ++i) {
                    for (int j = 0; j < m_kernelSize; ++j) {
                        if (i != center || j != center) {
                            laplacian.at<float>(i, j) = -1.0f;
                        }
                    }
                }

                m_kernel = identity + 0.2f * laplacian;
            }

            if (m_normalizeKernel) {
                double sum = cv::sum(m_kernel)[0];
                if (sum != 0) {
                    m_kernel = m_kernel / sum;
                }
            }
            break;
        }

        case ConvolutionFilterType::EDGE_DETECT: {
            // Edge detection kernel (Sobel-like)
            if (m_kernelSize == 3) {
                // 3x3 Sobel kernel
                m_kernel = (cv::Mat_<float>(3, 3) <<
                    -1, -2, -1,
                    0, 0, 0,
                    1, 2, 1);
            }
            else {
                // For larger kernels, create a more general edge detection filter
                m_kernel = cv::Mat::zeros(m_kernelSize, m_kernelSize, CV_32F);
                int center = m_kernelSize / 2;

                // Top half negative, bottom half positive
                for (int i = 0; i < m_kernelSize; ++i) {
                    for (int j = 0; j < m_kernelSize; ++j) {
                        if (i < center) {
                            m_kernel.at<float>(i, j) = -1.0f;
                        }
                        else if (i > center) {
                            m_kernel.at<float>(i, j) = 1.0f;
                        }
                    }
                }
            }

            if (m_normalizeKernel) {
                double sum = cv::sum(m_kernel)[0];
                if (sum != 0) {
                    m_kernel = m_kernel / sum;
                }
            }
            break;
        }

        case ConvolutionFilterType::EMBOSS: {
            // Emboss kernel
            m_kernel = cv::Mat::zeros(m_kernelSize, m_kernelSize, CV_32F);
            int center = m_kernelSize / 2;

            if (m_kernelSize == 3) {
                // 3x3 Emboss kernel
                m_kernel = (cv::Mat_<float>(3, 3) <<
                    -2, -1, 0,
                    -1, 1, 1,
                    0, 1, 2);
            }
            else {
                // For larger kernels, create a more general emboss filter
                for (int i = 0; i < m_kernelSize; ++i) {
                    for (int j = 0; j < m_kernelSize; ++j) {
                        float value = (i - center) + (j - center);
                        m_kernel.at<float>(i, j) = value;
                    }
                }
            }

            if (m_normalizeKernel) {
                double sum = cv::sum(m_kernel)[0];
                if (sum != 0) {
                    m_kernel = m_kernel / sum;
                }
            }
            break;
        }

        case ConvolutionFilterType::CUSTOM:
            // Custom kernel, do nothing
            break;

        default:
            std::cerr << "ConvolutionFilterNode::createPredefinedKernel: Unknown filter type." << std::endl;
            m_kernel = cv::Mat::zeros(m_kernelSize, m_kernelSize, CV_32F);
            m_kernel.at<float>(m_kernelSize / 2, m_kernelSize / 2) = 1.0f;
            break;
        }
    }

    int ConvolutionFilterNode::validateKernelSize(int size) {
        // Kernel size must be positive
        if (size <= 0) {
            size = 3;
        }

        // Kernel size must be odd
        if (size % 2 == 0) {
            size++;
        }

        return size;
    }

    bool ConvolutionFilterNode::validateCustomKernel(const cv::Mat& kernel) {
        // Kernel must not be empty
        if (kernel.empty()) {
            return false;
        }

        // Kernel must be square
        if (kernel.rows != kernel.cols) {
            return false;
        }

        // Kernel dimensions must be odd
        if (kernel.rows % 2 == 0) {
            return false;
        }

        return true;
    }

} // namespace image_processor
