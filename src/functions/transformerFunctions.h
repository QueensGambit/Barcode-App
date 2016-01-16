/*
 * transformerFunctions.h
 *
 *  Created on: 09.01.2016
 *      Author: Anonym
 */

#ifndef TRANSFORMERFUNCTIONS_H_
#define TRANSFORMERFUNCTIONS_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

#include "helperFunctions.h"

using namespace cv;
using namespace std;

vector<Mat> p_transform(Mat, vector<vector<Point2f> > );
bool speak_article_descr(const string&, const string&);
#endif /* TRANSFORMERFUNCTIONS_H_ */
