#include "web_server.hpp"
#include "image_io.hpp"
#include "image_scaling.hpp"
#include "color_processing.hpp"
#include "compression.hpp"
#include "logger.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <crow.h>
#include <crow/multipart.h>
#include <string>
#include <string_view>

bool ends_with(const std::string &str, const std::string &suffix)
{
    bool res = str.size() >= suffix.size() &&
               str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    image_processor::Logger::log(image_processor::LogLevel::IP_LOGLV_DEBUG,
                                 "string \"" + (str) + "\" " +
                                     (res ? "does" : "does not") + " end with " +
                                     "\"" + (suffix) + "\".");
    return res;
}

namespace image_processor
{
    const std::string PROJECT_DIR = "../../";

    WebServer::WebServer(int port) : port_(port)
    {
        Logger::log(LogLevel::IP_LOGLV_INFO, "Initializing WebServer on port " + std::to_string(port));
    }

    WebServer::~WebServer()
    {
        Logger::log(LogLevel::IP_LOGLV_INFO, "WebServer destroyed");
    }

    void WebServer::start()
    {
        Logger::log(LogLevel::IP_LOGLV_INFO, "Starting WebServer");
        setupRoutes();

        app_.port(port_).run();
    }

    void WebServer::setupRoutes()
    {
        // 静态文件服务 - 带MIME类型支持
        CROW_ROUTE(app_, "/")
        ([]()
         {
            Logger::log(LogLevel::IP_LOGLV_INFO, "Server gets \"/\", serving index.html");
            std::ifstream file(PROJECT_DIR + "web/index.html");
            if (file.is_open()) { 
                std::stringstream buffer;
                buffer << file.rdbuf();
                crow::response res(buffer.str());
                res.set_header("Content-Type", "text/html");
                return res;
            }
            return crow::response(404, "Web interface not found"); });

        // 通用静态文件路由 - 带MIME类型支持
        CROW_ROUTE(app_, "/<string>")
        ([](std::string filename)
         {
            Logger::log(LogLevel::IP_LOGLV_INFO, "Server gets \"/" + filename + "\", serving " + filename);
            std::string path = PROJECT_DIR + "web/" + filename;
            std::ifstream file(path, std::ios::binary);
            if (file.is_open()) { 
                std::stringstream buffer;
                buffer << file.rdbuf();
                
                crow::response res(buffer.str());
                
                // 设置MIME类型
                if (ends_with(filename, ".html")) res.set_header("Content-Type", "text/html");
                else if (ends_with(filename, ".css")) res.set_header("Content-Type", "text/css");
                else if (ends_with(filename, ".js")) res.set_header("Content-Type", "application/javascript");
                else if (ends_with(filename, ".png")) res.set_header("Content-Type", "image/png");
                else if (ends_with(filename, ".jpg") || ends_with(filename, ".jpeg")) res.set_header("Content-Type", "image/jpeg");
                
                return res;
            }
            return crow::response(404, "File not found"); });

        CROW_ROUTE(app_, "/js/<string>")
        ([](std::string filename)
         {
            Logger::log(LogLevel::IP_LOGLV_INFO, "Server gets \"/js/"+ filename + "\", serving " + filename);
            std::string path = PROJECT_DIR + "web/js/" + filename;
            std::ifstream file(path, std::ios::binary);
            if (file.is_open()) { 
                std::stringstream buffer;
                buffer << file.rdbuf();
                
                crow::response res(buffer.str());
                res.set_header("Content-Type", "application/javascript"); // 确保JavaScript模块有正确的MIME类型
                return res;
            }
            return crow::response(404, "File not found"); });
        // API路由
        CROW_ROUTE(app_, "/api/upload").methods("POST"_method)([this](const crow::request &req)
                                                               { return handleUpload(req); });

        CROW_ROUTE(app_, "/api/process").methods("POST"_method)([this](const crow::request &req)
                                                                { return handleProcess(req); });

        CROW_ROUTE(app_, "/api/download").methods("GET"_method)([this](const crow::request &req)
                                                                { return handleDownload(req); });

        // 静态资源路由 - 带MIME类型支持
        CROW_ROUTE(app_, "/<string>/<string>")
        ([](std::string dir, std::string filename)
         {
            Logger::log(LogLevel::IP_LOGLV_INFO, "Server gets \"/"+ dir + "/" + filename + "\", serving " + filename);
            std::string path = PROJECT_DIR + "web/" + dir + "/" + filename;
            std::ifstream file(path, std::ios::binary);
            if (file.is_open()) { 
                std::stringstream buffer;
                buffer << file.rdbuf();
                
                crow::response res(buffer.str());
                
                // 设置MIME类型
                if (ends_with(filename, ".html")) res.set_header("Content-Type", "text/html");
                else if (ends_with(filename, ".css")) res.set_header("Content-Type", "text/css");
                else if (ends_with(filename, ".js")) res.set_header("Content-Type", "application/javascript");
                else if (ends_with(filename, ".png")) res.set_header("Content-Type", "image/png");
                else if (ends_with(filename, ".jpg") || ends_with(filename, ".jpeg")) res.set_header("Content-Type", "image/jpeg");
                
                return res;
            }
            return crow::response(404, "File not found"); });
    }

    crow::response WebServer::handleUpload(const crow::request &req)
    {
        try
        {
            Logger::log(LogLevel::IP_LOGLV_INFO, "Server gets request.");
            // 解析multipart/form-data
            crow::multipart::message msg(req);

            // 检查是否有文件上传
            if (msg.part_map.find("image") != msg.part_map.end())
            {
                // 获取文件数据
                auto &part = msg.part_map.find("image")->second;

                // 将文件数据保存到临时文件
                std::string temp_filename = "temp_upload_" + std::to_string(std::time(nullptr)) + ".png";
                std::ofstream temp_file(temp_filename, std::ios::binary);
                if (temp_file.is_open())
                {
                    temp_file.write(part.body.data(), part.body.size());
                    temp_file.close();

                    // 保存文件名到会话或临时存储中，供后续处理使用
                    // 这里简化处理，直接返回成功响应
                    crow::json::wvalue response;
                    response["success"] = true;
                    response["message"] = "Upload successful";
                    response["filename"] = temp_filename;
                    return crow::response(response.dump());
                }
            }

            Logger::log(LogLevel::IP_LOGLV_INFO, "Handling image upload");
            return crow::response(generateResponse(true, "Upload successful"));
        }
        catch (const std::exception &e)
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Upload failed: " + std::string(e.what()));
            return crow::response(generateResponse(false, "Upload failed: " + std::string(e.what())));
        }
    }

    // 处理颜色操作的辅助方法
    void WebServer::processColorOperations(const crow::json::rvalue &params_json, cv::Mat &processed_image)
    {
        Logger::log(LogLevel::IP_LOGLV_INFO, "Server processing color operations request.");
        // 检查是否有colorOps参数
        if (!params_json.has("colorOps"))
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "No colorOps found in parameters.");
            return;
        }

        // 检查colorOps是否为数组
        if (params_json["colorOps"].t() != crow::json::type::List)
        {
            // 处理单个对象情况
            const auto &colorOp = params_json["colorOps"];

            // 检查是否有type参数
            if (colorOp.has("type"))
            {
                std::string operation = colorOp["type"].s();

                if (operation == "grayscale")
                {
                    processGrayscaleOperation(colorOp, processed_image);
                }
                else if (operation == "brightness")
                {
                    processBrightnessOperation(colorOp, processed_image);
                }
                else if (operation == "contrast")
                {
                    processContrastOperation(colorOp, processed_image);
                }
                else if (operation == "saturation")
                {
                    processSaturationOperation(colorOp, processed_image);
                }
                else if (operation == "invert")
                {
                    processInvertOperation(colorOp, processed_image);
                }
            }
            return;
        }

        // 遍历所有颜色图层，从上到下处理
        for (const auto &colorOp : params_json["colorOps"])
        {
            // 检查是否有type参数
            if (!colorOp.has("type"))
            {
                continue;
            }

            std::string operation = colorOp["type"].s();

            if (operation == "grayscale")
            {
                processGrayscaleOperation(colorOp, processed_image);
            }
            else if (operation == "brightness")
            {
                processBrightnessOperation(colorOp, processed_image);
            }
            else if (operation == "contrast")
            {
                processContrastOperation(colorOp, processed_image);
            }
            else if (operation == "saturation")
            {
                processSaturationOperation(colorOp, processed_image);
            }
            else if (operation == "invert")
            {
                processInvertOperation(colorOp, processed_image);
            }
        }
    }

    // 处理灰度转换操作
    void WebServer::processGrayscaleOperation(const crow::json::rvalue &colorOp, cv::Mat &processed_image)
    {
        // 检查是否有自定义灰度参数
        if (colorOp.has("params") && colorOp["params"].has("type"))
        {
            std::string type = colorOp["params"]["type"].s();

            if (type == "custom" && colorOp["params"].has("r") && colorOp["params"].has("g") && colorOp["params"].has("b"))
            {
                double r = colorOp["params"]["r"].d();
                double g = colorOp["params"]["g"].d();
                double b = colorOp["params"]["b"].d();
                processed_image = ColorProcessing::convertToGrayscale(processed_image, r, g, b);
            }
            else
            {
                processed_image = ColorProcessing::convertToGrayscale(processed_image);
            }
        }
        else
        {
            processed_image = ColorProcessing::convertToGrayscale(processed_image);
        }
    }

    // 处理亮度调整操作
    void WebServer::processBrightnessOperation(const crow::json::rvalue &colorOp, cv::Mat &processed_image)
    {
        if (colorOp.has("params") && colorOp["params"].has("value"))
        {
            int brightness = colorOp["params"]["value"].i();
            processed_image = ColorProcessing::adjustBrightness(processed_image, brightness);
        }
    }

    // 处理对比度调整操作
    void WebServer::processContrastOperation(const crow::json::rvalue &colorOp, cv::Mat &processed_image)
    {
        if (colorOp.has("params") && colorOp["params"].has("value"))
        {
            int contrast = colorOp["params"]["value"].i();
            processed_image = ColorProcessing::adjustContrast(processed_image, contrast);
        }
    }

    // 处理饱和度调整操作
    void WebServer::processSaturationOperation(const crow::json::rvalue &colorOp, cv::Mat &processed_image)
    {
        if (colorOp.has("params") && colorOp["params"].has("value"))
        {
            int saturation = colorOp["params"]["value"].i();
            processed_image = ColorProcessing::adjustSaturation(processed_image, saturation);
        }
    }

    // 处理反色操作
    void WebServer::processInvertOperation(const crow::json::rvalue &colorOp, cv::Mat &processed_image)
    {
        processed_image = ColorProcessing::invertColors(processed_image);
    }

    // 处理缩放操作的辅助方法
    void WebServer::processScaleOperations(const crow::json::rvalue &params_json, cv::Mat &processed_image)
    {
        if (!params_json.has("scaleOp"))
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "No scaleOp found in parameters.");
            return;
        }

        const crow::json::rvalue &scaleOp = params_json["scaleOp"];
        if (!scaleOp.has("type"))
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "No type found in scaleOp parameters.");
            return;
        }

        std::string scale_type = scaleOp["type"].s();

        if (scale_type == "factor" && scaleOp.has("factor"))
        {
            double factor = scaleOp["factor"].d();
            processed_image = ImageScaling::scaleImageByFactor(processed_image, factor);
        }
        else if (scale_type == "dimensions" && scaleOp.has("width") && scaleOp.has("height"))
        {
            int width = scaleOp["width"].i();
            int height = scaleOp["height"].i();
            std::string method = params_json.has("method") ? params_json["method"].s() : std::string("default");
            processed_image = ImageScaling::scaleImage(processed_image, width, height, method);
        }
    }

    crow::response WebServer::handleProcess(const crow::request &req)
    {
        try
        {
            // 解析multipart/form-data
            crow::multipart::message msg(req);

            // 检查是否有图像数据
            if (msg.part_map.find("image") == msg.part_map.end())
            {
                return crow::response(400, generateResponse(false, "No image provided"));
            }

            // 获取图像数据
            auto &image_part = msg.part_map.find("image")->second;

            // 将图像数据转换为cv::Mat
            std::vector<uchar> image_data(image_part.body.begin(), image_part.body.end());
            cv::Mat image = cv::imdecode(image_data, cv::IMREAD_COLOR);

            if (image.empty())
            {
                return crow::response(400, generateResponse(false, "Invalid image data"));
            }

            // 获取参数
            cv::Mat processed_image = image.clone();
            crow::json::rvalue params_json = crow::json::load("{}");
            bool has_params = false;

            // 检查是否有参数
            if (msg.part_map.find("params") != msg.part_map.end())
            {
                auto &params_part = msg.part_map.find("params")->second;
                std::string params_str(params_part.body.begin(), params_part.body.end());

                // 直接解析为rvalue类型
                auto parsed_json = crow::json::load(params_str);
                if (parsed_json.t() != crow::json::type::Null)
                {
                    params_json = parsed_json;
                    has_params = true;
                }
            }

            // 先处理颜色操作（支持图层系统）
            if (has_params)
            {
                // 添加调试日志，输出整个params_json内容
                Logger::log(LogLevel::IP_LOGLV_INFO, "Received params: " + (std::string)params_json);

                // 检查是否有colorOps参数
                if (params_json.has("colorOps"))
                {
                    Logger::log(LogLevel::IP_LOGLV_INFO, "Processing color operations");
                    processColorOperations(params_json, processed_image);
                }
                else
                {
                    Logger::log(LogLevel::IP_LOGLV_INFO, "No colorOps found in params");
                }

                // 后处理缩放操作
                if (params_json.has("scaleOp"))
                {
                    Logger::log(LogLevel::IP_LOGLV_INFO, "Processing scale operations");
                    processScaleOperations(params_json, processed_image);
                }
                else
                {
                    Logger::log(LogLevel::IP_LOGLV_INFO, "No scaleOp found in params");
                }
            }

            if (processed_image.empty())
            {
                return crow::response(500, generateResponse(false, "Image processing failed"));
            }

            // 将处理后的图像转换为base64字符串
            std::string result_data = imageToString(processed_image);

            return crow::response(generateResponse(true, "Processing successful", result_data));
        }
        catch (const std::exception &e)
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Processing failed: " + std::string(e.what()));
            return crow::response(generateResponse(false, "Processing failed: " + std::string(e.what())));
        }
    }

    crow::response WebServer::handleDownload(const crow::request &req)
    {
        try
        {
            // 处理图像下载请求
            Logger::log(LogLevel::IP_LOGLV_INFO, "Handling image download");

            // 这里应该返回处理后的图像文件
            return crow::response(generateResponse(true, "Download ready"));
        }
        catch (const std::exception &e)
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Download failed: " + std::string(e.what()));
            return crow::response(generateResponse(false, "Download failed: " + std::string(e.what())));
        }
    }

    std::string WebServer::generateResponse(bool success, const std::string &message, const std::string &data)
    {
        crow::json::wvalue response;
        response["success"] = success;
        response["message"] = message;
        if (!data.empty())
        {
            response["data"] = data;
        }
        return response.dump();
    }

    cv::Mat WebServer::stringToImage(const std::string &image_data)
    {
        // 使用Crow库的base64解码功能
        std::string decoded_data = crow::utility::base64decode(image_data);
        std::vector<uchar> data(decoded_data.begin(), decoded_data.end());
        cv::Mat image = cv::imdecode(data, cv::IMREAD_COLOR);
        return image;
    }

    std::string WebServer::imageToString(const cv::Mat &image)
    {
        // 将图像编码为base64字符串
        std::vector<uchar> buffer;
        cv::imencode(".png", image, buffer);
        std::string raw_data(buffer.begin(), buffer.end());
        // 使用Crow库的base64编码功能
        return crow::utility::base64encode(raw_data, raw_data.size());
    }

} // namespace image_processor