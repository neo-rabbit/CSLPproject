#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "../Deliverable2/Bitstream.h"
#include "./Golomb.h"

using namespace std;

int predict(int A, int B, int C) {
    if (C >= std::max(A, B)) return std::min(A, B);
    if (C <= std::min(A, B)) return std::max(A, B);
    return A + B - C;
}

void encodeVideoWithInterFrame(const string& videoPath, const string& outputPath, int m, int iframeInterval, int blockSize, int searchRange, bool useInterleaving) {
    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        cerr << "Failed to open video!" << endl;
        return;
    }

    int rows = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    int cols = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));

    BitStream bitStream(outputPath, true);
    GolombCoding golomb(useInterleaving);

    bitStream.writeBits(rows, 16);
    bitStream.writeBits(cols, 16);
    bitStream.writeBits(m, 8);
    bitStream.writeBits(iframeInterval, 8);
    bitStream.writeBits(blockSize, 8);
    bitStream.writeBits(searchRange, 8);
    bitStream.writeBits(totalFrames, 16);

    cv::Mat prevFrame;

    for (int frameIdx = 0; frameIdx < totalFrames; ++frameIdx) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

        bitStream.writeBit((frameIdx % iframeInterval == 0) ? 1 : 0); // I-frame or P-frame

        if (frameIdx % iframeInterval == 0) {
            // I-frame (intra-frame)
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    int A = (j > 0) ? frame.at<uchar>(i, j - 1) : 0;
                    int B = (i > 0) ? frame.at<uchar>(i - 1, j) : 0;
                    int C = (i > 0 && j > 0) ? frame.at<uchar>(i - 1, j - 1) : 0;

                    int predicted = predict(A, B, C);
                    int residual = frame.at<uchar>(i, j) - predicted;
                    golomb.encode(bitStream, residual, m);
                }
            }
            prevFrame = frame.clone();
        } else {
            // P-frame (inter-frame)
            for (int i = 0; i < rows; i += blockSize) {
                for (int j = 0; j < cols; j += blockSize) {

                    if (i + blockSize > rows || j + blockSize > cols) continue;

                    int bestDx = 0, bestDy = 0;
                    double bestMSE = DBL_MAX;

                    for (int dy = -searchRange; dy <= searchRange; ++dy) {
                        for (int dx = -searchRange; dx <= searchRange; ++dx) {
                            int refX = j + dx;
                            int refY = i + dy;
                            if (refX < 0 || refY < 0 || refX + blockSize > cols || refY + blockSize > rows)
                                continue;

                            double mse = 0.0;
                            for (int y = 0; y < blockSize; ++y) {
                                for (int x = 0; x < blockSize; ++x) {
                                    int diff = frame.at<uchar>(i + y, j + x) - prevFrame.at<uchar>(refY + y, refX + x);
                                    mse += diff * diff;
                                }
                            }
                            mse /= (blockSize * blockSize);

                            if (mse < bestMSE) {
                                bestMSE = mse;
                                bestDx = dx;
                                bestDy = dy;
                            }
                        }
                    }

                    // Encode motion vector
                    golomb.encode(bitStream, bestDx, m);
                    golomb.encode(bitStream, bestDy, m);

                    // Encode residuals
                    for (int y = 0; y < blockSize; ++y) {
                        for (int x = 0; x < blockSize; ++x) {
                            int currentX = j + x;
                            int currentY = i + y;
                            int refX = currentX + bestDx;
                            int refY = currentY + bestDy;
                            if (refX < 0 || refY < 0 || refX >= cols || refY >= rows) continue;

                            int predicted = prevFrame.at<uchar>(refY, refX);
                            int residual = frame.at<uchar>(currentY, currentX) - predicted;
                            golomb.encode(bitStream, residual, m);
                        }
                    }
                }
            }
            prevFrame = frame.clone();
        }
    }
    cap.release();
}

int main() {
    string videoPath = "./sample_video.mp4";
    string outputFile = "encoded_interframe.bin";
    int m = 8;
    int iframeInterval = 10;
    int blockSize = 8;
    int searchRange = 4;
    bool useInterleaving = true;

    encodeVideoWithInterFrame(videoPath, outputFile, m, iframeInterval, blockSize, searchRange, useInterleaving);

    cout << "Encoding complete (Inter-frame)." << endl;
    return 0;
}