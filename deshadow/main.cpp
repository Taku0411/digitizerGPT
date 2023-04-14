#include <filesystem>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

double getGray(cv::Mat &image, double x) {
  int height = image.rows;
  int sum = 0;
  for (int y = 0; y < height; y++) {
    sum += image.at<unsigned char>(y, x);
  }
  return (double)sum / height / 256;
}

void deleteShadow(fs::path &path) {
  cv::Mat image = cv::imread(path);
  // info
  int width = image.cols;
  int height = image.rows;
  // params
  int top = 300;
  int left = 400;
  int right = 400;
  int bottom = 400;
  int middle = 200;
  double gray_threshold = 0.9; //白さの値，これ白くなったら止める

  // delete top
  cv::rectangle(image, cv::Point(0, 0), cv::Point(width, top),
                cv::Scalar(255, 255, 255), -1);
  // delete right
  cv::rectangle(image, cv::Point(width - right, 0), cv::Point(width, height),
                cv::Scalar(255, 255, 255), -1);
  // delete left
  cv::rectangle(image, cv::Point(0, 0), cv::Point(left, height),
                cv::Scalar(255, 255, 255), -1);
  // delete bottom
  cv::rectangle(image, cv::Point(0, height - bottom), cv::Point(width, height),
                cv::Scalar(255, 255, 255), -1);
  // delete center
  int x_c = width / 2;
  cv::rectangle(image, cv::Point(x_c, 0), cv::Point(x_c, height),
                cv::Scalar(255, 255, 255), -1);


  // export
  std::string name = path.stem();
  name += "_shadow.png";
  std::cout << name << std::endl;
  cv::imwrite(name, image);
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
    deleteShadow(item);
    counter++;
  }
  return 0;
}
