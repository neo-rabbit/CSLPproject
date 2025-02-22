#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

Mat loadImage(char* img_file){
  Mat img = imread(img_file);
  return img;
}

void displayImage(Mat img){
  imshow("__missing_title__",img);
  waitKey(0);
}

std::vector<Mat> splitColor(Mat img){
  Mat red = img;
  return {red,img,img};
}

int main(int argc, char** argv){
  Mat img = loadImage(argv[1]);
  std::vector<Mat> img_split = splitColor(img);
  for (auto i: img_split){
    displayImage(i);
  }
}