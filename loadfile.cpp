#include <vector>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp> // Needed for image processing functions

using namespace std;
using namespace cv;

// Function to load an image from a file
Mat loadImage(const char* img_file) {
  Mat img = imread(img_file);
  if (img.empty()) {
    cerr << "Error: Unable to load image!" << endl;
    exit(1);
  }
  return img;
}

// Function to display an image with a given window title
void displayImage(const string& title, Mat img) {
  imshow(title, img);
  waitKey(0);
}

// Function to split an image into its color channels (B, G, R)
vector<Mat> splitColor(Mat img) {
  vector<Mat> channels(3);
  split(img, channels); // Splits into Blue, Green, and Red channels
  return channels;
}

Mat grayscale(Mat img) {
  Mat gray;
  cvtColor(img, gray, COLOR_BGR2GRAY);
  return gray;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
    return 1;
  }

  // Load the image
  Mat img = loadImage(argv[1]);

  // Display the original image
  displayImage("Original Image", img);

  // Split the image into its color channels
  vector<Mat> img_split = splitColor(img);

  // Display each channel
  displayImage("Blue Channel", img_split[0]);
  displayImage("Green Channel", img_split[1]);
  displayImage("Red Channel", img_split[2]);

  Mat img_gray = grayscale(img);
  displayImage("Grayscale", img_gray);


  return 0;
}
