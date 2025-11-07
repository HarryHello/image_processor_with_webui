#ifndef COLOR_PROCESSING_HPP
#define COLOR_PROCESSING_HPP

#include <opencv2/core/mat.hpp>

namespace image_processor
{

    class ColorProcessing
    {
    public:
        // 将彩色图像转换为灰度图像
        static cv::Mat convertToGrayscale(const cv::Mat &color_image);

        // 将彩色图像转换为灰度图像（自定义权重）
        static cv::Mat convertToGrayscale(const cv::Mat &color_image, double weight_r, double weight_g, double weight_b);

        // 将灰度图像转换为彩色图像（伪彩色）
        static cv::Mat convertToColor(const cv::Mat &grayscale_image);

        // 调整图像亮度和对比度
        static cv::Mat adjustBrightnessContrast(const cv::Mat &image, double alpha, double beta);

        // 调整图像亮度
        static cv::Mat adjustBrightness(const cv::Mat &image, int brightness);

        // 调整图像对比度
        static cv::Mat adjustContrast(const cv::Mat &image, int contrast);

        // 调整图像饱和度
        static cv::Mat adjustSaturation(const cv::Mat &image, int saturation);

        // 图像直方图均衡化
        static cv::Mat equalizeHistogram(const cv::Mat &image);

        // 反色处理
        static cv::Mat invertColors(const cv::Mat &image);
    };

} // namespace image_processor

#endif // COLOR_PROCESSING_HPP