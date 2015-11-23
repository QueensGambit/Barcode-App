#ifndef VEKTOR2D_H_
#define VEKTOR2D_H_

/*
 * Vektor2d.h
 *
 *  Created on: 19.11.2015
 *      Author: Suhay
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class Vektor2d {
private:
	Point ortsV;
public:
	Vektor2d(Point s, Point l);
	Vektor2d(Point2f s, Point2f l);
	virtual ~Vektor2d();
	Point getOrtsV();

	friend ostream& operator<<(ostream& out, Vektor2d& v) {
		out << "[" << v.ortsV.x << ", " << v.ortsV.y << "]";
		return out;
	}
};

#endif /* VEKTOR2D_H_ */
