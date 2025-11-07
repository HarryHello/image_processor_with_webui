#include "compression.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "logger.hpp"
#include <opencv2/imgproc.hpp>

namespace image_processor
{

    std::vector<PixelTriplet> Compression::imageToTriplets(const cv::Mat &image)
    {
        std::vector<PixelTriplet> triplets;

        if (image.empty())
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Cannot convert empty image to triplets");
            return triplets;
        }

        // 使用原始图像，不强制转换为灰度图
        cv::Mat processed_image = image.clone();

        // 将图像转换为三元组（只存储非零像素以节省空间）
        for (int i = 0; i < processed_image.rows; ++i)
        {
            for (int j = 0; j < processed_image.cols; ++j)
            {
                // 检查是否所有通道都是零值
                bool isAllZero = true;
                std::vector<uchar> pixel_values(processed_image.channels());

                for (int c = 0; c < processed_image.channels(); ++c)
                {
                    if (processed_image.channels() == 1)
                    {
                        pixel_values[c] = processed_image.at<uchar>(i, j);
                    }
                    else
                    {
                        pixel_values[c] = processed_image.at<cv::Vec3b>(i, j)[c];
                    }

                    if (pixel_values[c] != 0)
                    {
                        isAllZero = false;
                    }
                }

                // 只存储非零像素
                if (!isAllZero)
                {
                    triplets.push_back({i, j, pixel_values});
                }
            }
        }

        Logger::log(LogLevel::IP_LOGLV_INFO, "Image converted to triplets successfully");
        return triplets;
    }

    cv::Mat Compression::tripletsToImage(const std::vector<PixelTriplet> &triplets, int rows, int cols, int channels)
    {
        if (rows <= 0 || cols <= 0)
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Invalid image dimensions");
            return cv::Mat();
        }

        // 创建空图像（初始化为黑色）
        cv::Mat image;
        if (channels == 1)
        {
            image = cv::Mat(rows, cols, CV_8UC1, cv::Scalar(0));
        }
        else
        {
            image = cv::Mat(rows, cols, CV_8UC3, cv::Scalar(0, 0, 0));
        }

        // 将三元组数据填充到图像中
        for (const auto &triplet : triplets)
        {
            if (triplet.row >= 0 && triplet.row < rows && triplet.col >= 0 && triplet.col < cols)
            {
                if (channels == 1)
                {
                    image.at<uchar>(triplet.row, triplet.col) = triplet.values[0];
                }
                else
                {
                    cv::Vec3b &pixel = image.at<cv::Vec3b>(triplet.row, triplet.col);
                    for (int c = 0; c < std::min(3, (int)triplet.values.size()); ++c)
                    {
                        pixel[c] = triplet.values[c];
                    }
                }
            }
        }

        Logger::log(LogLevel::IP_LOGLV_INFO, "Triplets converted to image successfully");
        return image;
    }

    bool Compression::compressTriplets(const std::vector<PixelTriplet> &triplets, const std::string &filepath)
    {
        std::ofstream out_file(filepath, std::ios::binary);
        if (!out_file.is_open())
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Failed to open file for writing: " + filepath);
            return false;
        }

        // 写入三元组数量
        size_t triplet_count = triplets.size();
        out_file.write(reinterpret_cast<const char *>(&triplet_count), sizeof(triplet_count));

        // 简单的RLE压缩：压缩连续的重复像素
        for (size_t i = 0; i < triplets.size();)
        {
            const PixelTriplet &current_triplet = triplets[i];
            size_t count = 1;

            // 计算连续重复的像素数量
            while (i + count < triplets.size() &&
                   triplets[i + count].row == current_triplet.row &&
                   triplets[i + count].col == current_triplet.col + static_cast<int>(count) &&
                   triplets[i + count].values == current_triplet.values)
            {
                count++;
            }

            // 只有重复像素才写入计数，单一像素不写入计数
            if (count > 1)
            {
                // 写入负数表示重复像素数量
                int negative_count = -static_cast<int>(count);
                out_file.write(reinterpret_cast<const char *>(&negative_count), sizeof(negative_count));
            }
            // 单一像素不写入计数标记

            // 写入像素数据（行列值和像素值）
            out_file.write(reinterpret_cast<const char *>(&current_triplet.row), sizeof(current_triplet.row));
            out_file.write(reinterpret_cast<const char *>(&current_triplet.col), sizeof(current_triplet.col));

            size_t values_size = current_triplet.values.size();
            out_file.write(reinterpret_cast<const char *>(&values_size), sizeof(values_size));
            out_file.write(reinterpret_cast<const char *>(current_triplet.values.data()), values_size * sizeof(uchar));

            // 移动到下一个不重复的像素
            i += count;
        }

        out_file.close();
        Logger::log(LogLevel::IP_LOGLV_INFO, "Triplets compressed and saved to: " + filepath);
        return true;
    }

    std::vector<PixelTriplet> Compression::decompressTriplets(const std::string &filepath)
    {
        std::vector<PixelTriplet> triplets;

        std::ifstream in_file(filepath, std::ios::binary);
        if (!in_file.is_open())
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Failed to open file for reading: " + filepath);
            return triplets;
        }

        // 读取三元组数量
        size_t triplet_count = 0;
        in_file.read(reinterpret_cast<char *>(&triplet_count), sizeof(triplet_count));

        // 预留空间但要注意实际解压后的数量可能更多
        triplets.reserve(triplet_count);

        // 解压数据
        while (in_file.peek() != EOF && triplets.size() < triplet_count * 2) // 设置安全上限
        {
            // 尝试读取计数标记
            int count_flag = 1; // 默认为1个像素
            std::streampos pos = in_file.tellg();

            // 读取可能的计数标记
            if (in_file.read(reinterpret_cast<char *>(&count_flag), sizeof(count_flag)))
            {
                // 检查是否为负数（重复像素标记）
                if (count_flag >= 0)
                {
                    // 如果不是负数，回退文件指针，因为我们误读了像素数据
                    in_file.seekg(pos);
                    count_flag = 1; // 设置为默认的1个像素
                }
                // 如果是负数，count_flag已经正确设置为重复像素数量
            }
            else
            {
                // 文件结束或读取失败，跳出循环
                break;
            }

            // 读取像素数据
            PixelTriplet triplet;
            in_file.read(reinterpret_cast<char *>(&triplet.row), sizeof(triplet.row));
            in_file.read(reinterpret_cast<char *>(&triplet.col), sizeof(triplet.col));

            size_t values_size = 0;
            in_file.read(reinterpret_cast<char *>(&values_size), sizeof(values_size));
            triplet.values.resize(values_size);
            in_file.read(reinterpret_cast<char *>(triplet.values.data()), values_size * sizeof(uchar));

            if (count_flag < 0)
            {
                // 负数表示重复像素
                size_t repeat_count = static_cast<size_t>(-count_flag);
                for (size_t i = 0; i < repeat_count; ++i)
                {
                    PixelTriplet repeated_triplet = triplet;
                    repeated_triplet.col = triplet.col + static_cast<int>(i);
                    triplets.push_back(repeated_triplet);
                }
            }
            else
            {
                // 正数表示单一像素
                triplets.push_back(triplet);
            }
        }

        in_file.close();
        Logger::log(LogLevel::IP_LOGLV_INFO, "Triplets decompressed from: " + filepath);
        return triplets;
    }

} // namespace image_processor