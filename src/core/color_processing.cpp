#include <opencv2/imgproc.hpp>
#include "color_processing.hpp"
#include "logger.hpp"

namespace image_processor
{

    cv::Mat ColorProcessing::convertToGrayscale(const cv::Mat &color_image)
    {
        if (color_image.empty())
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Cannot convert empty image to grayscale");
            return cv::Mat();
        }

        // 检查图像是否已经是灰度图像
        if (color_image.channels() == 1)
        {
            Logger::log(LogLevel::IP_LOGLV_INFO, "Image is already grayscale");
            return color_image.clone();
        }

        cv::Mat grayscale_image;
        cv::cvtColor(color_image, grayscale_image, cv::COLOR_BGR2GRAY);

        Logger::log(LogLevel::IP_LOGLV_INFO, "Image converted to grayscale successfully");
        return grayscale_image;
    }

    cv::Mat ColorProcessing::convertToGrayscale(const cv::Mat &color_image, double weight_r, double weight_g, double weight_b)
    {
        if (color_image.empty())
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Cannot convert empty image to grayscale");
            return cv::Mat();
        }

        // 检查图像是否已经是灰度图像
        if (color_image.channels() == 1)
        {
            Logger::log(LogLevel::IP_LOGLV_INFO, "Image is already grayscale");
            return color_image.clone();
        }

        // 检查权重是否有效
        double sum_weights = weight_r + weight_g + weight_b;
        if (sum_weights == 0)
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Invalid weights for grayscale conversion: sum is zero");
            return cv::Mat();
        }

        // 归一化权重
        weight_r /= sum_weights;
        weight_g /= sum_weights;
        weight_b /= sum_weights;

        cv::Mat grayscale_image(color_image.rows, color_image.cols, CV_8UC1);

        // 手动计算加权平均
        for (int i = 0; i < color_image.rows; ++i)
        {
            for (int j = 0; j < color_image.cols; ++j)
            {
                cv::Vec3b pixel = color_image.at<cv::Vec3b>(i, j);
                uchar blue = pixel[0];
                uchar green = pixel[1];
                uchar red = pixel[2];

                // 使用加权平均计算灰度值
                uchar gray_value = static_cast<uchar>(weight_r * red + weight_g * green + weight_b * blue);
                grayscale_image.at<uchar>(i, j) = gray_value;
            }
        }

        Logger::log(LogLevel::IP_LOGLV_INFO, "Image converted to grayscale with custom weights successfully");
        return grayscale_image;
    }

    cv::Mat ColorProcessing::convertToColor(const cv::Mat &grayscale_image)
    {
        if (grayscale_image.empty())
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Cannot convert empty grayscale image to color");
            return cv::Mat();
        }

        cv::Mat color_image;
        cv::cvtColor(grayscale_image, color_image, cv::COLOR_GRAY2BGR);

        Logger::log(LogLevel::IP_LOGLV_INFO, "Grayscale image converted to color successfully");
        return color_image;
    }

    cv::Mat ColorProcessing::adjustBrightnessContrast(const cv::Mat &image, double alpha, double beta)
    {
        if (image.empty())
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Cannot adjust brightness/contrast of empty image");
            return cv::Mat();
        }

        cv::Mat adjusted_image;
        image.convertTo(adjusted_image, -1, alpha, beta);

        Logger::log(LogLevel::IP_LOGLV_INFO, "Brightness and contrast adjusted successfully");
        return adjusted_image;
    }

    cv::Mat ColorProcessing::adjustBrightness(const cv::Mat &image, int brightness)
    {
        if (image.empty())
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Cannot adjust brightness of empty image");
            return cv::Mat();
        }

        // 将亮度值从 [-100, 100] 转换为 [0, 200] 的范围，然后转换为 [0, 2.0] 的倍数
        double beta = brightness * 2.0;

        cv::Mat adjusted_image;
        image.convertTo(adjusted_image, -1, 1.0, beta);

        Logger::log(LogLevel::IP_LOGLV_INFO, "Brightness adjusted successfully");
        return adjusted_image;
    }

    cv::Mat ColorProcessing::adjustContrast(const cv::Mat &image, int contrast)
    {
        if (image.empty())
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Cannot adjust contrast of empty image");
            return cv::Mat();
        }

        // 将对比度值从 [-100, 100] 转换为 [0, 200] 的范围，然后转换为 [0, 2.0] 的倍数
        double alpha = (contrast + 100.0) / 100.0;

        cv::Mat adjusted_image;
        image.convertTo(adjusted_image, -1, alpha, 0.0);

        Logger::log(LogLevel::IP_LOGLV_INFO, "Contrast adjusted successfully");
        return adjusted_image;
    }

    cv::Mat ColorProcessing::adjustSaturation(const cv::Mat &image, int saturation)
    {
        if (image.empty())
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Cannot adjust saturation of empty image");
            return cv::Mat();
        }

        // 检查图像是否是彩色图像
        if (image.channels() != 3)
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Saturation adjustment requires a color image");
            return image.clone();
        }

        cv::Mat hsv_image;
        cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);

        // 将饱和度值从 [-100, 100] 转换为调整因子
        double factor = (saturation + 100.0) / 100.0;

        // 调整饱和度通道
        for (int i = 0; i < hsv_image.rows; ++i)
        {
            for (int j = 0; j < hsv_image.cols; ++j)
            {
                cv::Vec3b pixel = hsv_image.at<cv::Vec3b>(i, j);
                // 像素[1]是饱和度通道 (0-255)
                double new_saturation = pixel[1] * factor;
                // 确保值在0-255范围内
                pixel[1] = static_cast<uchar>(std::min(255.0, std::max(0.0, new_saturation)));
                hsv_image.at<cv::Vec3b>(i, j) = pixel;
            }
        }

        cv::Mat result_image;
        cv::cvtColor(hsv_image, result_image, cv::COLOR_HSV2BGR);

        Logger::log(LogLevel::IP_LOGLV_INFO, "Image saturation adjusted successfully");
        return result_image;
    }

    cv::Mat ColorProcessing::equalizeHistogram(const cv::Mat &image)
    {
        if (image.empty())
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Cannot equalize histogram of empty image");
            return cv::Mat();
        }

        cv::Mat equalized_image;
        if (image.channels() == 1)
        {
            // 灰度图像直方图均衡化
            cv::equalizeHist(image, equalized_image);
        }
        else
        {
            // 彩色图像直方图均衡化
            cv::Mat ycrcb_image;
            cv::cvtColor(image, ycrcb_image, cv::COLOR_BGR2YCrCb);

            std::vector<cv::Mat> channels;
            cv::split(ycrcb_image, channels);

            // 仅对亮度通道进行均衡化
            cv::equalizeHist(channels[0], channels[0]);

            // 合并通道
            cv::merge(channels, ycrcb_image);
            cv::cvtColor(ycrcb_image, equalized_image, cv::COLOR_YCrCb2BGR);
        }

        Logger::log(LogLevel::IP_LOGLV_INFO, "Image histogram equalized successfully");
        return equalized_image;
    }

    cv::Mat ColorProcessing::invertColors(const cv::Mat &image)
    {
        if (image.empty())
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Cannot invert colors of empty image");
            return cv::Mat();
        }

        cv::Mat inverted_image;
        cv::bitwise_not(image, inverted_image);

        Logger::log(LogLevel::IP_LOGLV_INFO, "Colors inverted successfully");
        return inverted_image;
    }

} // namespace image_processor