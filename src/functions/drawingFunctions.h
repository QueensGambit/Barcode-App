/*
 * drawingFunctions.h
 *
 *  Created on: 13.12.2015
 *      Author: Standardbenutzer
 */
#ifndef DRAWINGFUNCTIONS_H_
#define DRAWINGFUNCTIONS_H_

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
#include "filterFunctions.h"

using namespace cv;
using namespace std;

void draw_Circles(vector<Point2f>, Mat);
void draw_Circles(vector<ContourObject>, Mat);

void draw_Lines(vector<ContourObject>, Mat);
void draw_minRectangles (vector<ContourObject> vecCO, Mat m);
Mat draw_massCenter(vector<ContourObject>, Size);
void draw_hough_lines(Mat , vector<Vec4i>);
void draw_fit_line(vector<Point2f>, Mat);
void draw_article_description(const string&, const string&);

void draw_barcode(Mat&, const string&, const string&, size_t&);
#endif /* DRAWINGFUNCTIONS_H_ */
