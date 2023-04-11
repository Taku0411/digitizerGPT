#include <opencv2/opencv.hpp>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>

namespace fs = std::filesystem;

void rotate(fs::path &path, int index, fs::path destination)
{
  cv::Mat image = cv::imread(path);
  int width = image.cols;
  int height = image.rows;
  
  cv::Mat gray, edges;
  std::vector<cv::Vec4i> lines;

  cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);
  cv::Canny(gray, edges, 50, 150);
  cv::HoughLinesP(edges, lines, 1, 3.141592 / 180.0, 100, 100, 10);

  std::vector<double> angle;
  for(auto line: lines)
  {
    double tmp = std::atan2(line[3] - line[1], line[2] - line[0]) * 180 / 3.141592;
    if(std::fabs(tmp) < 10.0)
    {
      angle.push_back(tmp); 
      cv::line(image, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(0, 0, 255), 1);
    }
  }
  double rotate = std::accumulate(angle.begin(), angle.end(), 0.0) / angle.size();
  if(lines.size() == 0) rotate = 0.0;
  std::cout << rotate << std::endl;

  cv::Mat rotatedImg;
  cv::Mat rotationMat = cv::getRotationMatrix2D(cv::Point(image.cols/2, image.rows/2), rotate, 1.0);
  cv::warpAffine(image, rotatedImg, rotationMat, image.size());



  std::ostringstream ss;
  ss << std::setw(4) << std::setfill('0') << index;
  std::string name;
  name += ss.str();
  name += ".png";
  destination.append(name);
  std::cout << path << " -> " << destination << std::endl;
  cv::imwrite(destination, rotatedImg);
}

int main(int argc, char* argv[])
{
  int counter = 0;
  if(argc != 3)
  {
    std::cout << "Usage: ./rotate <dir png path> <destination>" << std::endl;
    std::exit(1);
  }
  fs::path dir = fs::path(argv[1]);
  fs::path destination = fs::path(argv[2]);
  for(const fs::directory_entry &x: fs::directory_iterator(dir))
  {
    fs::path path = x.path();
    if(path.extension() == ".png")
    {
      rotate(path, counter, destination);
      counter++;
    }
  }
  return 0;
}