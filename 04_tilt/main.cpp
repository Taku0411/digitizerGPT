#include <opencv2/opencv.hpp>
#include <filesystem>
#include <string>
#include <sstream>


namespace fs = std::filesystem;

void crop(fs::path &path, int index, fs::path destination)
{
  cv::Mat image = cv::imread(path);
  int width = image.cols;
  int height = image.rows;

  double threshold = 0.1;
  double offset = 0.05;
  double offset_right, offset_left;
  if(index % 2 == 0)
  {
    offset_right = 0.015;
    offset_left = 0.08;
  }
  else
  {
    offset_right = 0.1;
    offset_left = 0.07;
  }
  int margin = 10;
  

  // 画像の左から右に走査する
  for (int x = width * offset_left; x < width; x++) {
    int white_pixels = 0;
    int black_pixels = 0;
    // 現在の列に含まれる黒と白のピクセルの数を数える
    for (int y = 0; y < height; y++) {
      cv::Vec3b color = image.at<cv::Vec3b>(cv::Point(x, y));
      if (color == cv::Vec3b(0, 0, 0)) { // 黒色の場合
        black_pixels++;
      } else { // 白色の場合
        white_pixels++;
      }
    }
    // 黒と白のピクセルの比率を計算し、閾値を超えたらそれより左側の領域を白く塗りつぶす
    double ratio = (double)black_pixels / height;
    if (ratio < threshold) {
      rectangle(image, cv::Point(0, 0), cv::Point(x - margin, height), cv::Scalar(255, 255, 255), cv::FILLED);
      // rectangle(image, cv::Point(0, 0), cv::Point(x - margin, height), cv::Scalar(0, 0, 255), 2);
      break;
    }
  }

  // 画像の上から下に走査する
  for (int y = height * offset; y < height; y++) {
    int white_pixels = 0;
    int black_pixels = 0;
    // 現在の行に含まれる黒と白のピクセルの数を数える
    for (int x = 0; x < width; x++) {
      cv::Vec3b color = image.at<cv::Vec3b>(cv::Point(x, y));
      if (color == cv::Vec3b(0, 0, 0)) { // 黒色の場合
        black_pixels++;
      } else { // 白色の場合
        white_pixels++;
      }
    }
    // 黒と白のピクセルの比率を計算し、閾値を超えたらそれより上の領域を白く塗りつぶす
    double ratio = (double)black_pixels / width;
    if (ratio < threshold) {
      cv::rectangle(image, cv::Point(0, 0), cv::Point(width, y - margin), cv::Scalar(255, 255, 255), cv::FILLED);
      // cv::rectangle(image, cv::Point(0, 0), cv::Point(width, y - margin), cv::Scalar(0, 0, 255), 2);
      break;
    }
  }

  // 画像の右から左に走査する
  for (int x = width * (1.0 - offset_right) - 1; x >= 0; x--) {
    int white_pixels = 0;
    int black_pixels = 0;
    // 現在の列に含まれる黒と白のピクセルの数を数える
    for (int y = 0; y < height; y++) {
      cv::Vec3b color = image.at<cv::Vec3b>(cv::Point(x, y));
      if (color == cv::Vec3b(0, 0, 0)) { // 黒色の場合
        black_pixels++;
      } else { // 白色の場合
        white_pixels++;
      }
    }
    // 黒と白のピクセルの比率を計算し、閾値を超えたらそれより右の領域を白く塗りつぶす
    double ratio = (double)black_pixels / height;
    if (ratio < threshold) {
      cv::rectangle(image, cv::Point(x + margin, 0), cv::Point(width, height), cv::Scalar(255, 255, 255), cv::FILLED);
      // cv::rectangle(image, cv::Point(x + margin, 0), cv::Point(width, height), cv::Scalar(0, 0, 255), 2);
      break;
    }
  }
  // 画像の下から上に走査する
  for (int y = height * (1.0 - offset); y >= 0; y--) {
    int white_pixels = 0;
    int black_pixels = 0;
    // 現在の行に含まれる黒と白のピクセルの数を数える
    for (int x = 0; x < width; x++) {
      cv::Vec3b color = image.at<cv::Vec3b>(cv::Point(x, y));
      if (color == cv::Vec3b(0, 0, 0)) { // 黒色の場合
        black_pixels++;
      } else { // 白色の場合
        white_pixels++;
      }
    }
    // 黒と白のピクセルの比率を計算し、閾値を超えたらそれより下の領域を白く塗りつぶす
    double ratio = (double)black_pixels / width;
    if (ratio < threshold) {
      cv::rectangle(image, cv::Point(0, y + margin), cv::Point(width, height), cv::Scalar(255, 255, 255), cv::FILLED);
      // cv::rectangle(image, cv::Point(0, y + margin), cv::Point(width, height), cv::Scalar(0, 0, 255), 2);
      break;
    }
  }

  std::ostringstream ss;
  ss << std::setw(4) << std::setfill('0') << index;
  std::string name;
  name += ss.str();
  name += ".png";
  destination.append(name);
  std::cout << path << " -> " << destination << std::endl;
  cv::imwrite(destination, image);
}

int main(int argc, char* argv[])
{
  int counter = 0;
  if(argc != 3)
  {
    std::cout << "Usage: ./tilt <dir png path> <destination>" << std::endl;
    std::exit(1);
  }
  fs::path dir = fs::path(argv[1]);
  fs::path destination = fs::path(argv[2]);
  for(const fs::directory_entry &x: fs::directory_iterator(dir))
  {
    fs::path path = x.path();
    if(path.extension() == ".png")
    {
      crop(path, counter, destination);
      counter++;
    }
  }
  return 0;
}