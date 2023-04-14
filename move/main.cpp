#include <filesystem>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

void split(fs::path &path) {
  cv::Mat image = cv::imread(path);
	int id = std::stoi(path.stem().string().substr(0, 3));
  // info
  int width = image.cols;
  int height = image.rows;
	
	cv::Mat gray, edge, dilate, result;

	cv::Gray




	std::string name = path.stem() + "_final.png";
	std::string name2 = path.stem() + "_detected.png";
  cv::imwrite(name, result);
  cv::imwrite(name22, image);
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

  for (auto &item : list) {
    split(item);
  }
  return 0;
}
