#include <iostream>
#include <string>
#include <memory>
#include <opencv2/opencv.hpp>

#include "core/node_graph.h"
#include "core/image.h"
#include "core/base_node.h"
#include "input/input_node.h"
#include "core/output_node.h"
#include "nodes/brightness_contrast_node.h"
#include "nodes/channel_splitter_node.h"
#include "nodes/threshold_node.h"

using namespace image_processor;

// Function to display an image with OpenCV
void displayImage(const std::string& windowName, const cv::Mat& image) {
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
    cv::imshow(windowName, image);
}


void processBrightnessContrast(const std::string& inputImagePath, const std::string& outputImagePath) {
    std::cout << "Creating a simple image processing graph..." << std::endl;

    // Create a node graph
    NodeGraph graph;

    // Createing nodes
    InputNode* inputNode = new InputNode("Input");
    BrightnessContrastNode* brightnessNode = new BrightnessContrastNode("Brightness/Contrast", 1.2f, 10.0f);
    OutputNode* outputNode = new OutputNode("Output");

    // Adding nodes to the graph
    graph.addNode(inputNode);
    graph.addNode(brightnessNode);
    graph.addNode(outputNode);

    // Connecting nodes
    graph.connectNodes(inputNode->getId(), 0, brightnessNode->getId(), 0);
    graph.connectNodes(brightnessNode->getId(), 0, outputNode->getId(), 0);
    

    // Load input image
    if (!inputNode->loadImage(inputImagePath)) {
        std::cerr << "Failed to load input image: " << inputImagePath << std::endl;
        return;
    }

    // Process the graph
    graph.processGraph();

    // Save the output image
    if (!outputNode->saveImage(outputImagePath)) {
        std::cerr << "Failed to save output image: " << outputImagePath << std::endl;
    }
    else {
        std::cout << "Output image saved to: " << outputImagePath << std::endl;
    }

    // Display the input and output images
    displayImage("Input Image", inputNode->getImage());
    displayImage("Output Image", outputNode->getImage());

    // Wait for a key press
    cv::waitKey(0);
}



void processChannelSplitter(const std::string& inputImagePath) {
    std::cout << "Extracting RGB channels..." << std::endl;

    NodeGraph graph;

    // Creating nodes
    InputNode* inputNode = new InputNode("Input");
    ChannelSplitterNode* splitterNode = new ChannelSplitterNode("Channel Splitter");
    OutputNode* redOutputNode = new OutputNode("Red Output");
    OutputNode* greenOutputNode = new OutputNode("Green Output");
    OutputNode* blueOutputNode = new OutputNode("Blue Output");

    // Adding nodes to the graph
    graph.addNode(inputNode);
    graph.addNode(splitterNode);
    graph.addNode(redOutputNode);
    graph.addNode(greenOutputNode);
    graph.addNode(blueOutputNode);

    // Connect nodes
    graph.connectNodes(inputNode->getId(), 0, splitterNode->getId(), 0);
    graph.connectNodes(splitterNode->getId(), 2, redOutputNode->getId(), 0);   // Red channel (index 2)
    graph.connectNodes(splitterNode->getId(), 1, greenOutputNode->getId(), 0); // Green channel (index 1)
    graph.connectNodes(splitterNode->getId(), 0, blueOutputNode->getId(), 0);  // Blue channel (index 0)

    // Load input image
    if (!inputNode->loadImage(inputImagePath)) {
        std::cerr << "Failed to load input image: " << inputImagePath << std::endl;
        return;
    }

    // Process the graph
    graph.processGraph();

    // Save the output images
    redOutputNode->saveImage("output_red_channel.png");
    greenOutputNode->saveImage("output_green_channel.png");
    blueOutputNode->saveImage("output_blue_channel.png");

    // Display the input and output images
    displayImage("Input Image", inputNode->getImage());
    displayImage("Red Channel", redOutputNode->getImage());
    displayImage("Green Channel", greenOutputNode->getImage());
    displayImage("Blue Channel", blueOutputNode->getImage());

    // Wait for a key press
    cv::waitKey(0);
}


void processBlur(const std::string& inputImagePath, const std::string& outputImagePath) {
    std::cout << "Creating a simple image processing graph..." << std::endl;

    // Create a node graph
    NodeGraph graph;

    // Creating nodes
    InputNode* inputNode = new InputNode("Input");
    BlurNode* blurNode = new BlurNode("Blur", BlurType::GAUSSIAN, 15);
    OutputNode* outputNode = new OutputNode("Output");

    // Adding nodes to the graph
    graph.addNode(inputNode);
    graph.addNode(blurNode);
    graph.addNode(outputNode);

    // Connecting nodes
    graph.connectNodes(inputNode->getId(), 0, blurNode->getId(), 0);
    graph.connectNodes(blurNode->getId(), 0, outputNode->getId(), 0);

    // Load input image
    if (!inputNode->loadImage(inputImagePath)) {
        std::cerr << "Failed to load input image: " << inputImagePath << std::endl;
        return;
    }

    // Process the graph
    graph.processGraph();

    // Save the output image
    if (!outputNode->saveImage(outputImagePath)) {
        std::cerr << "Failed to save output image: " << outputImagePath << std::endl;
    }
    else {
        std::cout << "Output image saved to: " << outputImagePath << std::endl;
    }

    // Display the input and output images
    displayImage("Input Image", inputNode->getImage());
    displayImage("Output Image", outputNode->getImage());

    // Wait for a key press
    cv::waitKey(0);

}




int main(int argc, char** argv) {
    // Image path
    std::string inputImagePath = "input/input.jpg";

    if (argc > 1) {
        inputImagePath = argv[1];
    }

    
    processBrightnessContrast(inputImagePath, "output/output_simple.jpg");
    processChannelSplitter(inputImagePath);
    processBlur(inputImagePath, "output_blur.jpg");



    std::cout << "All processing complete!" << std::endl;

    return 0;
}
