//
//  main.cpp
//  Final_Project
//
//  Created by DeoBM on 27/11/2018.
//  Copyright © 2018 DeoBM. All rights reserved.
// 21500362 송진범
// 21700034 곽영혜
// 21700398 신유진


#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


Scalar distinguish(int radius);
void calculatePrice(Mat src, vector<Vec3f> circles);



int main()
{

	char fileName[20];

	cout << "Enter File Name : " << endl;

	cin >> fileName;
	VideoCapture capture;

	if (strcmp(fileName, "1.mp4") == 0) { VideoCapture capture1("1.mp4"); capture = capture1; }
	else if (strcmp(fileName, "2.mp4") == 0) { VideoCapture capture2("2.mp4"); capture = capture2; }
	else if (strcmp(fileName, "3.mp4") == 0) { VideoCapture capture3("3.mp4"); capture = capture3; }
	else if (strcmp(fileName, "4.mp4") == 0) { VideoCapture capture4("4.mp4"); capture = capture4; }
	else if (strcmp(fileName, "5.mp4") == 0) { VideoCapture capture5("5.mp4"); capture = capture5; }
	else if (strcmp(fileName, "test.mp4") == 0) { VideoCapture capture7("test.mp4"); capture = capture7; }
	else { VideoCapture capture6("6.mp4"); capture = capture6; }


	Mat src, pre_src, diff;
	capture >> pre_src;
	int no_move_cnt = 0;
	while (1) {
		capture >> src;
		if (src.empty())
			break;

		absdiff(pre_src, src, diff);
		cvtColor(diff, diff, COLOR_BGR2GRAY);
		threshold(diff, diff, 10, 255, THRESH_BINARY);
		erode(diff, diff, Mat(), Point(-1, -1), 2);
		resize(diff, diff, Size(), 0.5, 0.5);
		imshow("diff", diff);
		pre_src = src.clone();
		float mov_num = countNonZero(diff);// / (float)tot_area;
		cout << "NUM= " << mov_num << endl;

		imshow("src", src);

		if (mov_num >= 50) {
			no_move_cnt = 0;
			waitKey(2);
			continue;
		}

		no_move_cnt++;
		if (no_move_cnt < 10) {
			waitKey(2);
			continue;
		}

		GaussianBlur(src, src, Size(3, 3), 10.0);
		//edge extraction
		Mat canny;
		Canny(src, canny, 20, 60);
		dilate(canny, canny, Mat(), Point(-1, -1), 2);
		Mat cloned = canny.clone();
		//find contours
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		findContours(cloned, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

		//        resize(canny, canny, Size(), 0.5, 0.5);
		//        imshow("Canny", canny);
		//
		vector<vector<Point>> good_shapes;
		int num_good = 0;
		bool find_hand = false;
		Mat drawing = Mat::zeros(cloned.size(), CV_8UC3);
		for (size_t i = 0; i < contours.size(); i++) {
			//Mat result = Mat::zeros(cloned.size(), CV_8UC3);
			int area = contourArea(contours[i]);
			if (area < 1000)
				continue;
			if (hierarchy[i][3] < 0) // if top level
			{
				Rect top_bb = boundingRect(contours[i]);
				Point tl_bb = top_bb.tl();
				int area_bb = top_bb.area();
				rectangle(drawing, top_bb, Scalar(255, 0, 255));
				//area -> area_bb change!!
				if (area_bb > 2000 && tl_bb.y <= 50)
				{
					find_hand = true;

					break;
				}
			}

			// No motion and No hands --> There are Only Puzzles



			if (hierarchy[i][2] > 0) // if it is not a leaf
				continue;

			vector<Point> app_poly;
			approxPolyDP(contours[i], app_poly, 20, true);
			good_shapes.push_back(app_poly);
			//good_shapes.push_back(contours[i]);
			drawContours(drawing, good_shapes, num_good++, Scalar(255, 255, 255), 1);
			//imshow(to_string(i), result);
		}
		// End of the for loop

		if (find_hand) {
			//            resize(drawing, drawing, Size(), 0.5, 0.5);
			//            imshow("drawing", drawing);
			imshow("src", src);
			waitKey(100);
			continue;
		}

		// 동전 인식
		Mat gray;
		cvtColor(src, gray, COLOR_BGR2GRAY);
		medianBlur(gray, gray, 5);
		vector<Vec3f> circles;
		HoughCircles(gray, circles, HOUGH_GRADIENT, 1,
			gray.rows / 16,  // change this value to detect circles with different distances to each other
			100, 30, 20, 50 // change the last two parameters
			// (min_radius & max_radius) to detect larger circles
		);
		for (size_t i = 0; i < circles.size(); i++)
		{
			Vec3i c = circles[i];
			Point center = Point(c[0], c[1]);
			// circle center
			circle(src, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
			putText(src, to_string(c[0]) + ", " + to_string(c[1]), Point(c[0], c[1]), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 0), 2);
			// circle outline
			int radius = c[2];
			circle(src, center, radius, distinguish(pow(radius, 2)), 3, LINE_AA);
			calculatePrice(src, circles);
			//            putText(src, to_string(i), Point(10, 10), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 255, 0), 2);
		}
		imshow("src", src);

		//        resize(drawing, drawing, Size(), 0.5, 0.5);
		//        imshow("drawing", drawing);


		int ky = waitKey(3);
		if (ky == 27)
			break;
		else if (ky == ' ')
			waitKey(0);
	}
}



Scalar distinguish(int powRadius) {
	if (powRadius < 730) return Scalar(255, 0, 0);
	else if (powRadius > 900 && powRadius < 1250) return Scalar(0, 255, 0);
	else if (powRadius > 1270 && powRadius < 1700) return Scalar(0, 0, 255);

	return Scalar(255, 255, 255);
}


void calculatePrice(Mat src, vector<Vec3f> circles) {
	int ten = 0;
	int hundred = 0;
	int fiveHundred = 0;

	for (int i = 0; i < circles.size(); i++) {
		Vec3i c = circles[i];

		if (pow(c[2], 2) < 730) ten++;
		else if (pow(c[2], 2) > 900 && pow(c[2], 2) < 1245) hundred++;
		else if (pow(c[2], 2) > 1270 && pow(c[2], 2) < 1700) fiveHundred++;
	}


	putText(src, "10 Coin: " + to_string(ten), Point(10, 30), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 0, 0), 2);
	putText(src, "100 Coin: " + to_string(hundred), Point(10, 70), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 0, 0), 2);
	putText(src, "500 Coin: " + to_string(fiveHundred), Point(10, 110), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 0, 0), 2);
	putText(src, "Total Price: " + to_string(ten * 10 + hundred * 100 + fiveHundred * 500), Point(10, 150), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 0, 0), 2);
}
