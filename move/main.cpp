#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/core/base.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

void move(fs::path &path)
{
  cv::Mat image = cv::imread(path);
  int id = std::stoi(path.stem().string().substr(0, 3));
  // info
  int width = image.cols;
  int height = image.rows;


  cv::Mat cvt, bit, thre, dilate;
  cv::cvtColor(image, cvt, cv::COLOR_RGB2GRAY);

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

  cv::bitwise_not(cvt, bit);
  cv::threshold(bit, thre, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Point(20, 20));
  cv::dilate(thre, dilate, kernel, cv::Point(-1, -1), 1);
  cv::imwrite("dilate.png", dilate);

  cv::Mat x_vec, y_vec;
  cv::reduce(dilate, x_vec, 0, cv::REDUCE_AVG);
  cv::reduce(dilate, y_vec, 1, cv::REDUCE_AVG);

  int left, right, top, bottom;
  int threshold = 5;
  for (int i = 30; i < width - 30; i++)
  {
    int val = x_vec.data[i];
    if (val > threshold)
    {
      left = i;
      break;
    }
  }
  for (int i = 30; i < width - 30; i++)
  {
    int val = x_vec.data[width - i];
    if (val > threshold)
    {
      right = width - i;
      break;
    }
  }
  for (int i = 30; i < height; i++)
  {
    int val = y_vec.data[i];
    if (val > threshold)
    {
      top = i;
      break;
    }
  }
  for (int i = 30; i < height; i++)
  {
    int val = y_vec.data[height - i];
    if (val > threshold)
    {
      bottom = height - i;
      break;
    }
  }

  int h_pad = 450;
  int v_pad = 441;
	std::vector<int> warining;
  std::cout << cv::Point(left, top) << " -> " << cv::Point(h_pad, 500)
            << std::endl;
  std::cout << cv::Point(left, bottom) << " -> "
            << cv::Point(h_pad, height - v_pad) << std::endl;
  std::cout << cv::Point(right, bottom) << " -> "
            << cv::Point(width - h_pad, height - v_pad) << std::endl;

  std::vector<cv::Point2f> detected = {cv::Point(left, top),
                                       cv::Point(left, bottom),
                                       cv::Point(right, bottom)};
  std::vector<cv::Point2f> target = {cv::Point(h_pad, v_pad),
                                     cv::Point(h_pad, height - v_pad),
                                     cv::Point(width - h_pad, height - v_pad)};
  cv::Mat trans = cv::getAffineTransform(detected, target);
	std::cout << trans << std::endl;
  cv::Mat result;
  cv::warpAffine(image, result, trans, image.size(), cv::INTER_LINEAR,
                 cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));


  // draw lines
  cv::line(image, cv::Point(left, 0), cv::Point(left, height),
           cv::Scalar(0, 0, 255), 3);
  cv::line(image, cv::Point(right, 0), cv::Point(right, height),
           cv::Scalar(0, 0, 255), 3);
  cv::line(image, cv::Point(0, top), cv::Point(width, top),
           cv::Scalar(0, 0, 255), 3);
  cv::line(image, cv::Point(0, bottom), cv::Point(width, bottom),
           cv::Scalar(0, 0, 255), 3);


  std::string name = path.stem().string().substr(0, 3);
  cv::imwrite(name + "_detected.png", image);
  cv::imwrite(name + ".png", result);
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

  for (auto &item: list) { move(item); }
  return 0;
}
