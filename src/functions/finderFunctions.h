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

using namespace cv;
using namespace std;

void find_Colour_for_Groups(Scalar, ContourObject);
vector<ContourObject> find_moments(Mat, int, Mat, Size);
vector<ContourObject> find_mser(Mat);
vector<vector<ContourObject> >find_groups(vector<Vec4i>&);

#endif /* FINDERFUNCTIONS_H_ */
