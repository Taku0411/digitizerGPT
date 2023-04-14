#include <filesystem>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

void deleteShadow2(fs::path &path) {
	int id = std::stoi(path.stem().string().substr(0, 3));
  cv::Mat image = cv::imread(path);
  // info
  int width = image.cols;
  int height = image.rows;

  int margin = 140;

  // common
  cv::rectangle(image, cv::Point(0, 0), cv::Point(width, margin),
                cv::Scalar(255, 255, 255), -1);
  cv::rectangle(image, cv::Point(0, height - margin), cv::Point(width, height),
                cv::Scalar(255, 255, 255), -1);

  int l_margin = 100;
  int r_margin = 100;
  // left page case
  if (id % 2 == 0) {
    l_margin = 150;
    r_margin = 50;
  } else {
    l_margin = 320;
    r_margin = 50;
  }

  cv::rectangle(image, cv::Point(0, 0), cv::Point(l_margin, height),
                cv::Scalar(255, 255, 255), -1);
  cv::rectangle(image, cv::Point(width - r_margin, 0), cv::Point(width, height),
                cv::Scalar(255, 255, 255), -1);
  // right page case

  // export
  std::string name = path.stem();
  name += "_shadow2.png";
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
    deleteShadow2(item);
    counter++;
  }
  return 0;
}
