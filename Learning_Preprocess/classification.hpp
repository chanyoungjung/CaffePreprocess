/*
 * classification.hpp
 *
 *  Created on: Jan 5, 2016
 *      Author: root
 */

#ifndef CLASSIFICATION_HPP_
#define CLASSIFICATION_HPP_

#include <caffe/caffe.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>


using namespace caffe;  // NOLINT(build/namespaces)
using std::string;

/* Pair (label, confidence) representing a prediction. */
typedef std::pair<string, float> Prediction;

class Classifier {
 public:
  Classifier();

  void net_initialize(const string& model_file,
                      const string& trained_file,
                      const string& label_file,
                      const string& mean_file);

  std::vector<Prediction> Classify(const cv::Mat& img, int N = 1);

  void SetMean(const string& mean_file);

  std::vector<float> Predict(const cv::Mat& img);

  void WrapInputLayer(std::vector<cv::Mat>* input_channels);

  void Preprocess(const cv::Mat& img,
                  std::vector<cv::Mat>* input_channels);

 private:
  std::shared_ptr<Net<float> > net_;
  cv::Size input_geometry_;
  int num_channels_;
  cv::Mat mean_;
  std::vector<string> labels_;
};

#endif /* CLASSIFICATION_HPP_ */
