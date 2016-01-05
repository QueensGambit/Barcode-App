/*
 * filterFunctions.cpp
 *
 *  Created on: 13.12.2015
 *      Author: Standardbenutzer
 */

#include "filterFunctions.h"

vector<Point2f> filter_moments(vector<Point2f> mc, Mat skel) {

	int dst = 2, xDst = 0, yDst = 0;
	cout << "mc.size() = " << mc.size() << endl;
	vector<Point2f> tempMC;
	for (unsigned int i = 0; i < mc.size(); i++) {
		//weisser Punkt auf moment Zentrum
		if (skel.at<uchar>(Point(mc[i].x + xDst, mc[i].y + yDst)) == 255) {
			tempMC.push_back(mc[i]);
		}
	}

	return tempMC;
}

void filter_detected_Lines(vector<Vec4i> lines, vector<ContourObject> vecCO) {

	cout << "size of vecCO in filter_detected: " << vecCO.size() << endl;

	vector<Vec4i>::const_iterator it2 = lines.begin();

	ContourObject startContour;
	ContourObject endContour;

	vector<vector<Point2f> > cornerPoints;

	int startH = 0;
	int endH = 0;

	while (it2 != lines.end()) {

		Point2f start((*it2)[0], (*it2)[1]);
		Point2f end((*it2)[2], (*it2)[3]);
		cout << "startpoint: " << start << endl;
		cout << "endpoint: " << end << endl;
		cout << "entree" << endl;

			//startpoint inside countour?
			/*
			 * It returns positive (inside), negative (outside), or zero (on an edge) value,
			 * correspondingly. When measureDist=false , the return value is +1, -1, and 0
			 *
			 */

			//cout << "test: " << i << endl;
			//cout << "start " << pointPolygonTest(vecCO[i].getContour(), start, false) << endl;
			//cout << "ende " << pointPolygonTest(vecCO[i].getContour(), end, false) << endl;


//			if (pointPolygonTest(vecCO[i].getContour(), start, false) >= 0) {
			int sIndex = get_Contour_Min_Dst(vecCO, start);
			int eIndex = get_Contour_Min_Dst(vecCO, end);

			if (sIndex != -1 && eIndex != -1 ) {
				startContour = vecCO[sIndex];
				endContour = vecCO[eIndex];
				startH++;
				endH++;
				Point2f p[4];
				Point2f q[4];

				startContour.getRectPoints(p);
				endContour.getRectPoints(q);

				Vec4i pointsStart;
				pointsStart = get_Border_Points_from_Rect(p, true);

				vector<Point2f> tmpCPoints(4);

				tmpCPoints[0] = Point2f(pointsStart[0], pointsStart[1]);
				tmpCPoints[1] = Point2f(pointsStart[2], pointsStart[3]);

				Vec4i pointsEnd;
				pointsEnd = get_Border_Points_from_Rect(q, false);

				tmpCPoints[2] = Point2f(pointsEnd[0], pointsEnd[1]);
				tmpCPoints[3] = Point2f(pointsEnd[2], pointsEnd[3]);

				cout << "Angle: " << startContour.getAngle() << endl;

				cornerPoints.push_back(tmpCPoints);

			}
//			}

				//break;
//			}


		it2++;

	}
	Size s = Size(324, 244);
//	Size s = Size(1138, 1600);

	Mat mBarcodePoints =  Mat::zeros(s, CV_8UC3);
	RNG rng(12345);

	cout << "cornerPoints:" << cornerPoints.size() << endl;
	vector<Point2f> tmpCPoints(4);
	for(int p = 0; p < cornerPoints.size(); p++){
		tmpCPoints = cornerPoints[p];
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),rng.uniform(0, 255));
		for(int z = 0; z<4; z++){
			cout << "Punkt" << z << " " << tmpCPoints[z] << endl;

			circle(mBarcodePoints, tmpCPoints[z], 3, color, -1, 8, 0);
		}
	}

	namedWindow("Barocde Punkte", 1);
	imshow("Barocde Punkte", mBarcodePoints);

	cout << "hits start: " << startH << endl;
	cout << "hits end: " << endH << endl;

}

vector<ContourObject> filter_by_rect(vector<ContourObject> vecCO, Mat m,
		float threshWPxl, float threshAspect) {

	vector<RotatedRect> minRect(vecCO.size());
	vector<ContourObject> fVecCO;

	for (int i = 0; i < vecCO.size(); i++) {

		minRect[i] = minAreaRect(Mat(vecCO[i].getContour()));

		float width = minRect[i].size.width;
		float height = minRect[i].size.height;

		float ratio;

		if (width > height) {
			ratio = width / height;
		} else {
			ratio = height / width;
		}

		if (getfilledRatio(m, minRect[i]) >= threshWPxl
				&& ratio >= threshAspect) {
			Point2f rect_points[4];
			minRect[i].points(rect_points);
			vecCO[i].setRectPoints(rect_points);
			vecCO[i].setMassCenter(minRect[i].center);

			float angle = minRect[i].angle;
			if (angle < 0) {
				angle += 180;
			}
			vecCO[i].setAngle(angle);
			fVecCO.push_back(vecCO[i]);
		}

	}

	return fVecCO;
}

vector<ContourObject> filter_lines(vector<ContourObject> vecCO, Mat skel,
		int hitThresh) {
	Point last;
	Point temp;
	int hits = 0;
	vector<ContourObject> newVecCO;

	for (int i = 0; i < vecCO.size(); i++) {
		temp = vecCO[i].getFirstPoint();
		last = vecCO[i].getLastPoint();
		Vektor2d vektor(temp, last);

		float lambda = 0.0;
		float schritt = 0.1;
		while (lambda <= 1.0) {

			if (skel.at<uchar>(temp) == 255) {
				temp.x += lambda * vektor.getOrtsV().x;
				temp.y += lambda * vektor.getOrtsV().y;
				cout << "temp: " << temp << endl;

				hits++;
				cout << "hits: " << hits << endl;
			}
			lambda += schritt;
		}

		if (hits >= hitThresh) {
			newVecCO.push_back(vecCO[i]);
		}
	}
	return newVecCO;
}

vector<Point2f> island_filter(vector<Point2f> mc, Mat skel, int thresh) {

	Point2f tL, bR;
	vector<Point2f> temp;
	for (unsigned int i = 0; i < mc.size(); i++) {
		tL = topLeftPixel(mc[i], skel);
		bR = bottomRightPixel(mc[i], skel);

		if (count_whitePixel(tL, bR, skel) >= thresh) {
			temp.push_back(mc[i]);
		}
	}

	return temp;
}

vector<ContourObject> filter_by_dst(vector<ContourObject> vecCO, int pxlSum,
		float threshDst, Size size) {

	vector<ContourObject> fVecCO;
	Mat m = Mat::zeros(size, CV_8UC1);

	float dist;
	for (int i = 0; i < vecCO.size(); i++) {
		for (int z = 0; z < vecCO.size(); z++) {
			if (z != i) {
				dist = norm(
						vecCO[z].getMassCenter() - vecCO[i].getMassCenter());
				if (dist <= threshDst * pxlSum) { //0.00008
//								 cout << "dist: " << dist << endl;
					line(m, vecCO[z].getMassCenter(), vecCO[i].getMassCenter(),
							Scalar(255, 255, 255), 2, 8);
					fVecCO.push_back(vecCO[i]);
					break;
				}
			}
		}
	}

	namedWindow("dst", WINDOW_AUTOSIZE);
	imshow("dst", m);

	return fVecCO;
}

void cluster_rect(Mat b, vector<ContourObject> vecCO) {

	draw_minRectangles(vecCO, b);

	namedWindow("Rect", CV_WINDOW_AUTOSIZE);
	imshow("Rect", b);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(21, 7));
	morphologyEx(b, b, MORPH_CLOSE, kernel);

	erode(b, b, kernel);

	blur(b, b, Size(9, 9));
	dilate(b, b, kernel);

	namedWindow("RectDilate", CV_WINDOW_AUTOSIZE);
	imshow("RectDilate", b);

}

//sideEffect for pLine
void filter_hough_lines(vector<Vec4i>& pLine, float threshDst, int pxlSum) {

	float dist;
	Vector<int> delIndex;
	for (int i = 0; i < pLine.size(); i++) {
		for (int z = 0; z < pLine.size(); z++) {
			if (z != i) {
				for (int j = 0; j < 2; j++) {
					for (int k = 0; k < 2; k++) {
						dist = norm(pLine[z].val[k] - pLine[i].val[j]);
						if (dist <= threshDst * pxlSum) {
							if (norm(pLine[i]) < norm(pLine[z])) {
								//delete z
								bool ok = true;
								for (int l = 0; l < delIndex.size(); l++) {
									if (delIndex[l] == z) {
										ok = false;
									}
								}
								if (ok) {
									delIndex.push_back(z);
								}
							} else {
								//delete i
								bool ok = true;
								for (int l = 0; l < delIndex.size(); l++) {
									if (delIndex[l] == i) {
										ok = false;
									}
								}
								if (ok) {
									delIndex.push_back(i);
								}
							}

						}
					}
				}
			}
		}
	}

	//delete Indexes
	for (int i = 0; i < delIndex.size(); i++) {
		cout << "delIndex[" << i << "]: " << delIndex[i] << endl;
		pLine.erase(pLine.begin() + delIndex[i] - i);
	}
	cout << "pLine.size: " << pLine.size() << endl;
}

vector<Vec4i> filter_hough_lines2(vector<Vec4i>& pLines,
		vector<ContourObject> vecCO) {

//	cout << "pLine.size(): " << pLines.size() << endl;
	vector<int> labels;

	//partition sorts the line into similar equivalence classes
	int numberOfLines = partition(pLines, labels, isEqual);
	/*	cout << "numberOfLines: " << numberOfLines << endl;
	 for (int i = 0; i < labels.size(); i++) {
	 cout << "label[" << i << "]: " << labels[i] << endl;
	 }
	 //	cout << "labels.size(): " << labels.size() << endl;*/

	//get the longest line out of the equivalence classes
	vector<Vec4i> fLines(numberOfLines);
	float maxLength;
	int indexWithMaxLength;
	cout << "--------- direktes Auslesen ---------------------" << endl;
	cout << "single points:" << endl;
	for (int i = 0; i < numberOfLines; i++) {
		maxLength = 0;
		for (int z = 0; z < labels.size(); z++) {
			if (labels[z] == i) {
				if (norm(pLines[z]) > maxLength) {
					indexWithMaxLength = z;
					maxLength = norm(pLines[z]);
				}
			}
		}
		fLines[i] = pLines[indexWithMaxLength];
		cout << "Line[" << i << "]:" << endl;
		cout << "xStart: " << fLines[i][0] << " ";
		cout << "yStart: " << fLines[i][1] << endl;
		cout << "xEnd: " << fLines[i][2] << " ";
		cout << "yEnd: " << fLines[i][3] << endl;
	}
	cout << "-------------------------------------------" << endl;
	//lkajsdfklajsflkj
	filter_detected_Lines(fLines, vecCO);
	return fLines;
}

bool isEqual(const Vec4i& _l1, const Vec4i& _l2) {
	Vec4i l1(_l1), l2(_l2);

	float length1 = sqrtf(
			(l1[2] - l1[0]) * (l1[2] - l1[0])
					+ (l1[3] - l1[1]) * (l1[3] - l1[1]));
	float length2 = sqrtf(
			(l2[2] - l2[0]) * (l2[2] - l2[0])
					+ (l2[3] - l2[1]) * (l2[3] - l2[1]));

	float product = (l1[2] - l1[0]) * (l2[2] - l2[0])
			+ (l1[3] - l1[1]) * (l2[3] - l2[1]);

	if (fabs(product / (length1 * length2)) < cos(CV_PI / 30))
		return false;

	float mx1 = (l1[0] + l1[2]) * 0.5f;
	float mx2 = (l2[0] + l2[2]) * 0.5f;

	float my1 = (l1[1] + l1[3]) * 0.5f;
	float my2 = (l2[1] + l2[3]) * 0.5f;
	float dist = sqrtf((mx1 - mx2) * (mx1 - mx2) + (my1 - my2) * (my1 - my2));

//    cout << "dist: " << dist << endl;
	if (dist > max(length1, length2) * 0.1f) //0.5f
		return false;

	return true;
}
