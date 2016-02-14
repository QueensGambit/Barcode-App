/*
 * ContourObject.h
 *
 *  Created on: 18.11.2015
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

#ifndef CONTOUROBJECT_H_
#define CONTOUROBJECT_H_

class ContourObject {
private:
	Point2f massCenter;
	vector<Point> contour;
	Point firstPoint, lastPoint;
	Point2f rect_point[4];

	//the angle and the quadrant is not actively used
	float angle;
	int quadrant;

public:

	ContourObject(Point2f, vector<Point>);
	ContourObject(vector<Point>);
	ContourObject();

	virtual ~ContourObject();

	//getter setter
	void setMassCenter(Point2f);
	Point2f getMassCenter();
	vector<Point> getContour();
	Point getLastPoint();
	Point getFirstPoint();
	void setRectPoints(Point2f rectAngle[4]);
	void getRectPoints(Point2f[4]);

	void setAngle(float);
	float getAngle();
	int getQuadrant();

	friend ostream& operator<<(ostream& out, ContourObject& c) {
		out << "massCenter: [" << c.massCenter.x << ", " << c.massCenter.y << "]" << endl;
		out << "contLength: " << c.contour.size() << endl;
		out << "firstPoint: " << c.firstPoint << " lasPoint: " << c.lastPoint << endl;
		return out;
	}

	void drawContourOnMat(Mat);

};

#endif /* CONTOUROBJECT_H_ */
