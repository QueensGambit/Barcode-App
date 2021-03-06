/*
 * transformerFunctions.cpp
 *
 *  Created on: 09.01.2016
 *      Author: Anonym
 */

#include "transformerFunctions.h"

/*
 * perspectively transforms a given area on a matrix
 * which is defined by 4 points into a ordinary rectangle.
 * First the corner points are sorted, then the area is scaled by a percentage
 * and then the area is transformed with the warpPerspective() method.
 */
vector<Mat> p_transform(Mat src, vector<vector<Point2f> > cornerPoints, const SettingObject& s) {

	vector<Mat> mBarcode;

	String strTransform = "transform ";


	///sort corners to correct order
	for (int i = 0; i < cornerPoints.size(); i++) {

		Point2f tmpP[4];
		if (cornerPoints[i][0].y > cornerPoints[i][1].y && cornerPoints[i][0].x > cornerPoints[i][2].x) {
			tmpP[0] = cornerPoints[i][2];
			tmpP[1] = cornerPoints[i][3];
			tmpP[2] = cornerPoints[i][0];
			tmpP[3] = cornerPoints[i][1];

			cornerPoints[i][0] = tmpP[0];
			cornerPoints[i][1] = tmpP[1];
			cornerPoints[i][2] = tmpP[2];
			cornerPoints[i][3] = tmpP[3];
		}
		//is the [1] the actual first point?
		else if (cornerPoints[i][0].y > cornerPoints[i][1].y) {
		Point2f tmpP[4];

		tmpP[0] = cornerPoints[i][1]; //1
		tmpP[1] = cornerPoints[i][0]; //0
		tmpP[2] = cornerPoints[i][2]; //2
		tmpP[3] = cornerPoints[i][3]; //3

		cornerPoints[i][0] = tmpP[0];
		cornerPoints[i][1] = tmpP[1];
		cornerPoints[i][2] = tmpP[2];
		cornerPoints[i][3] = tmpP[3];
		}

		/*for (int z = 0; z < 4; z++) {
			cout << "cornerPoints[" << z << "]:" << cornerPoints[i][z] << endl;
		}*/
	}

		///zoom out
		for (int i = 0; i < cornerPoints.size(); i++) {

			//a simple method to add a constant factor doesn't work all the time
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

			float width1 = max(norm(v[0][1]), norm(v[0][2]));
			float height1 = min(norm(v[0][1]), norm(v[0][2]));
			float width2= max(norm(v[2][1]), norm(v[2][2]));
			float height2 = min(norm(v[2][1]), norm(v[2][2]));

			float width = min(width1, width2);
			float height = max(height1, height2);

			//2 Variants for each Barcode
			//1 is Original Size, the other is half the size
//		if (min(height, width) > 25 && max(height, width) / min(height, width) < 10) {
		Mat transform;
		float newWidth = width * ((300 / height));
		if (newWidth > 1400) {
			newWidth = 1400;
		}
		if (width / height < 4) {

			transform = Mat::zeros(300, newWidth, CV_8UC3);
		}
		else {
			transform = Mat::zeros(300, newWidth * 0.5, CV_8UC3);
		}

//		int scaleVal = 20; //was the default value for scaling
		int scaleVal = width * height * 0.003;

		//see more on:
		//http://stackoverflow.com/questions/7995547/enlarge-and-restrict-a-quadrilateral-polygon-in-opencv-2-3-with-c
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


		vector<Point2f> quad_pts;
		quad_pts.push_back(Point2f(0, 0));
		quad_pts.push_back(Point2f(0, transform.rows));
		quad_pts.push_back(Point2f(transform.cols, transform.rows));
		quad_pts.push_back(Point2f(transform.cols, 0));

			Mat transmtx = getPerspectiveTransform(cornerPoints[i], quad_pts);
			warpPerspective(src, transform, transmtx, transform.size());
			Mat src_bw;
			cvtColor(transform, transform, CV_BGR2GRAY);

			transform.copyTo(src_bw);

			//blur is useful depending on the matrix size
			int blurFac = newWidth / 200;
			if (blurFac == 0) {
			blurFac = 1;
			}
			blur(src_bw, src_bw, Size(blurFac, blurFac)); //def: 5, 5*/							//15
			adaptiveThreshold(src_bw, src_bw, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, 0);
			~transform += .6 * ~src_bw;
//			transform = ~transform;
			cvtColor(src_bw, src_bw, CV_GRAY2BGR);
			cvtColor(transform, transform, CV_GRAY2BGR);
	//		blur(src_bw, src_bw, Size(3, 3));


			if (s.isShowAllSteps()) {
			imshow(strTransform + char(i+48), src_bw); //+ "." + char(u+48)).c_str()
			imshow(strTransform + char(i+48) + ".2", transform);
			}

			mBarcode.push_back(src_bw);
			mBarcode.push_back(transform);
	}

	return mBarcode;

}

/*
 * calls the command_line exe of espeaks
 * to read the article name and the article descripion
 */
bool speak_article_descr(const string& article, const string& descr) {

	//the exe has the following paramaters to set up
	// -p: Pitch default: 50
	// -s: words per Minut: def: 175 (80 - 450)
	// -g: Word gap. Pause between words: def 10ms of def speed
	// -m: Enables SSML for adding a pause/break time
																									//30	//135 //4
	string speak = string(".\\src\\eSpeak\\command_line\\espeak.exe --path=\".\\src\\eSpeak\" -v de -p 30 -s 145 -g 3 -m \"") + string("artikel: ")
			+ article + string("<break time = \'1000\'/> beschreibung: ") + descr + string("\"");
//	waitKey(100);

	//the system() method is an easier way to start an exe
	//but this way it doesn't run parallel
//	system(speak.c_str());

	start_executable(speak);
}
