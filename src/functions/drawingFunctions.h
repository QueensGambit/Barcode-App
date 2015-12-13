/*
 * drawingFunctions.h
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

using namespace cv;
using namespace std;

#ifndef DRAWINGFUNCTIONS_H_
#define DRAWINGFUNCTIONS_H_

void draw_Circles(vector<Point2f>, Mat);
void draw_Circles(vector<ContourObject>, Mat);

void draw_Lines(vector<ContourObject>, Mat);
void draw_minRectangles (vector<ContourObject> vecCO, Mat m);

#endif /* DRAWINGFUNCTIONS_H_ */
