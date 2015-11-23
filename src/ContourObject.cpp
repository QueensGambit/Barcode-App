/*
 * ContourObject.cpp
 *
 *  Created on: 18.11.2015
 *      Author: Standardbenutzer
 */

#include "ContourObject.h"

ContourObject::ContourObject(Point2f mc, vector<Point>c) {
			massCenter = mc;
			contour.swap(c);
			firstPoint = contour[0];
			lastPoint = contour[contour.size()-1];
			//lastPoint = contour[4];
}

ContourObject::~ContourObject() {
	// TODO Auto-generated destructor stub
}

//getter setter
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
/*ostream& ContourObject::operator<<(ostream& out, ContourObject& c) {
	out << "massCenter: [" << c.massCenter.x << ", " << c.massCenter.y << "]" << endl;
	out << "contLength: " << c.contour.size();
	return out;
}*/

void ContourObject::drawContourOnMat(Mat m) {
	for (int i = 0; i < contour.size(); i++) {
		circle(m, contour[i], 4, Scalar(0,255,0), -1, 8, 0);
	}
}
