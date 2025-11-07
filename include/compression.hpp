#ifndef COMPRESSION_HPP
#define COMPRESSION_HPP

#include <opencv2/core/mat.hpp>
#include <vector>
#include <string>

namespace image_processor
{

    // 定义三元组结构（行、列、像素值）
    struct PixelTriplet
    {
        int row;
        int col;
        std::vector<uchar> values; // 支持多通道像素值
    };

    class Compression
    {
    public:
        // 将图像转换为三元组结构
        static std::vector<PixelTriplet> imageToTriplets(const cv::Mat &image);

        // 将三元组结构转换回图像
        static cv::Mat tripletsToImage(const std::vector<PixelTriplet> &triplets, int rows, int cols, int channels = 1);

        // 压缩三元组数据并保存到文件
        static bool compressTriplets(const std::vector<PixelTriplet> &triplets, const std::string &filepath);

        // 从压缩文件中加载三元组数据
        static std::vector<PixelTriplet> decompressTriplets(const std::string &filepath);
    };

} // namespace image_processor

#endif // COMPRESSION_HPP