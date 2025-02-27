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

Mat histogram(Mat img) {
  Mat hist;
  int histSize = 256; 
  float range[] = { 0, 256 };
  const float* histRange[] = { range };
  calcHist(&img, 1, 0, Mat(), hist, 1, &histSize, histRange, true, false);
  int hist_w = 1024, hist_h = 800;
  int bin_w = cvRound( (double) hist_w/histSize );
  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
  for( int i = 1; i < histSize; i++ )
  {
    line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ),
      Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
      Scalar( 255, 255, 255), 1, 8, 0  );
  }
  return histImage;
}

Mat imageBlur(Mat img, Size kSize){
  Mat img_blur;
  blur(img, img_blur, kSize);
  return img_blur;
}

// Function to compute absolute difference between two images
Mat computeDifference(Mat img1, Mat img2) {
  Mat img2_resized;
  resize(img2, img2_resized, img1.size()); // Resize img2 to match img1
  Mat diff;
  absdiff(img1, img2_resized, diff);
  return diff;
}

// Function to compute Mean Squared Error (MSE)
double computeMSE(Mat img1, Mat img2) {
  Mat img2_resized;
  resize(img2, img2_resized, img1.size()); // Resize img2 to match img1
  Mat diff;
  absdiff(img1, img2_resized, diff);
  diff.convertTo(diff, CV_32F);
  diff = diff.mul(diff);
  Scalar s = sum(diff);
  double mse = (s[0] + s[1] + s[2]) / (img1.channels() * img1.total());
  return mse;
}

// Compute Peak Signal-to-Noise Ratio (PSNR)
double computePSNR(Mat img1, Mat img2) {
  double mse = computeMSE(img1, img2);
  if (mse == 0) return INFINITY;
  double psnr = 10.0 * log10((255 * 255) / mse);
  return psnr;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <image_path>" << endl;
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
  displayImage("Original Image", img);
  displayImage("Grayscale", img_gray);

  Mat hist_img = histogram(img_gray);
  displayImage("Histogram",hist_img);

  Mat blur_img = imageBlur(img, Size(10,10));
  displayImage("Original Image", img);
  displayImage("Blurred Image",blur_img);

  if (argc == 3) {
    Mat img2 = loadImage(argv[2]);
    Mat diff_img = computeDifference(img_gray, grayscale(img2));
    displayImage("Difference Image", diff_img);
    double mse = computeMSE(img_gray, grayscale(img2));
    double psnr = computePSNR(img_gray, grayscale(img2));

    cout << "MSE: " << mse << endl;
    cout << "PSNR: " << psnr << " dB" << endl;
  }

  return 0;
}
