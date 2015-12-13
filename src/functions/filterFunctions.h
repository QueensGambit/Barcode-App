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

using namespace cv;
using namespace std;

#ifndef FILTERFUNCTIONS_H_
#define FILTERFUNCTIONS_H_

vector<Point2f> filter_moments(vector<Point2f>, Mat);
vector<ContourObject> filter_by_rect(vector<ContourObject>, Mat, float, float);

#endif /* FILTERFUNCTIONS_H_ */
