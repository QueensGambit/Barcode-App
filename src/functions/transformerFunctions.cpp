/*
 * transformerFunctions.cpp
 *
 *  Created on: 09.01.2016
 *      Author: Anonym
 */

#include "transformerFunctions.h"


void p_transform(Mat src, vector<vector<Point2f> > cornerPoints) {


	Mat transform = Mat::zeros(300, 1200, CV_8UC3);


//	Mat cropImage;
//	Rect roi;
//	for (int i = 0; i < cornerPoints.size(); i++) {
//		roi = boundingRect(cornerPoints[i]);
//	}
//
//	Size s(roi.width * .1, roi.height * .1);
//	Point offset(s.width / 2, s.height / 2);			//shifting the rectangle
//	roi += s;
//	roi -= offset;
//
//	Mat croppedRef(src, roi);						//crop and copy the region
//	croppedRef.copyTo(cropImage);
//	namedWindow("cropped", 0);
//	imshow("cropped", cropImage);



	///sort corners to correct order
	for (int i = 0; i < cornerPoints.size(); i++) {
/*
		Point2f tmpP2, tmpP3, tmpP4;

		tmpP4 = cornerPoints[i][1];
		tmpP3 = cornerPoints[i][3];
		tmpP2 = cornerPoints[i][2];

		cornerPoints[i][1] = tmpP2;
		cornerPoints[i][2] = tmpP3;
		cornerPoints[i][3] = tmpP4;
		*/



		Point2f tmpP[4];

		tmpP[0] = cornerPoints[i][1];
		tmpP[1] = cornerPoints[i][0];
		tmpP[2] = cornerPoints[i][2];
		tmpP[3] = cornerPoints[i][3];

		cornerPoints[i][0] = tmpP[0];
		cornerPoints[i][1] = tmpP[1];
		cornerPoints[i][2] = tmpP[2];
		cornerPoints[i][3] = tmpP[3];

		/*for (int z = 0; z < 4; z++) {
			cout << "cornerPoints[" << z << "]:" << cornerPoints[i][z] << endl;
		}*/
	}


	int scaleVal = 20;
		///zoom out
		for (int i = 0; i < cornerPoints.size(); i++) {

			/*for (int z = 0; z < 4; z++) {
				cout << "cornerPoints[" << z << "]:" << cornerPoints[i][z] << endl;
			}*/
/*
			cornerPoints[i][0].x -= scaleVal;
			cornerPoints[i][0].y -= scaleVal;

			cornerPoints[i][1].x -= scaleVal;
			cornerPoints[i][1].y += scaleVal;

			cornerPoints[i][2].x += scaleVal;
			cornerPoints[i][2].y += scaleVal;

			cornerPoints[i][3].x += scaleVal;
			cornerPoints[i][3].y -= scaleVal;
		*/
			Point2f v[4][2];
			Point2f w[4];
			Point2f w_norm[4];

			v[0][1] = cornerPoints[i][1] - cornerPoints[i][0];
			v[0][2] = cornerPoints[i][3] - cornerPoints[i][0];

			v[1][1] = cornerPoints[i][0] - cornerPoints[i][1];
			v[1][2] = cornerPoints[i][2] - cornerPoints[i][1];

			v[2][1] = cornerPoints[i][1] - cornerPoints[i][2];
			v[2][2] = cornerPoints[i][3] - cornerPoints[i][2];

			v[3][1] = cornerPoints[i][0] - cornerPoints[i][3];
			v[3][2] = cornerPoints[i][2] - cornerPoints[i][3];

			//(The computation is componentwise for the x and y coordinates respectively).
//			In the shrunk polygon the vertex p is moved to p' along the line which halves the angle a between the vectors v1 and v2.
//			The vector w in this direction is
			for (int z = 0; z < 4; z++) {
				w[z] = v[z][1] + v[z][2];

	//			and the unit vector v in this direction is
	//			v0 = w0 / |w0|

				w_norm[z].x = w[z].x / norm(w[z]);
				w_norm[z].y = w[z].y / norm(w[z]);

	//			The new point p' is
//				int k = 15;
				cornerPoints[i][z] -= scaleVal * w_norm[z];
	//			p_x' = p_x + k * v_x
	//			p_y' = p_y + k * v_y
			}

		}

	vector<Point2f> quad_pts;
	quad_pts.push_back(Point2f(0, 0));
	quad_pts.push_back(Point2f(0, transform.rows));
	quad_pts.push_back(Point2f(transform.cols, transform.rows));
	quad_pts.push_back(Point2f(transform.cols, 0));

	String strTransform = "transform ";

	for (int i = 0; i < cornerPoints.size(); i++) {
		Mat transmtx = getPerspectiveTransform(cornerPoints[i], quad_pts);
		warpPerspective(src, transform, transmtx, transform.size());
		/*Mat src_bw;
		cvtColor(transform, src_bw, CV_BGR2GRAY);
		blur(src_bw, src_bw, Size(3, 3));
		adaptiveThreshold(src_bw, src_bw, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, 0);*/

//		blur(src_bw, src_bw, Size(3, 3));

		/*
		Mat kernel = getStructuringElement(MORPH_RECT, Size(4, 4));
//		morphologyEx(b, b, MORPH_CLOSE, kernel);

		dilate(src_bw, src_bw, kernel);
		dilate(src_bw, src_bw, kernel);
		erode(src_bw, src_bw, kernel);
		erode(src_bw, src_bw, kernel);
		erode(src_bw, src_bw, kernel);
		erode(src_bw, src_bw, kernel);*/
//		cout << "0:" << int('0') << endl;
//		cout << "strTransform" << endl;
		imshow((strTransform + char(i+48)).c_str(), transform);
	}
}

