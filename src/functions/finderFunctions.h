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
#include "../objects/ContourObject.h"
#include "helperFunctions.h"
#include "../objects/Vektor2d.h"
#include "drawingFunctions.h"
#include <map>
#include <iomanip>
#include <sstream>
#include <fstream>

#include "helperFunctions.h"
#include "../objects/SettingObject.h"

#include "../ZBar/include/zbar.h"
#include "../Network/curl-7.34.0-win32/include/curl/curl.h"
#include "../Network/curl-7.34.0-win32/include/curl/easy.h"

using namespace zbar;
using namespace cv;
using namespace std;


vector<ContourObject> find_moments(Mat, int, Mat, Size); // !not in use!
vector<ContourObject> find_mser(Mat, const SettingObject&);
//void find_Colour_for_Groups(map<Scalar, vector< vector< ContourObject> > >&,Scalar, ContourObject);// !not in use!
//void find_groups(Mat, vector<ContourObject>);// !not in use!

vector<Vec4i> get_probabilistic_hough_lines(Mat);
vector<vector<Point2f> > get_corner_points(vector<Vec4i>, vector<ContourObject>, Mat, const SettingObject&);

bool get_barcode_string(Mat&, string&, string&, float&, size_t&, const SettingObject&);
int writer(char *data, size_t size, size_t nmemb, string *buffer);
string curl_httpget(const string &url);

bool get_article_description_internet(const string&, string&, string&);
bool get_article_descr_csv(const char*, string&, string& article, string& descr);
bool add_article_descr_to_csv(const char*, string&, string&, string&);

#endif /* FINDERFUNCTIONS_H_ */
