#include <filesystem>
#include <numeric>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct detected {
  detected(cv::Vec4i line) : line(line) {
    degree_raw =
        std::atan2(line[3] - line[1], line[2] - line[0]) * 180 / 3.141592;

    if (std::fabs(degree_raw) < 2.0 || std::fabs(degree_raw) < 2.0) {
      if (std::fabs(degree_raw) > 80) {
        degree_result -= 90;
        isValid = true;
      }
    }
    line_length = std::sqrt(std::pow(line[0] - line[1], 2) +
                            std::pow(line[2] - line[3], 2));
  }
  double degree_raw;    // input degree
  double degree_result; // modified degree
  double line_length;
  bool isValid = false; // valid or not

  cv::Vec4i line;
};

bool compare(detected &left, detected &right) {
  return left.line_length < right.line_length;
}

double getDegree(std::vector<detected> &lines) {
  double result = 100.0;
  // sort lines by length
  std::sort(lines.begin(), lines.end(), compare);
	
      return result;
};

void rotate(fs::path &path) {
  cv::Mat image = cv::imread(path);
  int width = image.cols;
  int height = image.rows;

  cv::Mat gray, edges;
  std::vector<cv::Vec4i> lines;

  cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);
  cv::Canny(gray, edges, 50, 150);
  cv::HoughLinesP(edges, lines, 1, 3.141592 / 180.0, 200, 20, 8);

  std::vector<detected> rot_data;
  for (auto &line : lines) {
    rot_data.push_back(line);
  }

  cv::Mat rotatedImg;
  cv::Mat rotationMat = cv::getRotationMatrix2D(
      cv::Point(image.cols / 2, image.rows / 2), getDegree(rot_data), 1.0);
  cv::warpAffine(image, rotatedImg, rotationMat, image.size());

  // cv::line(image, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]),
  //        cv::Scalar(0, 0, 255), 3);
  //  cv::putText(image, std::to_string(degree), cv::Point(line[0], line[1]),
  //              cv::FONT_HERSHEY_PLAIN, 4, cv::Scalar(0, 0, 255), 5,
  //              cv::LINE_AA);
  // export
  std::string name = path.stem();
  std::cout << name << std::endl;
  cv::imwrite(name + "_detect.png", image);
  cv::imwrite(name + "_rotated.png", rotatedImg);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: ./tilt <dir png path>" << std::endl;
    std::exit(1);
  }
  std::vector<fs::path> list;
  for (int i = 1; i < argc; i++) {
    fs::path tmp = fs::path(argv[i]);
    if (tmp.extension() != ".png") {
      std::cout << "this file not supported: " << tmp << std::endl;
      std::exit(1);
    }
    list.push_back(fs::path(argv[i]));
  }

  int counter = 0;
  for (auto &item : list) {
    rotate(item);
    counter += 2;
  }
  return 0;
}
