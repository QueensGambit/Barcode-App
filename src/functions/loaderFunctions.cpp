/*
 * loaderFunctions.cpp
 *
 *  Created on: 07.01.2016
 *      Author: Standardbenutzer
 */

#include "loaderFunctions.h"


Mat get_image_from_webcam() {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	get source from camera by video capture
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Mat src;
		VideoCapture capture(0); 								// open default camera

		if (!capture.isOpened()) {
			cout << "ERROR: Could not open Camera";
			//return -1;
		}

		namedWindow("Camera", CV_WINDOW_AUTOSIZE);
		bool readSuccess = false;
		Mat frame;
//		while (true) {
		while (!readSuccess && waitKey(30) != 13) {

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
			String barcode, type;
			float angle = 0;
			int i;
			readSuccess = get_barcode_string(frame, barcode, type, angle, i);
			if (!readSuccess) {
				Mat bw_frame;
				frame.copyTo(bw_frame);
				make_adaptiv_bw(bw_frame);
				readSuccess = get_barcode_string(bw_frame, barcode, type, angle, i);
			}
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
