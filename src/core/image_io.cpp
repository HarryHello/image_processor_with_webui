#include "image_io.hpp"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "logger.hpp"

namespace image_processor
{

    cv::Mat ImageIO::readImage(const std::string &filepath)
    {
        // Remove quotes if they exist at both ends of the path
        std::string clean_path = filepath;
        if (clean_path.size() >= 2 && clean_path.front() == '"' && clean_path.back() == '"')
        {
            clean_path = clean_path.substr(1, clean_path.size() - 2);
        }

        if (!fileExists(clean_path))
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "File does not exist: " + filepath);
            return cv::Mat();
        }

        cv::Mat image = cv::imread(clean_path);
        if (!isImageValid(image))
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Failed to read image: " + filepath);
            return cv::Mat();
        }

        Logger::log(LogLevel::IP_LOGLV_INFO,
                    "Image loaded: " + filepath +
                        " Size: " + std::to_string(image.cols) + "x" +
                        std::to_string(image.rows));

        return image;
    }

    bool ImageIO::saveImage(const std::string &filepath, const cv::Mat &image)
    {
        if (!isImageValid(image))
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Invalid image data");
            return false;
        }

        try
        {
            bool success = cv::imwrite(filepath, image);
            if (success)
            {
                Logger::log(LogLevel::IP_LOGLV_INFO, "Image saved to: " + filepath);
                return true;
            }
            else
            {
                Logger::log(LogLevel::IP_LOGLV_ERROR, "Failed to save image");
                return false;
            }
        }
        catch (const cv::Exception &e)
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "OpenCV error: " + std::string(e.what()));
            return false;
        }
    }

    void ImageIO::displayImage(const std::string &window_name, const cv::Mat &image)
    {
        if (!isImageValid(image))
        {
            Logger::log(LogLevel::IP_LOGLV_ERROR, "Cannot display invalid image");
            return;
        }

        cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
        cv::imshow(window_name, image);
        Logger::log(LogLevel::IP_LOGLV_INFO, "Displaying image in window: " + window_name);
    }

    void ImageIO::waitAndClose()
    {
        cv::waitKey(0);
        cv::destroyAllWindows();
    }

    bool ImageIO::fileExists(const std::string &filepath)
    {
        FILE *file = fopen(filepath.c_str(), "r");
        if (file)
        {
            fclose(file);
            return true;
        }
        return false;
    }

    bool ImageIO::isImageValid(const cv::Mat &image)
    {
        return !image.empty() && image.data != nullptr;
    }

} // namespace image_processor