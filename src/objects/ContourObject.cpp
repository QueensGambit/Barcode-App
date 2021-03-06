/*
 * ContourObject.cpp
 *
 *  Created on: 18.11.2015
 *      Author: Standardbenutzer
 */

#include "ContourObject.h"

/*
 * Constructor to initialize a ContourObject
 * with its area-contour and its masscenter
 */
ContourObject::ContourObject(Point2f mc, vector<Point>c) {
			massCenter = mc;
			contour.swap(c);
			firstPoint = contour[0];
			lastPoint = contour[contour.size()-1];
			angle = 0;
			quadrant = 0;
			//lastPoint = contour[4];
}

/*
 * Constructor to initialize a ContourObject
 * with its area-contour
 */
ContourObject::ContourObject(vector<Point> c) {
	contour.swap(c);
	angle = 0;
	quadrant = 0;
}

/*
 * default constructor
 */
ContourObject::ContourObject() {
	angle = 0;
	quadrant = 0;
}

/*
 * default destructor
 */
ContourObject::~ContourObject() {
	// TODO Auto-generated destructor stub
}

//getter setter Methods:
void ContourObject::setMassCenter(Point2f p){
	massCenter = p;
}

Point2f ContourObject::getMassCenter() {
	return massCenter;
}

vector<Point> ContourObject::getContour() {
	return contour;
}

Point ContourObject::getLastPoint(){
	return lastPoint;
}

Point ContourObject::getFirstPoint(){
	return firstPoint;
}

void ContourObject::setRectPoints(Point2f point[4]){
	for(int i =0; i<4;i++){
		rect_point[i] = point[i];
	}
}

/*
 * function returns the rectPoints via the parameter
 */
void ContourObject::getRectPoints(Point2f p[4]){
//	return rect_point;
	for (int i = 0; i<4; i++){
		p[i] = rect_point[i];
	}
}
/*ostream& ContourObject::operator<<(ostream& out, ContourObject& c) {
	out << "massCenter: [" << c.massCenter.x << ", " << c.massCenter.y << "]" << endl;
	out << "contLength: " << c.contour.size();
	return out;
}*/

/*
 * draws single circles for every point of the contour
 */
void ContourObject::drawContourOnMat(Mat m) {
	for (size_t i = 0; i < contour.size(); i++) {
		circle(m, contour[i], 4, Scalar(0,255,0), -1, 8, 0);
	}
}

/*
 * sets up the angle of the ContourObject
 * and the associating 'quadrant' with it
 */
void ContourObject::setAngle(float a) {
	angle = a;
	if (a >= 0 && a < 90) {
		quadrant = 1;
	}
	if (a >= 90 && a < 180) {
		quadrant = 2;
	}
	if (a >= 180 && a < 270) {
		quadrant = 3;
	}
	if (a >= 270 && a < 360) {
		quadrant = 4;
	}

}

float ContourObject::getAngle() {
	return angle;
}

int ContourObject::getQuadrant() {
	return quadrant;
}
