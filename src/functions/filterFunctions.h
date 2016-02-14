/*
 * filterFunctions.h
 *
 *  Created on: 13.12.2015
 *      Author: Standardbenutzer
 */

#ifndef FILTERFUNCTIONS_H_
#define FILTERFUNCTIONS_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "../objects/ContourObject.h"
#include "helperFunctions.h"
#include "../objects/Vektor2d.h"
#include "drawingFunctions.h"

using namespace cv;
using namespace std;
vector<Point2f> filter_moments(vector<Point2f>, Mat);// !not in use!
vector<ContourObject> filter_by_rect(vector<ContourObject>, Mat, float, float);

vector<ContourObject> filter_lines(vector<ContourObject>, Mat, int);// !not in use!

vector<Point2f> island_filter(vector<Point2f>, Mat, int);// !not in use!

vector<ContourObject> filter_by_dst(vector<ContourObject>, int, float, Size);// !not in use!
void cluster_rect(Mat, vector<ContourObject>);// !not in use!

void filter_hough_lines (vector<Vec4i>&, float, int);// !not in use!
vector<Vec4i> filter_hough_lines2 (vector<Vec4i>& );
bool isEqual(const Vec4i& _l1, const Vec4i& _l2);


#endif /* FILTERFUNCTIONS_H_ */
