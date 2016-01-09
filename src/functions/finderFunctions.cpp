/*
 * finderFunctions.cpp
 *
 *  Created on: 13.12.2015
 *      Author: Standardbenutzer
 */

#include "finderFunctions.h"

/*
 * Lines of Barcodes have to be grouped now
 */

//void find_Colour_for_Groups(map<Scalar, vector< vector< ContourObject> > >& scalarMap,Scalar scalar, ContourObject vecCO) {
//
//	map<Scalar, vector<vector<ContourObject> > >::iterator it;
//	vector<ContourObject> temp;
//
//	for (it = scalarMap.begin(); it != scalarMap.end(); it++) {
//		if (it == scalar) {
//			//get group of line
//			temp = scalarMap[scalar];
//			// extend the group with the contour
//			temp.push_back(vecCO);
//			//push it back
//			scalarMap[scalar].push_back(temp);
//		} else {
//			vector<vector<ContourObject> > group;
//			scalarMap.insert(pair<Scalar, vector<ContourObject>> (scalar, group));
//		}
//	}
////	if (scalarMap.find(scalar)) {
////		scalarMap[scalar].push_back(vecCO);
////	} else {
////		vector<ContourObject> group;
////		scalarMap.insert(pair<Scalar, vector<ContourObject>> (scalar, group));
////
////	}
//}
//void find_groups(Mat m, vector<ContourObject> vecCO) {
//	vector<vector<ContourObject> > group;
//	for (int i = 0; i < vecCO.size(); i++) {
//		Scalar scalar = m.at<uchar>(vecCO[i].getMassCenter());
//		if ((m.at<uchar>(vecCO[i].getMassCenter())) != 0) {
//			find_Colour_for_Groups(scalar, vecCO[i]);
//		}
//	}
//}


vector<ContourObject> find_moments(Mat gray, int thresh, Mat skel, Size size) {

	RNG rng(12345);
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using canny
	//						  50
	Canny(gray, canny_output, 50, 150, 3);

	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE,
			CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	cout << "contours.size()" << contours.size() << endl;

	// Get the Area of the contours
	vector<ContourObject> vecCO;

	/// Get the moments
	vector<Moments> mu(contours.size());
	///  Get the mass centers:
	vector<Point2f> mc(mu.size());

	for (int i = 0; i < contours.size(); i++) {

		if (contourArea(contours[i]) > thresh) {

			mu[i] = moments(contours[i], false);
			//calc the mass centers
			float x = mu[i].m10 / mu[i].m00;
			float y = mu[i].m01 / mu[i].m00;
			//x != x return true, if x is NaN
			if (!(x != x) && !(y != y)) {
				Point2f tempMassCenter = Point2f(mu[i].m10 / mu[i].m00,
						mu[i].m01 / mu[i].m00);
				//mc.push_back(Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ));

				int maxDst = 1;

				Matx33f mask(0, 255, 0, 255, 255, 0, 0, 255, 0);

				Matx33f maskResult;

				if (wPxl_in_Area(tempMassCenter, skel, 2)) {
					ContourObject temp(tempMassCenter, contours[i]);
					vecCO.push_back(temp);

				}
			}
		}
	}

	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++) {
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));

		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 4, color, -1, 8, 0);

	}

	resize(canny_output, canny_output, size);

	/// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	namedWindow("Canny", CV_WINDOW_AUTOSIZE);
	imshow("Canny", canny_output);

	/// Calculate the area with the moments 00 and compare with the result of the OpenCV function
	//printf("\t Info: Area and Contour Length \n");
	//for( int i = 0; i< contours.size(); i++ )
	//printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(vecCO[i].getContour()), arcLength( vecCO[i].getContour(), true ) );
	return vecCO;
}

vector<ContourObject> find_mser(Mat gray) {
	RNG rng(12345);
	MSER ms;

	ms.set("delta", 3); //default 5
	ms.set("minArea", 30); //default 60
	ms.set("maxArea", 3500); //default 14400
	ms.set("minMargin", 0.001); //.003
	ms.set("edgeBlurSize", 1); //5
	ms.set("minDiversity", .9); //0.2
	ms.set("maxVariation", .3); //0.25g

	vector<vector<Point> > regions;

	ms(gray, regions, Mat());
	vector<ContourObject> vecCO(regions.size());

	Mat mser = Mat::zeros(gray.size(), CV_8UC3);

	for (int i = 0; i < regions.size(); i++) {
		ContourObject temp(regions[i]);
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		drawContours(mser, regions, i, color, 1, 8, vector<Vec4i>(), 0,
				Point());
		vecCO[i] = temp;
	}

	namedWindow("mser", WINDOW_AUTOSIZE);
	imshow("mser", mser);

	return vecCO;

}

vector<Vec4i> get_probabilistic_hough_lines(Mat m) {
	vector<Vec4i> p_lines;

	Mat dst;
//	blur(m, m, Size(3, 3));
//	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
//	dilate( m, m, element );
	 Canny(m, dst, 50, 200, 3);

//	cvtColor(m, gray, COLOR_BGR2GRAY);

//	Mat bw = m > 128;

	/// 2. Use Probabilistic Hough Transform //30	//80
	HoughLinesP(dst, p_lines, 1, CV_PI / 180, 30, 30, 60);

	/// Show the result
	cout << "p_line.size(): " <<  p_lines.size() << endl;
//	filter_hough_lines(p_lines, 0.00002, m.cols*m.rows);
//	filter_hough_lines2(p_lines);





	return p_lines;
}

vector<vector<Point2f> > get_corner_points(vector<Vec4i> lines, vector<ContourObject> vecCO, Mat mBarcodePoints) {

//	cout << "size of vecCO in filter_detected: " << vecCO.size() << endl;

	vector<Vec4i>::const_iterator it2 = lines.begin();

	ContourObject startContour;
	ContourObject endContour;

	vector<vector<Point2f> > cornerPoints;

	int startH = 0;
	int endH = 0;

	while (it2 != lines.end()) {

		Point2f start((*it2)[0], (*it2)[1]);
		Point2f end((*it2)[2], (*it2)[3]);
//		cout << "startpoint: " << start << endl;
//		cout << "endpoint: " << end << endl;
//		cout << "entree" << endl;

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

//				cout << "Angle: " << startContour.getAngle() << endl;

				cornerPoints.push_back(tmpCPoints);

			}
//			}

				//break;
//			}


		it2++;

	}
//	Size s = Size(324, 244);
//	Size s = Size(1138, 1600);

//	Mat mBarcodePoints =  Mat::zeros(size, CV_8UC3);
	RNG rng(12345);

//	cout << "cornerPoints:" << cornerPoints.size() << endl;
	vector<Point2f> tmpCPoints(4);

	mBarcodePoints -= .5 * mBarcodePoints;
	for(int p = 0; p < cornerPoints.size(); p++){
		tmpCPoints = cornerPoints[p];
		Scalar color = Scalar(rng.uniform(0, 2) * 255, rng.uniform(0, 2)* 255,rng.uniform(0, 2) * 255);
//		Scalar color = Scalar(255, 255, 0);
//	    Point intRect_points[4];
		for(int z = 0; z<4; z++){
//			cout << "Punkt" << z << " " << tmpCPoints[z] << endl;

			circle(mBarcodePoints, tmpCPoints[z], 3, color, -1, 8, 0);
//			intRect_points[z] = tmpCPoints[z];

		}
//		fillConvexPoly( mBarcodePoints, intRect_points, 4, Scalar(255,255,255), 8, 0);
	}

	namedWindow("Barocde Punkte", 1);
	imshow("Barocde Punkte", mBarcodePoints);

//	cout << "hits start: " << startH << endl;
//	cout << "hits end: " << endH << endl;

	return cornerPoints;

}

bool get_barcode_string(Mat& img, string& code, string& type, float& angle) {

    ImageScanner scanner;
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
     // obtain image data
    //char file[256];
    //cin>>file;
    //Mat img = imread(file,0);
//      Mat img = imread("D:/Gimp/DBV/media/gut/mandarine_scaled.jpg", CV_LOAD_IMAGE_COLOR);
//       Mat img = imread("D:/Gimp/DBV/media/gut/auschnitt_himbeere.jpg", CV_LOAD_IMAGE_COLOR);
//       Mat img = imread("D:/Gimp/DBV/media/gut/auschnitt_joghurt2.jpg", CV_LOAD_IMAGE_COLOR);
//       Mat img = imread("D:/Gimp/DBV/barcode_schnitt2_mirrored.png", CV_LOAD_IMAGE_COLOR);
//     Mat img = imread("D:/Gimp/DBV/barcode_schnitt4.jpg", CV_LOAD_IMAGE_COLOR);
//      Mat img = imread("D:/Gimp/DBV/media/gut/mandarine.jpg", CV_LOAD_IMAGE_COLOR);
    //Mat img = imread("D:/Program Files (x86)/ZBar/examples/barcode.png", CV_LOAD_IMAGE_COLOR);



    //Mat img = imread("D:/Gimp/DBV/Lenna.png", CV_LOAD_IMAGE_COLOR);

    //cvtColor(img,imgout,CV_GRAY2RGB);

    Mat imgout;
    cvtColor(img,imgout,CV_RGB2GRAY);
    int width = img.cols;
    int height = img.rows;
//    cout << "cols: " << width << endl;
//    cout << "height: " << height << endl;

    uchar *raw = (uchar *)imgout.data;
 // wrap image data
 Image image(width, height, "Y800", raw, width * height);
 // scan the image for barcodes
int n = scanner.scan(image);
 // extract results
 for(Image::SymbolIterator symbol = image.symbol_begin();
   symbol != image.symbol_end();
   ++symbol)
 {
              vector<Point> vp;
   // do something useful with results
   /*cout << "decoded " << symbol->get_type_name()
      << " symbol \"" << symbol->get_data() << '"' <<" "<< endl;*/

   code = symbol->get_data();
   type = symbol->get_type_name();

         int n = symbol->get_location_size();

         for(int i=0;i<n;i++){
              vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
         }

         RotatedRect r = minAreaRect(vp);
         Point2f pts[4];
         r.points(pts);

         for(int i=0;i<4;i++){
              line(img,pts[i],pts[(i+1)%4],Scalar(255,0,0),3);
         }
//         cout<<"Angle: "<<r.angle<<endl;

         angle = r.angle;
 }

///addition
/*
     // extract results
  for(Image::SymbolIterator symbol = image.symbol_begin();
      symbol != image.symbol_end();
      ++symbol) {
  	cout << "Test" << endl;
      // do something useful with results
      cout << "decoded " << symbol->get_type_name()
           << " symbol \"" << symbol->get_data() << '"' << endl;
  }
 //////*/

//cout << "The Barcode is: " << n << endl;
   // imshow("imgout.jpg",imgout);
    namedWindow("MyWindow", CV_WINDOW_AUTOSIZE);
    imshow("MyWindow", img);
    namedWindow("Result",1);
    imshow("Result", imgout);
 // clean up
 image.set_data(NULL, 0);

	 if (n == 1) {
		return true;
	 }
	 return false;
}
