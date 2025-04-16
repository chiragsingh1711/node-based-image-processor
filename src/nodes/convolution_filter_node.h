#pragma once

#include "base_node.h"
#include <opencv2/opencv.hpp>
#include <vector>

namespace image_processor {

    /**
     * @brief Enumeration of predefined convolution filter types
     */
    enum class ConvolutionFilterType {
        CUSTOM,         // Custom kernel defined by user
        IDENTITY,       // Identity filter (no effect)
        BOX_BLUR,       // Box blur filter
        GAUSSIAN_BLUR,  // Gaussian blur filter
        SHARPEN,        // Sharpen filter
        EDGE_DETECT,    // Edge detection filter
        EMBOSS          // Emboss filter
    };

    /**
     * @brief Node for applying convolution filters to an image
     *
     * This node applies a convolution filter to an input image using a kernel.
     * It supports both predefined filters and custom kernels.
     */
    class ConvolutionFilterNode : public BaseNode {
    public:
        /**
         * @brief Constructor for ConvolutionFilterNode
         * @param name The name of the node
         * @param filterType Initial filter type (default: IDENTITY)
         * @param kernelSize Initial kernel size for predefined filters (default: 3)
         */
        ConvolutionFilterNode(const std::string& name = "Convolution Filter",
            ConvolutionFilterType filterType = ConvolutionFilterType::IDENTITY,
            int kernelSize = 3);

        /**
         * @brief Destructor
         */
        virtual ~ConvolutionFilterNode() = default;

        /**
         * @brief Process the node
         *
         * Applies the convolution filter to the input image
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
         * @brief Set the filter type
         * @param filterType The new filter type
         */
        void setFilterType(ConvolutionFilterType filterType);

        /**
         * @brief Get the current filter type
         * @return The current filter type
         */
        ConvolutionFilterType getFilterType() const;

        /**
         * @brief Set the kernel size for predefined filters
         * @param size The new kernel size (must be odd)
         */
        void setKernelSize(int size);

        /**
         * @brief Get the current kernel size
         * @return The current kernel size
         */
        int getKernelSize() const;

        /**
         * @brief Set a custom kernel
         * @param kernel The new custom kernel (must be a square matrix with odd dimensions)
         * @return True if the kernel was set successfully, false otherwise
         */
        bool setCustomKernel(const cv::Mat& kernel);

        /**
         * @brief Get the current kernel
         * @return The current kernel
         */
        cv::Mat getKernel() const;

        /**
         * @brief Set whether to normalize the kernel
         * @param normalize Whether to normalize the kernel
         */
        void setNormalizeKernel(bool normalize);

        /**
         * @brief Get whether the kernel is being normalized
         * @return True if the kernel is being normalized, false otherwise
         */
        bool getNormalizeKernel() const;

        /**
         * @brief Set the border type for convolution
         * @param borderType The new border type
         */
        void setBorderType(int borderType);

        /**
         * @brief Get the current border type
         * @return The current border type
         */
        int getBorderType() const;

    private:
        ConvolutionFilterType m_filterType;  // Type of filter to apply
        int m_kernelSize;                    // Size of the kernel for predefined filters
        cv::Mat m_kernel;                    // The convolution kernel
        bool m_normalizeKernel;              // Whether to normalize the kernel
        int m_borderType;                    // Border type for convolution

        /**
         * @brief Create a predefined kernel based on the current filter type and kernel size
         */
        void createPredefinedKernel();

        /**
         * @brief Validate the kernel size (must be odd)
         * @param size The size to validate
         * @return A valid kernel size (odd)
         */
        int validateKernelSize(int size);

        /**
         * @brief Validate a custom kernel (must be square with odd dimensions)
         * @param kernel The kernel to validate
         * @return True if the kernel is valid, false otherwise
         */
        bool validateCustomKernel(const cv::Mat& kernel);
    };

} // namespace image_processor
