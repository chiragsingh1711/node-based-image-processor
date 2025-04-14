#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <opencv2/opencv.hpp>

namespace image_processor {
    class Image;
    class BaseNode {
    public:
        BaseNode(const std::string& name);
        virtual ~BaseNode() = default;
        std::string getName() const;

        void setName(const std::string& name);
        int getId() const;

        bool connectOutput(int outputIndex, BaseNode* targetNode, int inputIndex);
        bool disconnectOutput(int outputIndex, BaseNode* targetNode, int inputIndex);

        std::vector<std::pair<BaseNode*, int>> getConnectedNodes(int outputIndex) const;
        std::pair<BaseNode*, int> getInputConnection(int inputIndex) const;

        virtual void process() = 0;
        virtual bool isReady() const;

        virtual int getInputCount() const = 0;
        virtual int getOutputCount() const = 0;

        virtual std::string getInputName(int index) const = 0;
        virtual std::string getOutputName(int index) const = 0;
        
        virtual bool setInputValue(int inputIndex, const cv::Mat& value);
        virtual cv::Mat getOutputValue(int outputIndex) const;

    protected:
        std::string m_name;                  // Node name
        int m_id;                            // Unique node ID
        static int s_nextId;                 // Static counter for generating unique IDs

        // Maps input index to (source node, output index)
        std::unordered_map<int, std::pair<BaseNode*, int>> m_inputs;

        // Maps output index to vector of (target node, input index)
        std::unordered_map<int, std::vector<std::pair<BaseNode*, int>>> m_outputs;

        // Output values stored after processing
        std::unordered_map<int, cv::Mat> m_outputValues;
    };

}
