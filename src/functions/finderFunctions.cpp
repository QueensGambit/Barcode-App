/*
 * finderFunctions.cpp
 *
 *  Created on: 13.12.2015
 *      Author: Standardbenutzer
 */

#include "finderFunctions.h"

/*
 * !not in use!
 * Earlier attempt to sort all single lines of a barcode into one group
 * The idea was to give every hough line a specific colour to identify belonging barcode lines
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
//!not in use!
//void find_groups(Mat m, vector<ContourObject> vecCO) {
//	vector<vector<ContourObject> > group;
//	for (int i = 0; i < vecCO.size(); i++) {
//		Scalar scalar = m.at<uchar>(vecCO[i].getMassCenter());
//		if ((m.at<uchar>(vecCO[i].getMassCenter())) != 0) {
//			find_Colour_for_Groups(scalar, vecCO[i]);
//		}
//	}
//}

/*
 * !not in use!
 * It was used in association with the approach to get contours with the canny filter
 * later this procedure was replaced with the usage mser-detection
 *  */
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

	for (size_t i = 0; i < contours.size(); i++) {

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

//				int maxDst = 1;

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
	for (size_t i = 0; i < contours.size(); i++) {
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

/*
 * This function finds maximum stable areas
 * and saves them as a set of points in a vector.
 * The parameters are set up in a way to fit a the criteria of a single barcode stripe
 * as closely as possible.
 */
vector<ContourObject> find_mser(Mat gray, const SettingObject& s) {
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

	for (size_t i = 0; i < regions.size(); i++) {
		ContourObject temp(regions[i]);
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
				rng.uniform(0, 255));
		drawContours(mser, regions, i, color, 1, 8, vector<Vec4i>(), 0,
				Point());
		vecCO[i] = temp;
	}

	if (s.isShowAllSteps()) {
	namedWindow("mser", WINDOW_AUTOSIZE);
	imshow("mser", mser);
	waitAnyKey(s);
	}
	return vecCO;

}

/*
 * Calculates possible lines on the matrix where the mass-centers are printed on.
 * It return a vector of lines, which are define by 4 integer values.
 */
vector<Vec4i> get_probabilistic_hough_lines(Mat m) {
	vector<Vec4i> p_lines;

	Mat dst;

	Canny(m, dst, 50, 200, 3);

	/// 2nd Use Probabilistic Hough Transform //30	//80
	HoughLinesP(dst, p_lines, 1, CV_PI / 180, 30, 30, 60);

	return p_lines;
}

/*
 * Calculates the 4 corner points of a barcode
 * with the help of the found and filtered hough lines.
 * It calls the get_Border_Points_from_Rect() function for the ContourObject(single stripe)
 * which represents the start of a barcode
 * and the ContourObject which represents the end of a barcode.
 * At the end it returns a vector of a vector
 * in which each element contains 4 belonging corner points.
 */
vector<vector<Point2f> > get_corner_points(vector<Vec4i> lines, vector<ContourObject> vecCO, Mat mBarcodePoints, const SettingObject& s) {

	vector<Vec4i>::const_iterator it2 = lines.begin();

	ContourObject startContour;
	ContourObject endContour;

	vector<vector<Point2f> > cornerPoints;

	int startH = 0;
	int endH = 0;

	while (it2 != lines.end()) {

		Point2f start((*it2)[0], (*it2)[1]);
		Point2f end((*it2)[2], (*it2)[3]);

		//approach to use pointPolygonTest was replaced
			//startpoint inside countour?
			/*
			 * It returns positive (inside), negative (outside), or zero (on an edge) value,
			 * correspondingly. When measureDist=false , the return value is +1, -1, and 0
			 *
			 */

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

		it2++;

	}

	RNG rng(12345);

	vector<Point2f> tmpCPoints(4);

	mBarcodePoints -= .5 * mBarcodePoints;
	for(size_t p = 0; p < cornerPoints.size(); p++){
		tmpCPoints = cornerPoints[p];
		Scalar color = Scalar(rng.uniform(0, 2) * 255, rng.uniform(0, 2)* 255,rng.uniform(0, 2) * 255);

		for(int z = 0; z<4; z++){

			circle(mBarcodePoints, tmpCPoints[z], 3, color, -1, 8, 0);

		}

	}
	if (!s.isBasic()) {
	namedWindow("Barocde Punkte", WINDOW_AUTOSIZE); //WINDOW_NORMAL
	imshow("Barocde Punkte", mBarcodePoints);
	}

	return cornerPoints;

}

/*
 * Uses the ZBar Library in order to detect and receive barcode informations on a given matrix.
 * The function returns via the parameters the resulting barcode-string, barcode-type and the barcode-angle
 */
bool get_barcode_string(Mat& img, string& code, string& type, float& angle,
		size_t& number, const SettingObject& s) {

	//Scanner is used for the ZBar library
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

	Mat imgout;
	cvtColor(img, imgout, CV_RGB2GRAY);
	int width = img.cols;
	int height = img.rows;

	uchar *raw = (uchar *) imgout.data;
	// wrap image data
	Image image(width, height, "Y800", raw, width * height);
	// scan the image for barcodes
	int n = scanner.scan(image);

	//here is the result being printed on
	Mat res;

	// extract results
	for (Image::SymbolIterator symbol = image.symbol_begin();
			symbol != image.symbol_end(); ++symbol) {
		vector<Point> vp;

		// save the results in variables
		code = symbol->get_data();
		type = symbol->get_type_name();

		int n = symbol->get_location_size();

		for (int i = 0; i < n; i++) {
			vp.push_back(
					Point(symbol->get_location_x(i),
							symbol->get_location_y(i)));
		}

		RotatedRect r = minAreaRect(vp);
		Point2f pts[4];
		r.points(pts);

		img.copyTo(res);
		for (int i = 0; i < 4; i++) {
			line(res, pts[i], pts[(i + 1) % 4], Scalar(255, 0, 0), 3);
		}
//         cout<<"Angle: "<<r.angle<<endl;

		angle = r.angle;
	}

///addition

	// print results in the console
	for (Image::SymbolIterator symbol = image.symbol_begin();
			symbol != image.symbol_end(); ++symbol) {
		// do something useful with results
		cout << "decoded " << symbol->get_type_name() << " symbol \""
				<< symbol->get_data() << '"' << endl;
	}
	//////

	// clean up
	image.set_data(NULL, 0);

	if (n >= 1 && code != "" && type != "I2/5" && type != "UPC-E" && code.length() > 2) {

		string result = "Result ";

		if (!s.isBasic()) {
		draw_barcode(res, code, type, number);
		namedWindow(result + char(number + 48), 1);
		imshow(result + char(number + 48), res);
		}
		return true;
	}
	return false;
}

/*
 * Looks up the barcode-string on codecheck.info if it wasn't found in the local csv-files.
 * Here the LibCurl library is used to create a http-get-request.
 * The result containing the article name and the article description is excrated
 * via string operations on the http site-string and are saved in the parameters.
 * The function returns true if the article was successfully found, otherwise it return false.
 */
bool get_article_description_internet(const string& barcode, string& article, string& description) {

	//	const char* url = "http://www.example.com";

	//example barcode-strings
	//	string barcode = "8430631154438";
	//	string barcode = "3423621672";
	//	string barcode = "42103684";
	//	string barcode = "4311501359105";
	//	string barcode = "4018077711770";
	//	string barcode = "8430631216549";
	//	string barcode = "4018077711770";
	//	string barcode = "4250589702215";
	//	string barcode = "4014400400007";

	if (barcode.size() == 0) {
		return false;
	}

	//url of codecheck.info
	string url = "http://www.codecheck.info/product.search?q=" + barcode;
	string result = curl_httpget(url);

	if (result == "") {
		return false;
	}
	string title = "<title>";

	size_t titlefound = result.find(title);
	titlefound += title.size();
	size_t barcodefound = result.find(barcode);

	//	cout << "titlefound-Index: " << titlefound << endl;
	//	cout << "barcodefound-Index2: " << barcodefound << endl;

	size_t diff = barcodefound - titlefound - 3; //-3 because of " - "
	article = result.substr(titlefound, diff);

	//if the barcode couldn't be found in the databasa:
	if (article.find("Produkt erfassen (Schritt 1 von 2)") != -1) {
		return false;
	}
	if (article.length() > 300) {
		return false;
	}
	//delete String of "&amp" if it exists
	int ampLenght = 4;
	size_t ampPos = article.find("&amp");

	if (ampPos != -1) { //is -1 if &amp wasn't found (npos)
		string articleEnd = article.substr(ampPos+ampLenght);
		cout << "articleEnd: " << articleEnd << endl;
		article = article.substr(0, ampPos) + articleEnd;
	}

	cout << "article: " << article << endl;

	string descriptionstart = "description\" content=\'";
	string descriptionend = " - Test";
	string descriptionend2 = "✓";
	size_t descriptionfound = result.find(descriptionstart);
	descriptionfound += descriptionstart.size();
	size_t descriptionendfound = result.find(descriptionend);
	size_t descriptionendfound2 = result.find(descriptionend2);
	//	cout << "descriptionfound-Index: " << descriptionfound << endl;
	//	cout << "descriptionendfound-Index: " << descriptionendfound << endl;
	if (descriptionendfound2 < descriptionendfound) {
		descriptionendfound = descriptionendfound2;
	}
	size_t diff2 = descriptionendfound - descriptionfound;
	description = result.substr(descriptionfound, diff2);

	cout << "beschreibung: " << description << endl;

	//print the result on the console till letter 900
	/*cout << "result:" << endl;

	 int stop = 900;
	 for (int i = 0; i < stop; i++) {
	 cout << result[i];
	 }*/

	return true;
}

/*
 * This function is used in association with the LibCurl library
 * to get the http-answer string of an url.
 */
string curl_httpget(const string &url)
{
    string buffer;

    CURL *curl;
    CURLcode result;

    curl = curl_easy_init();

    if (curl)
    {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str()  );
      curl_easy_setopt(curl, CURLOPT_HEADER, 0);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

      //here its necessary to follow the http request
      //because you are automaticly redirected
      //-----------------------------------------------------
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
      //-----------------------------------------------------

      result = curl_easy_perform(curl);//http get performed

      curl_easy_cleanup(curl);//must cleanup

      //error codes: http://curl.haxx.se/libcurl/c/libcurl-errors.html
      if (result == CURLE_OK)
          return buffer;
      //curl_easy_strerror was added in libcurl 7.12.0
      cerr << "error: " << result << " " << curl_easy_strerror(result) <<endl;
      return "";
    }

    cerr << "error: could not initalize curl" << endl;
    return "";
}

/*
 * Is used in association with the LibCurl-Library
 * to write the data form the buffer.
 */
int writer(char *data, size_t size, size_t nmemb, string *buffer)
{
  int result = 0;
  if (buffer != NULL)
  {
      buffer->append(data, size * nmemb);
      result = size * nmemb;
  }
  return result;
}

/*
 * Looks up the barcode-string in the csv-file, which is specified by its file-path.
 * The result containing the article name and the article description is saved in the parameters.
 *
 * More information about reading data from a csv-file on:
 * //http://www.cplusplus.com/forum/general/17771/
 */
bool get_article_descr_csv(const char* path, string& code, string& article,
		string& descr) {
	vector<vector<string> > data;
	ifstream infile(path);

	//checks if the file is empty
	if (infile.peek() == ifstream::traits_type::eof()) {
		cout << "file is empty" << endl;
		infile.close();
		return false;
	}

	while (infile) {
		string s;

		if (!getline(infile, s)) {
			break;
		}


		istringstream ss(s);
		vector<string> record;

		while (ss) {
			string s;

			if (!getline(ss, s, '\t')) {
				break;
			}
			record.push_back(s);
		}

		data.push_back(record);
	}

	if (!infile.eof()) {
		cerr << "Haven't found csv\n";
	}
	infile.close();
	/*
	 for (int i = 0;  i < data.size(); i++) {
	 for (int z = 0; z < data[i].size(); z++) {
	 cout << "data [" << i << "][" << z << "]: " << data[i][z] << endl;
	 }
	 }
	 */
	cout << "data.size(): " << data.size() << endl;

	for (size_t i = 0; i < data.size(); i++) {
//	  cout << "data 0:" << data[i][0] << endl;
		if (data[i][0] == code) {
			cout << "successfully found in csv" << endl;

			article = data[i][1];
			descr = data[i][2];
			return true;
		}
	}

	return false;
}

/*
 * Adds a new article-description to the end of the adder.csv file.
 * Tabs are used as the delimiter.
 * The entry contains the the code, the article name and the article-description
 */
bool add_article_descr_to_csv(const char* path, string& code, string& article, string& descr) {

	ofstream out;

	if (article.length() == 0) {
		article = "-";
	}
	if (descr.length() == 0) {
		descr = "-";
	}
	// std::ios::app is the open mode "append" meaning
	// new data will be written tostr the end of the file.
	out.open(path, ios::app);

	cout << "code: " << code << endl;
	cout << "article: " << article << endl;
	out << code << "\t" << article << "\t" << descr << endl;

	out.close();
	cout << "succefully added to database" << endl;

	return true;
}

