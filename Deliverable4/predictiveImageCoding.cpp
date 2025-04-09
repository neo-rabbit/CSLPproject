#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>

#include "../Deliverable2/Bitstream.h"
#include "Golomb.h"

using namespace std;

int predict(int A, int B, int C) {
    if (C >= std::max(A, B)) return std::min(A, B);
    if (C <= std::min(A, B)) return std::max(A, B);
    return A + B - C;
}

void encodeImageLossless(const string& imagePath, const string& outputPath, int m, bool useInterleaving) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Failed to read image!" << endl;
        return;
    }

    int rows = image.rows;
    int cols = image.cols;

    BitStream bitStream(outputPath, true);
    GolombCoding golomb(useInterleaving);

    bitStream.writeBits(rows, 16);
    bitStream.writeBits(cols, 16);
    bitStream.writeBits(m, 8);

    for (int c = 0; c < 3; ++c) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int A = (j > 0) ? image.at<cv::Vec3b>(i, j - 1)[c] : 0;
                int B = (i > 0) ? image.at<cv::Vec3b>(i - 1, j)[c] : 0;
                int C = (i > 0 && j > 0) ? image.at<cv::Vec3b>(i - 1, j - 1)[c] : 0;

                int actual = image.at<cv::Vec3b>(i, j)[c];
                int predicted = predict(A, B, C);
                int residual = actual - predicted;

                golomb.encode(bitStream, residual, m);
            }
        }
    }
}

void decodeImageLossless(const string& inputPath, const string& outputImagePath, bool useInterleaving) {
    BitStream bitStream(inputPath, false);
    GolombCoding golomb(useInterleaving);

    int rows = bitStream.readBits(16);
    int cols = bitStream.readBits(16);
    int m = bitStream.readBits(8);

    cv::Mat decoded(rows, cols, CV_8UC3);

    for (int c = 0; c < 3; ++c) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int A = (j > 0) ? decoded.at<cv::Vec3b>(i, j - 1)[c] : 0;
                int B = (i > 0) ? decoded.at<cv::Vec3b>(i - 1, j)[c] : 0;
                int C = (i > 0 && j > 0) ? decoded.at<cv::Vec3b>(i - 1, j - 1)[c] : 0;

                int predicted = predict(A, B, C);
                int residual = golomb.decode(bitStream, m);
                int pixel = predicted + residual;

                decoded.at<cv::Vec3b>(i, j)[c] = cv::saturate_cast<uchar>(pixel);
            }
        }
    }

    cv::imwrite(outputImagePath, decoded);
}

int main() {
    string originalImage = "./landscape.png";
    string encodedFile = "encoded_lossless.bin";
    string decodedImage = "./decoded_landscape.png";
    int m = 8;
    bool useInterleaving = true;

    encodeImageLossless(originalImage, encodedFile, m, useInterleaving);
    decodeImageLossless(encodedFile, decodedImage, useInterleaving);

    cv::Mat orig = cv::imread(originalImage);
    cv::Mat recon = cv::imread(decodedImage);

    if (orig.empty() || recon.empty()) {
        cerr << "Failed to load images for comparison!" << endl;
        return 1;
    }

    // Compare properly using absdiff and grayscale
    cv::Mat diff;
    cv::absdiff(orig, recon, diff);
    if (diff.channels() > 1) {
        cv::cvtColor(diff, diff, cv::COLOR_BGR2GRAY);
    }

    int nonZeroDiffs = cv::countNonZero(diff);
    if (nonZeroDiffs == 0)
        cout << "Success: Images are identical" << endl;
    else
        cout << "Mismatch: " << nonZeroDiffs << " differing pixels" << endl;

    return 0;
}
