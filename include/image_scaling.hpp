#ifndef IMAGE_SCALING_HPP
#define IMAGE_SCALING_HPP

#include <opencv2/core/mat.hpp>
#include <string>

namespace image_processor
{

    enum class InterpolationMethod
    {
        NEAREST,
        LINEAR,
        CUBIC,
        AREA,
        LANCZOS
    };

    class ImageScaling
    {
    public:
        // 按指定尺寸缩放图像（使用字符串指定插值算法）
        static cv::Mat scaleImage(const cv::Mat &image, int new_width, int new_height,
                                  const std::string &method = "default");

        // 按指定比例缩放图像（使用字符串指定插值算法）
        static cv::Mat scaleImageByFactor(const cv::Mat &image, double scale_factor,
                                          const std::string &method = "default");

        // 图像放大（使用字符串指定插值算法）
        static cv::Mat enlargeImage(const cv::Mat &image, int new_width, int new_height,
                                    const std::string &method = "cubic");

        // 图像缩小（使用字符串指定插值算法）
        static cv::Mat shrinkImage(const cv::Mat &image, int new_width, int new_height,
                                   const std::string &method = "area");

    private:
        // 辅助函数：将字符串转换为插值方法枚举
        static InterpolationMethod stringToInterpolationMethod(const std::string &method_str);

        // 辅助函数：根据字符串或缩放动态选择插值算法
        static InterpolationMethod selectInterpolationMethod(double scale_factor, const std::string &method_str);

        // 辅助函数：将自定义的插值方法转换为OpenCV的插值方法
        static int convertInterpolationMethod(InterpolationMethod method);
    };

} // namespace image_processor

#endif // IMAGE_SCALING_HPP