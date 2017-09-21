#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#define ESC_KEY 27

using namespace std;
using namespace cv;


int main(void)
{
	// Load a input video
	String file_path("aaa(3).mp4");
	VideoCapture input(file_path);
	vector<vector<Point>> contours;
	if (!input.isOpened()) {
		cout << "input video error!" << endl;
		return -1;
	}


	// Load an output video
	VideoWriter output;
	Size video_size = Size((int)input.get(CAP_PROP_FRAME_WIDTH),
		(int)input.get(CAP_PROP_FRAME_HEIGHT));

	output.open("ouput.avi",
		VideoWriter::fourcc('X', 'V', 'I', 'D'),
		input.get(CAP_PROP_FPS),
		video_size,
		true);

	if (!output.isOpened()) {
		cout << "output video error!" << endl;
		return -1;
	}


	//Processing
	/// Create new windows for playing video
	namedWindow("InputVideo", CV_WINDOW_AUTOSIZE);
	namedWindow("FilteringVideo", CV_WINDOW_AUTOSIZE);
	namedWindow("OutputVideo", CV_WINDOW_AUTOSIZE);

	Mat input_frame, filter_frame, edge_frame;
	/// 'MOG2' is faster than 'KNN', But 'MOG2''s performance is less than 'KNN'
	/// 'KNN''s performance is better than 'MOG2', But 'KNN' is slower than 'MOG2'
	Ptr<BackgroundSubtractor> bg = createBackgroundSubtractorMOG2();
	/// Ptr<BackgroundSubtractor> bg = createBackgroundSubtractorKNN(100, 200.0, true); 
	///		what about changing threshold to 400.0?

	cout << "If you want stop, push 'ESC' key" << endl;
	while (true) {
		input >> input_frame;
		if (input_frame.empty()) {
			cout << "End of video." << endl;
			break;
		}

		/// Process background subtractor
		bg->apply(input_frame, filter_frame);

		/// Remove shadow
		threshold(filter_frame, filter_frame, 150, 255, THRESH_BINARY);
		
		/// Fill in the blanks due to shadow removal
		dilate(filter_frame, filter_frame, Mat(2, 2, CV_8U));
		
		/// Local Averaging : blur(spImg, localAvgImg, Size(5, 5)) 
		/// Gaussian Smoothing : GaussianBlur(filter_frame, filter_frame, Size(3, 3), 1); 
		/// MedianBlur filtering : medianBlur(filter_frame, filter_frame, 5); 

		// GaussianBlur(filter_frame, filter_frame, Size(7, 7), 1);
		medianBlur(filter_frame, filter_frame, 7);

		input_frame.copyTo(edge_frame);

		/// Find Contours
		/// Finding contours is to find white object on a black background
		/// 'CHAIN_APPROX_SIMPLE' is faster than 'CHAIN_APPROX_NONE'
		findContours(filter_frame, contours, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


		/// Draw a box
		int cmin = 20;
		int cmax = 70;

		vector<vector<Point>>::iterator itc = contours.begin();

		while (itc != contours.end()) {
			if (itc->size() < cmin || itc->size() > cmax) {
				itc = contours.erase(itc);
			}
			else {
				vector<Point> pts = *itc;
				Mat pointsMatrix = Mat(pts);
				Scalar color(0, 255, 0);

				Rect r0 = boundingRect(pointsMatrix);
				rectangle(edge_frame, r0, color, 2);

				++itc;
			}
		}
		imshow("InputVideo", input_frame);
		imshow("FilteringVideo", filter_frame);
		imshow("OutputVideo", edge_frame);

		/// If you want break, push 'ESC' key
		if (waitKey(1) == ESC_KEY) break;
	}

	return 0;
}
