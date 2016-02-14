/*
 * loaderFunctions.cpp
 *
 *  Created on: 07.01.2016
 *      Author: Standardbenutzer
 */

#include "loaderFunctions.h"

/*
 * reads the frames from the webcam and permamently checks for barcodes,
 * if the 'm' (many frames) style was selected.
 * As soon as a barcode was successfully found it returns the current frame.
 * Alternatively you can check one single frame manually by pressing enter with
 * the webcamstyle 's' (single frame) selected.
 */
Mat get_image_from_webcam(char webcamVersion, char webcamStyle) {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	get source from camera by video capture
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Mat src;
		int camIndex = 0;  // 0 is the default camera
		if (webcamVersion == 'e') {
			camIndex = -1; //get a menu to choose the camera
		}
		VideoCapture capture(camIndex);
		SettingObject s("path");

		if (webcamStyle == 'm') {
			s.setBasic(true);
		}

		if (!capture.isOpened()) {
			cout << "ERROR: Could not open Camera" << endl;
			return src; //-1 src is empty here
		}

		namedWindow("Camera", CV_WINDOW_AUTOSIZE);
		int readSuccess = 0;
		Mat frame;
		String barcode, type;
		float angle = 0;
		size_t i;
		int wKey = 0;
//		while (true) {
		while (readSuccess == 0 && wKey != 13) { //13 = ENTER

			//if esc is pressed return
			if (wKey == 27) {
				return src;
			}

//			Mat frame;
//			capture >> frame; 									// get frames

			bool bSuccess = capture.read(frame); // read a new frame from video
			capture.retrieve(frame);
			if (bSuccess)
			imshow("Camera", frame);

			/*if (waitKey(30) == 13 || readSuccess) {// press enter to break loop and copy frame into source image
				frame.copyTo(src);
				destroyAllWindows();
				break;
			}*/

			if (webcamStyle == 'm') { //many frames

				//use ZBar only
				/*readSuccess = get_barcode_string(frame, barcode, type, angle, i, s);
				if (!readSuccess) {
					Mat bw_frame;
					frame.copyTo(bw_frame);
					make_adaptiv_bw(bw_frame);
					readSuccess = get_barcode_string(bw_frame, barcode, type, angle, i, s);
	//				if (readSuccess)
	//				cout << "success bw" << endl;*/


				//calls our whole algorithm as a single methode
				readSuccess = get_Barcode(frame, s);
				if (readSuccess == -1) {
					return src;
				}
			}
			wKey = waitKey(15);
		}
		frame.copyTo(src);
		destroyAllWindows();
		return src;


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	end of capturing
		//
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
