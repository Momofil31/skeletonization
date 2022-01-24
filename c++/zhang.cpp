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

void getNeighbours(std::vector<uchar> &neighbours, const int &i, const int &j,
                   const cv::Mat &img) {
  neighbours.push_back(img.at<uchar>(i - 1, j));
  neighbours.push_back(img.at<uchar>(i - 1, j + 1));
  neighbours.push_back(img.at<uchar>(i, j + 1));
  neighbours.push_back(img.at<uchar>(i + 1, j + 1));
  neighbours.push_back(img.at<uchar>(i + 1, j));
  neighbours.push_back(img.at<uchar>(i + 1, j - 1));
  neighbours.push_back(img.at<uchar>(i, j - 1));
  neighbours.push_back(img.at<uchar>(i - 1, j - 1));
}

int zeroOnePatterns(const std::vector<uchar> &neighbours) {
  int count = 0;
  int i = 0;
  for (i = 0; i < neighbours.size() - 1; i++) {
    count += (neighbours[i] == 0 && neighbours[i + 1] == 1) ? 1 : 0;
  }
  count += (neighbours[i] == 0 && neighbours[0] == 1) ? 1 : 0;
  return count;
}

cv::Mat zhangSkeletonize(cv::Mat &image) {
  cv::Mat skeleton;
  cv::Mat cleanedSkeleton;
  cv::copyMakeBorder(image, skeleton, 1, 1, 1, 1, cv::BORDER_CONSTANT,
                     cv::Scalar(0));
  skeleton /= 255;
  skeleton.copyTo(cleanedSkeleton);
  uint32_t rows = skeleton.rows, cols = skeleton.cols;
  uint32_t step = 0;
  bool pixelRemoved = true;
  bool firstSubIter = true;

  while (pixelRemoved) {
    pixelRemoved = false;
    for (int i = 1; i < rows - 1; i++) {
      for (int j = 1; j < cols - 1; j++) {
        auto pixel = skeleton.at<uchar>(i, j);
        // cout << (int) pixel << endl;
        if (pixel == 1) {
          std::vector<uchar> neighbours;
          getNeighbours(neighbours, i, j, skeleton);
          uchar P2 = neighbours[0];
          uchar P4 = neighbours[2];
          uchar P6 = neighbours[4];
          uchar P8 = neighbours[6];
          int nonZeroNeighbours =
              std::accumulate(neighbours.begin(), neighbours.end(), 0);

          if (nonZeroNeighbours >= 2 && nonZeroNeighbours <= 6 &&
              zeroOnePatterns(neighbours) == 1) {
            int m1 = firstSubIter == 0 ? (P2 * P4 * P6) : (P2 * P4 * P8);
            int m2 = firstSubIter == 0 ? (P4 * P6 * P8) : (P2 * P6 * P8);
            if (m1 == 0 && m2 == 0) {
              cleanedSkeleton.at<uchar>(i, j) = 0;
              pixelRemoved = true;
            }
          }
        }
      }
    }
    firstSubIter = !firstSubIter;
    cleanedSkeleton.copyTo(skeleton);
  }
  skeleton *= 255;
  return skeleton(cv::Range(1, rows - 1), cv::Range(1, cols - 1));
}

void innerloop(const Mat &im, Mat &marker, const int iter, const int i_start,
               const int i_end, const int j_start, const int j_end) {
  for (int i = i_start; i < i_end - 1; i++) {
    for (int j = j_start; j < j_end - 1; j++) {
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

void thinningIteration(Mat &im, int iter) {
  Mat marker = Mat::zeros(im.size(), CV_8UC1);

  for (int i = 1; i < im.rows - 1; i++) {
    for (int j = 1; j < im.cols - 1; j++) {
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

  im &= ~marker;
}

/**
 * Function for thinning the given binary image
 */
void thinning(Mat &im) {
  im /= 255;

  Mat prev = Mat::zeros(im.size(), CV_8UC1);
  Mat diff;

  do {
    thinningIteration(im, 0);
    thinningIteration(im, 1);
    absdiff(im, prev, diff);
    im.copyTo(prev);
  } while (countNonZero(diff) > 0);

  im *= 255;
}

int main(int argc, char **argv) {
  // if (argc != 2) {
  //   cout << "Expecting a image file to be passed to program" << endl;
  //   return -1;
  // }

  cv::Mat img_rbg = cv::imread("../../sample_images/horse.png");

  if (img_rbg.empty()) {
    cout << "Not a valid image file" << endl;
    return -1;
  }

  // the following is an alternative to Mat img_bw = im_gray > 128
  cv::Mat img;
  cvtColor(img_rbg, img, cv::COLOR_BGRA2GRAY);
  threshold(img, img, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);

  cv::namedWindow("Original greyscale", cv::WINDOW_AUTOSIZE);
  cv::imshow("Original greyscale", img);
  cv::waitKey(0);
  cv::destroyAllWindows();

  auto start = std::chrono::high_resolution_clock::now();

  cv::Mat out = zhangSkeletonize(img);

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  cout << "Duration: " << duration.count() << " milliseconds" << endl;

  cv::namedWindow("Simple Demo", cv::WINDOW_AUTOSIZE);
  cv::imshow("Simple Demo", out);
  cv::waitKey(0);
  cv::destroyAllWindows();

  return 0;
}