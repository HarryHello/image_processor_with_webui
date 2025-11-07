#ifndef WEB_SERVER_HPP
#define WEB_SERVER_HPP

#include <crow.h>
#include <opencv2/opencv.hpp>
#include <string>

namespace image_processor
{

    class WebServer
    {
    public:
        WebServer(int port = 8000);
        ~WebServer();

        void start();

    private:
        crow::SimpleApp app_;
        int port_;

        // 图像处理路由处理器
        void setupRoutes();

        // 图像处理功能
        crow::response handleUpload(const crow::request &req);
        crow::response handleProcess(const crow::request &req);
        crow::response handleDownload(const crow::request &req);

        // 辅助处理函数
        void processColorOperations(const crow::json::rvalue &params_json, cv::Mat &processed_image);
        void processGrayscaleOperation(const crow::json::rvalue &colorOp, cv::Mat &processed_image);
        void processBrightnessOperation(const crow::json::rvalue &colorOp, cv::Mat &processed_image);
        void processContrastOperation(const crow::json::rvalue &colorOp, cv::Mat &processed_image);
        void processSaturationOperation(const crow::json::rvalue &colorOp, cv::Mat &processed_image);
        void processInvertOperation(const crow::json::rvalue &colorOp, cv::Mat &processed_image);
        void processScaleOperations(const crow::json::rvalue &params_json, cv::Mat &processed_image);

        // 工具函数
        std::string generateResponse(bool success, const std::string &message, const std::string &data = "");
        cv::Mat stringToImage(const std::string &image_data);
        std::string imageToString(const cv::Mat &image);
    };

} // namespace image_processor

#endif // WEB_SERVER_HPP