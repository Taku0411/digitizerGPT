#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

namespace fs = std::filesystem;


void bind(int i)
{
  std::stringstream ss, ss2;
  ss << std::setw(3) << std::setfill('0') << i;
  ss2 << std::setw(3) << std::setfill('0') << i + 1;
  fs::path left = std::string("../08_laste/" + ss.str() + ".png");
  fs::path right = std::string("../08_laste/" + ss2.str() + ".png");
  assert(fs::exists(left));
  assert(fs::exists(right));
  cv::Mat left_img = cv::imread(left);
  std::cout << left << std::endl;
  cv::Mat right_img = cv::imread(right);
  std::cout << right << std::endl;


  // bind
  cv::Mat bind;
  cv::hconcat(left_img, right_img, bind);

  // fill white around the edges
  int width = bind.cols;
  int height = bind.rows;
  int middle = width / 2;
  int margin = 300;
  cv::rectangle(bind, cv::Point(0, 0), cv::Point(width, margin),
                cv::Scalar(255, 255, 255), cv::FILLED);
  cv::rectangle(bind, cv::Point(0, 0), cv::Point(margin, height),
                cv::Scalar(255, 255, 255), cv::FILLED);
  cv::rectangle(bind, cv::Point(width - margin, 0), cv::Point(width, height),
                cv::Scalar(255, 255, 255), cv::FILLED);
  cv::rectangle(bind, cv::Point(0, height - margin), cv::Point(width, height),
                cv::Scalar(255, 255, 255), cv::FILLED);
  cv::rectangle(bind, cv::Point(middle - margin, 0),
                cv::Point(middle + margin, height), cv::Scalar(255, 255, 255),
                cv::FILLED);
  cv::Mat gray;
  cv::cvtColor(bind, gray, cv::COLOR_BGR2GRAY);

  int thresh = 200;
  cv::Mat result;
  cv::threshold(gray, result, thresh, 255, cv::THRESH_TOZERO);


  std::stringstream ss3;
  ss3 << std::setw(3) << std::setfill('0') << i / 2;
  auto name = ss3.str() + ".png";
  std::cout << name << " -> ";
  std::fflush(stdout);
  cv::imwrite(name, result);
  std::cout << "OK" << std::endl;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    std::cout << "Usage: ./tilt start end" << std::endl;
    std::exit(1);
  }

  int start = std::stoi(argv[1]);
  int end = std::stoi(argv[2]);
  if (start % 2 != 0) start--;
  if (end % 2 != 0) end++;
  for (int i = start; i < end; i = i + 2) { bind(i); }


  return 0;
}
