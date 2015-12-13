/*
 * filterFunctions.h
 *
 *  Created on: 13.12.2015
 *      Author: Standardbenutzer
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "../ContourObject.h"
#include "helperFunctions.h"
#include "../Vektor2d.h"
#include "drawingFunctions.h"

using namespace cv;
using namespace std;

#ifndef FILTERFUNCTIONS_H_
#define FILTERFUNCTIONS_H_

vector<Point2f> filter_moments(vector<Point2f>, Mat);
vector<ContourObject> filter_by_rect(vector<ContourObject>, Mat, float, float);

vector<ContourObject> filter_lines(vector<ContourObject>, Mat, int);

vector<Point2f> island_filter(vector<Point2f>, Mat, int);

vector<ContourObject> filter_by_dst(vector<ContourObject>, int, float, Size);

void cluster_rect(Mat, vector<ContourObject>);
#endif /* FILTERFUNCTIONS_H_ */
