/*
 * helperFunctions.h
 *
 *  Created on: 10.12.2015
 *      Author: Standardbenutzer
 */

#ifndef HELPERFUNCTIONS_H_
#define HELPERFUNCTIONS_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "filterFunctions.h"
#include <windows.h>
#include "../objects/SettingObject.h"

using namespace cv;
using namespace std;

//helper fuctions
Point normalize (Point, Mat);

bool wPxl_in_Area(Point2f&, Mat&, float);

float get_length(Point, Point);
float get_length(Vec4i);

int chess_length(Point, Point);

int count_whitePixel(Point2f, Point2f, Mat);

Point2f topLeftPixel(Point2f, Mat);

Point2f bottomRightPixel(Point2f, Mat);

double getfilledRatio(Mat&, RotatedRect&);

void make_skelekton(Mat);

void make_adaptiv_bw(Mat&);

int get_Contour_Min_Dst (vector<ContourObject>, Point2f);

int get_min_Point_from_Rect (Point2f rect_point[4]);

Vec4i get_Border_Points_from_Rect(Point2f rect_point[4], bool);

bool getBoolValue(const char*);

int start_executable(string command);

void waitAnyKey(const SettingObject&);
#endif /* HELPERFUNCTIONS_H_ */
