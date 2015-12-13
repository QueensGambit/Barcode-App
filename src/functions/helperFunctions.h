/*
 * helperFunctions.h
 *
 *  Created on: 10.12.2015
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

using namespace cv;
using namespace std;

#ifndef HELPERFUNCTIONS_H_
#define HELPERFUNCTIONS_H_

//helper fuctions
Point normalize (Point, Mat);

bool wPxl_in_Area(Point2f&, Mat&, float);

double get_length(Point, Point);

int chess_length(Point, Point);

int count_whitePixel(Point2f, Point2f, Mat);

Point2f topLeftPixel(Point2f, Mat);

Point2f bottomRightPixel(Point2f, Mat);

double getfilledRatio(Mat&, RotatedRect&);

void make_skelekton(Mat);

void probabilistic_hough();

#endif /* HELPERFUNCTIONS_H_ */
