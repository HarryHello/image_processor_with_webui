#include "image_scaling.hpp"
#include "logger.hpp"
#include <opencv2/imgproc.hpp>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <string>

namespace image_processor
{

    cv::Mat ImageScaling::scaleImage(const cv::Mat &image, int new_width, int new_height,
                                     const std::string &method)
    {
        if (image.empty())
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Input image is empty");
            throw std::invalid_argument("Input image is empty");
        }

        if (new_width <= 0 || new_height <= 0)
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "New dimensions must be positive");
            throw std::invalid_argument("New dimensions must be positive");
        }

        // 根据缩放要求动态选择插值算法
        double scale_factor_x = static_cast<double>(new_width) / image.cols;
        double scale_factor_y = static_cast<double>(new_height) / image.rows;
        double avg_scale_factor = (scale_factor_x + scale_factor_y) / 2.0;

        InterpolationMethod interp_method = selectInterpolationMethod(avg_scale_factor, method);

        int interpolation = convertInterpolationMethod(interp_method);
        cv::Mat result;
        cv::resize(image, result, cv::Size(new_width, new_height), 0, 0, interpolation);
        Logger::log(LogLevel::IP_LOGLV_INFO,
                    "Image scaled: " + std::to_string(new_width) + "x" + std::to_string(new_height) + " using method: " + method);
        return result;
    }

    cv::Mat ImageScaling::scaleImageByFactor(const cv::Mat &image, double scale_factor,
                                             const std::string &method)
    {
        if (image.empty())
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Input image is empty");
            throw std::invalid_argument("Input image is empty");
        }

        if (scale_factor <= 0.0)
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Scale factor must be positive");
            throw std::invalid_argument("Scale factor must be positive");
        }

        int new_width = static_cast<int>(image.cols * scale_factor);
        int new_height = static_cast<int>(image.rows * scale_factor);

        InterpolationMethod interp_method = selectInterpolationMethod(scale_factor, method);

        return scaleImage(image, new_width, new_height, method);
    }

    cv::Mat ImageScaling::enlargeImage(const cv::Mat &image, int new_width, int new_height,
                                       const std::string &method)
    {
        // 确保新尺寸大于原尺寸
        if (new_width <= image.cols || new_height <= image.rows)
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "New dimensions must be larger than original for enlargement");
            throw std::invalid_argument("New dimensions must be larger than original for enlargement");
        }

        // method 有默认参数 "cubic"
        return scaleImage(image, new_width, new_height, method);
    }

    cv::Mat ImageScaling::shrinkImage(const cv::Mat &image, int new_width, int new_height,
                                      const std::string &method)
    {
        // 确保新尺寸小于原尺寸
        if (new_width >= image.cols || new_height >= image.rows)
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "New dimensions must be smaller than original for shrinking");
            throw std::invalid_argument("New dimensions must be smaller than original for shrinking");
        }

        // method 有默认参数 "area"
        return scaleImage(image, new_width, new_height, method);
    }

    // 辅助函数：将字符串转换为插值方法枚举
    InterpolationMethod ImageScaling::stringToInterpolationMethod(const std::string &method_str)
    {
        // 将输入字符串转换为小写以进行比较
        std::string lower_method = method_str;
        std::transform(lower_method.begin(), lower_method.end(), lower_method.begin(), ::tolower);
        Logger::log(LogLevel::IP_LOGLV_INFO, "Converting interpolation method string: " + lower_method);

        if (lower_method == "nearest" || lower_method == "nearest_neighbor")
        {
            return InterpolationMethod::NEAREST;
        }
        else if (lower_method == "linear" || lower_method == "bilinear")
        {
            return InterpolationMethod::LINEAR;
        }
        else if (lower_method == "cubic" || lower_method == "bicubic")
        {
            return InterpolationMethod::CUBIC;
        }
        else if (lower_method == "area")
        {
            return InterpolationMethod::AREA;
        }
        else if (lower_method == "lanczos")
        {
            return InterpolationMethod::LANCZOS;
        }
        else
        {
            // 默认返回LINEAR
            return InterpolationMethod::LINEAR;
        }
    }

    // 辅助函数：根据缩放要求动态选择插值算法
    InterpolationMethod ImageScaling::selectInterpolationMethod(double scale_factor, const std::string &method_str)
    {
        // 如果提供了具体的插值方法字符串，则使用该方法
        if (!method_str.empty() && method_str != "default")
        {
            Logger::log(LogLevel::IP_LOGLV_INFO, "Using specified interpolation method: " + method_str);
            return stringToInterpolationMethod(method_str);
        }

        // 根据缩放因子动态选择默认插值方法
        if (scale_factor > 1.0)
        {
            // 放大图像时，默认使用双三次插值
            Logger::log(LogLevel::IP_LOGLV_INFO, "Default interpolation method for enlargement: cubic");
            return InterpolationMethod::CUBIC;
        }
        else if (scale_factor < 1.0)
        {
            // 缩小图像时，默认使用区域插值
            Logger::log(LogLevel::IP_LOGLV_INFO, "Default interpolation method for shrinking: area");
            return InterpolationMethod::AREA;
        }
        else
        {
            // 1:1缩放时，使用邻近插值（实际是不插值）
            Logger::log(LogLevel::IP_LOGLV_INFO, "Default interpolation method for 1:1 scaling: nearest");
            return InterpolationMethod::NEAREST;
        }
    }

    // 辅助函数：将自定义的插值方法转换为OpenCV的插值方法
    int ImageScaling::convertInterpolationMethod(InterpolationMethod method)
    {
        Logger::log(LogLevel::IP_LOGLV_INFO, "Converting interpolation method: " + std::to_string(static_cast<int>(method)));
        switch (method)
        {
        case InterpolationMethod::NEAREST:
            return cv::INTER_NEAREST;
        case InterpolationMethod::LINEAR:
            return cv::INTER_LINEAR;
        case InterpolationMethod::CUBIC:
            return cv::INTER_CUBIC;
        case InterpolationMethod::AREA:
            return cv::INTER_AREA;
        case InterpolationMethod::LANCZOS:
            return cv::INTER_LANCZOS4;
        default:
            return cv::INTER_LINEAR;
        }
    }

} // namespace image_processor