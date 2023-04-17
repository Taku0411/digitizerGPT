#include <filesystem>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/core/base.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

void rotate(fs::path &path)
{
  cv::Mat image = cv::imread(path);
  int width = image.cols;
  int height = image.rows;

  // delete margin black region
  int margin = 30;
  cv::rectangle(image, cv::Point(0, 0), cv::Point(width, margin),
                cv::Scalar(255, 255, 255), cv::FILLED);
  cv::rectangle(image, cv::Point(0, 0), cv::Point(margin, height),
                cv::Scalar(255, 255, 255), cv::FILLED);
  cv::rectangle(image, cv::Point(width - margin, 0), cv::Point(width, height),
                cv::Scalar(255, 255, 255), cv::FILLED);
  cv::rectangle(image, cv::Point(0, height - margin), cv::Point(width, height),
                cv::Scalar(255, 255, 255), cv::FILLED);

  // define mat
  cv::Mat edge, gray, bit, thresh, dilate;

  cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);

  cv::bitwise_not(gray, bit);

  cv::threshold(bit, thresh, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Point(20, 20));
  cv::dilate(thresh, dilate, kernel, cv::Point(-1, -1), 1);

  std::vector<cv::Vec4i> lines;
  cv::HoughLinesP(dilate, lines, 1, 3.1415926535 / 180, 10, (double) width / 5,
                  10);

  std::vector<double> rot;
  for (auto &line: lines)
  {
    double line_rot =
            std::atan2(line[3] - line[1], line[2] - line[0]) * 180 / 3.141592;
    if (std::fabs(line_rot) > 1.0e-3 || std::fabs(line_rot - 90) > 1.0e-3)
    {
      std::cout << line_rot << std::endl;
      if (std::fabs(line_rot) > 45)
      {
        line_rot -= 90;
      }
      rot.push_back(line_rot);
      cv::line(image, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]),
               cv::Scalar(0, 0, 255), 3);
    }
  }

  std::vector<double> rot_res;
  double rot_mean =
          std::accumulate(rot.begin(), rot.end(), 0) / (double) rot.size();
  double rot_var =
          std::inner_product(rot.begin(), rot.end(), rot.begin(), 0.0) /
                  (double) rot.size() -
          rot_mean * rot_mean;
  std::cout << "--- stat ---" << std::endl;
  std::cout << "mean: " << rot_mean << std::endl;
  std::cout << "var : " << rot_var << std::endl;
  for (double rot_i: rot)
  {
    double std_var = std::fabs(rot_i - rot_mean) / std::sqrt(rot_var);
    //std::cout << std::fabs(rot_i - rot_mean) / std::sqrt(rot_var) << std::endl;
    if (std::fabs(rot_i - rot_mean) / std::sqrt(rot_var) < 1.0)
      rot_res.push_back(rot_i);
  }
  double rot_res_mean = std::accumulate(rot_res.begin(), rot_res.end(), 0) /
                        (double) rot_res.size() * 2.0;
  std::cout << "skew degree: " << rot_res_mean << std::endl;


  cv::Mat M = cv::getRotationMatrix2D(cv::Point(width / 2, height / 2),
                                      rot_mean, 1.0);
  cv::Mat image_result;
  cv::warpAffine(image, image_result, M, image.size(), cv::INTER_LINEAR,
                 cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));

  std::string name = path.stem().string() + ".png";
  std::string name2 = path.stem().string() + "_detected.png";
  std::cout << name << std::endl;
  cv::imwrite(name, image_result);
  cv::imwrite(name2, image);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: ./tilt <dir png path>" << std::endl;
    std::exit(1);
  }
  std::vector<fs::path> list;
  for (int i = 1; i < argc; i++)
  {
    fs::path tmp = fs::path(argv[i]);
    if (tmp.extension() != ".png")
    {
      std::cout << "this file not supported: " << tmp << std::endl;
      std::exit(1);
    }
    list.push_back(fs::path(argv[i]));
  }

  for (auto &item: list) { rotate(item); }
  return 0;
}
