/*
 * finderFunctions.h
 *
 *  Created on: 13.12.2015
 *      Author: Standardbenutzer
 */

#ifndef FINDERFUNCTIONS_H_
#define FINDERFUNCTIONS_H_

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
#include <map>
#include <iomanip>

#include "../ZBar/include/zbar.h"
#include "../Network/curl-7.34.0-win32/include/curl/curl.h"
#include "../Network/curl-7.34.0-win32/include/curl/easy.h"

using namespace zbar;
using namespace cv;
using namespace std;


vector<ContourObject> find_moments(Mat, int, Mat, Size);
vector<ContourObject> find_mser(Mat);
//void find_Colour_for_Groups(map<Scalar, vector< vector< ContourObject> > >&,Scalar, ContourObject);
//void find_groups(Mat, vector<ContourObject>);

vector<Vec4i> get_probabilistic_hough_lines(Mat);
vector<vector<Point2f> > get_corner_points(vector<Vec4i>, vector<ContourObject>, Mat);

bool get_barcode_string(Mat&, string&, string&, float&);
int writer(char *data, size_t size, size_t nmemb, string *buffer);
string curl_httpget(const string &url);
bool get_article_description(string&);

#endif /* FINDERFUNCTIONS_H_ */
