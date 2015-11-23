/*
 * Vektor2d.cpp
 *
 *  Created on: 19.11.2015
 *      Author: Suhay
 */

#include "Vektor2d.h"

Vektor2d::Vektor2d(Point s, Point l) {
	ortsV.x = l.x-s.x;
	ortsV.y = l.y-s.y;
}

Vektor2d::Vektor2d(Point2f s, Point2f l) {
	ortsV.x = l.x-s.x;
	ortsV.y = l.y-s.y;
}

Vektor2d::~Vektor2d() {
	// TODO Auto-generated destructor stub
}
Point Vektor2d::getOrtsV(){
	return ortsV;
}
