/*
 * loaderFunctions.h
 *
 *  Created on: 07.01.2016
 *      Author: Standardbenutzer
 */

#ifndef LOADERFUNCTIONS_H_
#define LOADERFUNCTIONS_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

#include "finderFunctions.h"
#include "../OpenCV_Main.h"

using namespace cv;
using namespace std;

Mat get_image_from_webcam(char, char);

#endif /* LOADERFUNCTIONS_H_ */
