#include <iostream>
#include <opencv2/opencv.hpp>
#include "../Deliverable2/Bitstream.h"
#include "./Golomb.h"

using namespace std;

int predict(int A, int B, int C) {
    if (C >= std::max(A, B)) return std::min(A, B);
    if (C <= std::min(A, B)) return std::max(A, B);
    return A + B - C;
}

void encodeVideoLossless(const string& videoPath, const string& outputPath, int m, bool useInterleaving) {
    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        cerr << "Error opening video!" << endl;
        return;
    }

    int frameCount = (int)cap.get(cv::CAP_PROP_FRAME_COUNT);
    int rows = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int cols = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);

    BitStream bitStream(outputPath, true);
    GolombCoding golomb(useInterleaving);

    // Write header
    bitStream.writeBits(frameCount, 16);
    bitStream.writeBits(rows, 16);
    bitStream.writeBits(cols, 16);
    bitStream.writeBits(m, 8);

    cv::Mat frame;
    for (int f = 0; f < frameCount; ++f) {
        cap >> frame;
        if (frame.empty()) break;

        for (int c = 0; c < 3; ++c) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    int A = (j > 0) ? frame.at<cv::Vec3b>(i, j - 1)[c] : 0;
                    int B = (i > 0) ? frame.at<cv::Vec3b>(i - 1, j)[c] : 0;
                    int C = (i > 0 && j > 0) ? frame.at<cv::Vec3b>(i - 1, j - 1)[c] : 0;

                    int actual = frame.at<cv::Vec3b>(i, j)[c];
                    int predicted = predict(A, B, C);
                    int residual = actual - predicted;

                    golomb.encode(bitStream, residual, m);
                }
            }
        }
    }

    cap.release();
}

void decodeVideoLossless(const string& inputPath, const string& outputVideoPath, bool useInterleaving) {
    BitStream bitStream(inputPath, false);
    GolombCoding golomb(useInterleaving);

    int frameCount = bitStream.readBits(16);
    int rows = bitStream.readBits(16);
    int cols = bitStream.readBits(16);
    int m = bitStream.readBits(8);

    cv::VideoWriter writer(
        outputVideoPath,
        cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
        30, // FPS
        cv::Size(cols, rows)
    );

    for (int f = 0; f < frameCount; ++f) {
        cv::Mat frame(rows, cols, CV_8UC3);

        for (int c = 0; c < 3; ++c) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    int A = (j > 0) ? frame.at<cv::Vec3b>(i, j - 1)[c] : 0;
                    int B = (i > 0) ? frame.at<cv::Vec3b>(i - 1, j)[c] : 0;
                    int C = (i > 0 && j > 0) ? frame.at<cv::Vec3b>(i - 1, j - 1)[c] : 0;

                    int predicted = predict(A, B, C);
                    int residual = golomb.decode(bitStream, m);
                    int pixel = predicted + residual;

                    frame.at<cv::Vec3b>(i, j)[c] = cv::saturate_cast<uchar>(pixel);
                }
            }
        }

        writer.write(frame);
    }

    writer.release();
}

int main() {
    string inputVideo = "./sample_video.mp4";
    string encodedFile = "encoded_video_lossless.bin";
    string decodedVideo = "decoded_video.avi";
    int m = 8;
    bool useInterleaving = true;

    encodeVideoLossless(inputVideo, encodedFile, m, useInterleaving);
    decodeVideoLossless(encodedFile, decodedVideo, useInterleaving);

    cout << "Done encoding and decoding video!" << endl;

    return 0;
}
