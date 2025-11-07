#ifndef IMAGE_IO_HPP
#define IMAGE_IO_HPP

#include <string>
#include <opencv2/core/mat.hpp>

namespace image_processor {

class ImageIO {
public:
    // 读取图像文件
    static cv::Mat readImage(const std::string& filepath);
    
    // 保存图像文件
    static bool saveImage(const std::string& filepath, const cv::Mat& image);
    
    // 显示图像
    static void displayImage(const std::string& window_name, const cv::Mat& image);
    
    // 等待按键并关闭所有窗口
    static void waitAndClose();

private:
    // 检查文件是否存在
    static bool fileExists(const std::string& filepath);
    
    // 检查图像是否有效
    static bool isImageValid(const cv::Mat& image);
};

} // namespace image_processor

#endif // IMAGE_IO_HPP