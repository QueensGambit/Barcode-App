/*
 * OpenCV_Main.h
 *
 *  Created on: 19.01.2016
 *      Author: Anonym
 */

#ifndef OPENCV_MAIN_H_
#define OPENCV_MAIN_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "objects/ContourObject.h"
#include "objects/Vektor2d.h"
#include "objects/SettingObject.h"

#include "functions/helperFunctions.h"
#include "functions/drawingFunctions.h"
#include "functions/filterFunctions.h"
#include "functions/finderFunctions.h"
#include "functions/loaderFunctions.h"
#include "functions/transformerFunctions.h"

using namespace cv;
using namespace std;

int get_Barcode( Mat&, SettingObject&);

#endif /* OPENCV_MAIN_H_ */
