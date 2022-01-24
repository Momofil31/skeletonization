#include <chrono>
#include <iostream>
#include <numeric>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>

using cv::Mat;
using std::cout;
using std::endl;

void innerloop(const Mat &im, Mat &marker, const int iter, const int i_start,
               const int i_end, const int j_start, const int j_end) {
  for (int i = i_start; i <= i_end; i++) {
    for (int j = j_start; j <= j_end; j++) {
      uchar p2 = im.at<uchar>(i - 1, j);
      uchar p3 = im.at<uchar>(i - 1, j + 1);
      uchar p4 = im.at<uchar>(i, j + 1);
      uchar p5 = im.at<uchar>(i + 1, j + 1);
      uchar p6 = im.at<uchar>(i + 1, j);
      uchar p7 = im.at<uchar>(i + 1, j - 1);
      uchar p8 = im.at<uchar>(i, j - 1);
      uchar p9 = im.at<uchar>(i - 1, j - 1);

      int A = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
              (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
              (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
              (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
      int B = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
      int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
      int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

      if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
        marker.at<uchar>(i, j) = 1;
    }
  }
}

void thinningIteration(Mat &im, int iter, const int &num_workers) {
  Mat marker =
      Mat::zeros(im.size(), CV_8UC1);  // To mark which pixels to delete
  int n = im.rows, m = im.cols;
  std::vector<std::thread> threads;

  // splits the image in num_workers * num_workers parts and assign each one to
  // a thread
  int q = num_workers;
  for (int i = 0; i < q; i++) {
    for (int j = 0; j < q; j++) {
      int row_s = (i * n) / q + 1;
      int row_e = (i + 1) * n / q - ((i + 1) / q);
      int col_s = (j * m) / q + 1;
      int col_e = (j + 1) * m / q - ((j + 1) / q);
      threads.emplace_back(innerloop, std::ref(im), std::ref(marker), iter,
                           row_s, row_e, col_s, col_e);
    }
  }

  for (auto &t : threads) {
    t.join();
  }
  im &= ~marker;  // delete pixels from the image via bitwise operations
}

void skeletonize(Mat &im, const int &num_workers = 3) {
  im /= 255;

  Mat prev = Mat::zeros(im.size(), CV_8UC1);
  Mat diff;

  // Main skeletonization loop
  do {
    thinningIteration(im, 0, num_workers);  // sub-iteration 1
    thinningIteration(im, 1, num_workers);  // sub-iteration 2
    absdiff(im, prev, diff);
    im.copyTo(prev);
  } while (countNonZero(diff) >
           0);  // if the image has not been modified break the loop

  im *= 255;
}

int main() {
  cv::Mat img_rbg = cv::imread("../../sample_images/horse.png");

  if (img_rbg.empty()) {
    cout << "Not a valid image file" << endl;
    return -1;
  }
  //    cv::namedWindow("Original rbg", cv::WINDOW_AUTOSIZE);
  //    cv::imshow("Original rbg", img_rbg);
  //    cv::waitKey(0);
  //    cv::destroyAllWindows();

  // Convert rbg to greyscale (0-255) 1 channel image
  cv::Mat img;
  cvtColor(img_rbg, img, cv::COLOR_BGRA2GRAY);
  // for horse_large.png
  // cv::bitwise_not(img, img);
  threshold(img, img, 128, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

  cv::namedWindow("Original greyscale", cv::WINDOW_AUTOSIZE);
  cv::imshow("Original greyscale", img);
  cv::waitKey(0);
  cv::destroyAllWindows();

  // Perform skeletonization
  auto start = std::chrono::high_resolution_clock::now();
  cv::Mat out;
  img.copyTo(out);
  skeletonize(out);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  cout << "Duration: " << duration.count() << " milliseconds" << endl;

  // Display skeletonized image
  cv::namedWindow("Skeleton", cv::WINDOW_AUTOSIZE);
  cv::imshow("Skeleton", out);
  cv::waitKey(0);
  cv::destroyAllWindows();

  return 0;
}
