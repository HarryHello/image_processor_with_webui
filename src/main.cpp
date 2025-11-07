// #include "image_io.hpp"
#include "logger.hpp"
#include "image_io.hpp"
#include "compression.hpp"
#include "color_processing.hpp"
#include "image_scaling.hpp"
#include "web_server.hpp"
#include <iostream>
#include <opencv2/core.hpp>

using namespace image_processor;

int main()
{
    try
    {
        // // Set up logger: output to both console and file
        // // Logger::setLogFile("image_processor.log");
        // Logger::setConsoleOutput(true);

        // std::string input_path;
        // std::cout << "Enter image file path to load (default: ../assets/color-block.ppm): ";
        // std::getline(std::cin, input_path);

        // // 如果用户没有输入路径，则使用默认的彩色图像路径
        // if (input_path.empty())
        // {
        //     input_path = "../assets/color-block.ppm";
        // }

        // // Read image
        // cv::Mat image = ImageIO::readImage(input_path);
        // if (image.empty())
        // {
        //     Logger::log(LogLevel::IP_LOGLV_ERROR, "Failed to load image: " + input_path);
        //     std::cout << "Failed to load image. Exiting." << std::endl;
        //     std::cout << "\nPress Enter to exit..." << std::endl;
        //     std::cin.get();
        //     return 1;
        // }

        // // Display image
        // ImageIO::displayImage("Loaded Image", image);
        // ImageIO::waitAndClose();

        // // Save image as a copy
        // std::string output_path = "output_copy.png";
        // if (ImageIO::saveImage(output_path, image))
        // {
        //     std::cout << "Image saved as: " << output_path << std::endl;
        // }
        // else
        // {
        //     std::cout << "Failed to save image." << std::endl;
        // }

        // // 测试压缩功能
        // std::cout << "\nTesting compression functionality..." << std::endl;

        // // 将图像转换为三元组
        // std::vector<PixelTriplet> triplets = Compression::imageToTriplets(image);
        // std::cout << "Converted image to " << triplets.size() << " triplets." << std::endl;

        // // 压缩三元组并保存到文件
        // std::string compressed_path = "compressed_data.bin";
        // if (Compression::compressTriplets(triplets, compressed_path))
        // {
        //     std::cout << "Triplets compressed and saved to: " << compressed_path << std::endl;

        //     // 从压缩文件中加载三元组
        //     std::vector<PixelTriplet> decompressed_triplets = Compression::decompressTriplets(compressed_path);
        //     std::cout << "Decompressed " << decompressed_triplets.size() << " triplets from file." << std::endl;

        //     // 将三元组转换回图像
        //     cv::Mat reconstructed_image = Compression::tripletsToImage(decompressed_triplets, image.rows, image.cols, image.channels());
        //     if (!reconstructed_image.empty())
        //     {
        //         std::cout << "Reconstructed image from triplets." << std::endl;

        //         // 保存重建的图像
        //         std::string reconstructed_path = "reconstructed_image.png";
        //         if (ImageIO::saveImage(reconstructed_path, reconstructed_image))
        //         {
        //             std::cout << "Reconstructed image saved as: " << reconstructed_path << std::endl;
        //         }
        //         else
        //         {
        //             std::cout << "Failed to save reconstructed image." << std::endl;
        //         }

        //         // 显示重建的图像
        //         ImageIO::displayImage("Reconstructed Image", reconstructed_image);
        //         ImageIO::waitAndClose();
        //     }
        //     else
        //     {
        //         std::cout << "Failed to reconstruct image from triplets." << std::endl;
        //     }
        // }
        // else
        // {
        //     std::cout << "Failed to compress triplets." << std::endl;
        // }

        // // 测试图像缩放功能
        // std::cout << "\nTesting image scaling functionality..." << std::endl;

        // // 测试按比例缩放（使用默认动态选择的插值算法）
        // cv::Mat scaled_image = image_processor::ImageScaling::scaleImageByFactor(image, 0.5);
        // if (!scaled_image.empty())
        // {
        //     std::cout << "Image scaled to 50% using default interpolation method." << std::endl;
        //     ImageIO::displayImage("Scaled Image (50%)", scaled_image);
        //     ImageIO::waitAndClose();
        // }
        // else
        // {
        //     std::cout << "Failed to scale image to 50%." << std::endl;
        // }

        // // 测试按指定尺寸缩放（使用字符串指定插值算法）
        // cv::Mat resized_image = image_processor::ImageScaling::scaleImage(image, 200, 200, "cubic");
        // if (!resized_image.empty())
        // {
        //     std::cout << "Image resized to 200x200 using cubic interpolation." << std::endl;
        //     ImageIO::displayImage("Resized Image (200x200)", resized_image);
        //     ImageIO::waitAndClose();
        // }
        // else
        // {
        //     std::cout << "Failed to resize image to 200x200." << std::endl;
        // }

        // // 测试放大图像（使用字符串指定插值算法）
        // cv::Mat enlarged_image = image_processor::ImageScaling::enlargeImage(image, image.cols * 2, image.rows * 2, "area");
        // if (!enlarged_image.empty())
        // {
        //     std::cout << "Image enlarged to 2x using area interpolation." << std::endl;
        //     ImageIO::displayImage("Enlarged Image (2x)", enlarged_image);
        //     ImageIO::waitAndClose();
        // }
        // else
        // {
        //     std::cout << "Failed to enlarge image to 2x." << std::endl;
        // }

        // // 测试缩小图像（使用字符串指定插值算法）
        // cv::Mat shrunk_image = image_processor::ImageScaling::shrinkImage(image, image.cols / 2, image.rows / 2, "linear");
        // if (!shrunk_image.empty())
        // {
        //     std::cout << "Image shrunk to 50% using linear interpolation." << std::endl;
        //     ImageIO::displayImage("Shrunk Image (50%)", shrunk_image);
        //     ImageIO::waitAndClose();
        // }
        // else
        // {
        //     std::cout << "Failed to shrink image to 50%." << std::endl;
        // }

        // // 测试彩色图像转灰度图像功能
        // std::cout << "\nTesting color to grayscale conversion..." << std::endl;

        // // 使用OpenCV默认方法转换为灰度图像
        // cv::Mat grayscale_image = ColorProcessing::convertToGrayscale(image);
        // if (!grayscale_image.empty())
        // {
        //     std::cout << "Image converted to grayscale using default method." << std::endl;

        //     // 保存灰度图像
        //     std::string grayscale_path = "grayscale_image.png";
        //     if (ImageIO::saveImage(grayscale_path, grayscale_image))
        //     {
        //         std::cout << "Grayscale image saved as: " << grayscale_path << std::endl;
        //     }
        //     else
        //     {
        //         std::cout << "Failed to save grayscale image." << std::endl;
        //     }

        //     // 显示灰度图像
        //     ImageIO::displayImage("Grayscale Image", grayscale_image);
        //     ImageIO::waitAndClose();

        //     // 使用自定义权重转换为灰度图像 (人眼感知权重)
        //     cv::Mat custom_grayscale_image = ColorProcessing::convertToGrayscale(image, 0, 1, 0);
        //     if (!custom_grayscale_image.empty())
        //     {
        //         std::cout << "Image converted to grayscale using custom weights (human perception)." << std::endl;

        //         // 保存自定义权重灰度图像
        //         std::string custom_grayscale_path = "custom_grayscale_image.png";
        //         if (ImageIO::saveImage(custom_grayscale_path, custom_grayscale_image))
        //         {
        //             std::cout << "Custom grayscale image saved as: " << custom_grayscale_path << std::endl;
        //         }
        //         else
        //         {
        //             std::cout << "Failed to save custom grayscale image." << std::endl;
        //         }

        //         // 显示自定义权重灰度图像
        //         ImageIO::displayImage("Custom Grayscale Image", custom_grayscale_image);
        //         ImageIO::waitAndClose();
        //     }
        //     else
        //     {
        //         std::cout << "Failed to convert image to grayscale using custom weights." << std::endl;
        //     }
        // }
        // else
        // {
        //     std::cout << "Failed to convert image to grayscale." << std::endl;
        // }

        // 询问用户是否启动Web服务器
        std::cout << "\nDo you want to start the web server? (y/n): ";
        std::string start_server;
        std::getline(std::cin, start_server);

        if (start_server == "y" || start_server == "Y")
        {
            try
            {
                WebServer server(8000);
                std::cout << "Starting web server on port 8000..." << std::endl;
                std::cout << "Access the web interface at http://localhost:8000" << std::endl;
                server.start();
            }
            catch (const std::exception &e)
            {
                Logger::log(LogLevel::IP_LOGLV_ERROR, "Web server error: " + std::string(e.what()));
                std::cout << "Web server error: " << e.what() << std::endl;
            }
        }
        else
        {
            // 等待用户按回车后再退出，防止终端立即关闭
            std::cout << "\nPress Enter to exit..." << std::endl;
            std::cin.get();
        }

        return 0;
    }
    catch (const std::exception &e)
    {
        Logger::log(LogLevel::IP_LOGLV_ERROR, std::string("Exception occurred: ") + e.what());
        std::cout << "Error: " << e.what() << std::endl;
        std::cout << "\nPress Enter to exit..." << std::endl;
        std::cin.get();
        return 1;
    }

    return 0;
}