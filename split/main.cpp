#include <filesystem>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

void split(fs::path &path)
{
  cv::Mat image = cv::imread(path);
  std::cout << path << " -> ";
  int id = std::stoi(path.stem());
  // info
  int width = image.cols;
  int height = image.rows;

  cv::Mat left = image(cv::Rect(0, 0, width / 2, height));
  cv::Mat right = image(cv::Rect(width / 2, 0, width / 2, height));

  std::stringstream ss, ss2;
  ss << std::setw(3) << std::setfill('0') << id * 2;
  ss2 << std::setw(3) << std::setfill('0') << id * 2 + 1;


  std::string name1 = ss.str(), name2 = ss2.str();

  name1 += ".png";
  name2 += ".png";

  cv::imwrite(name1, left);
  cv::imwrite(name2, right);
  std::cout << name1 << " + " << name2 << std::endl;
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

  for (auto &item: list) { split(item); }
  return 0;
}
