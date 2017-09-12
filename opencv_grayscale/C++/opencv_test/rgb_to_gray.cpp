
#include <iostream>
#include <opencv2/opencv.hpp>

#include <stdlib.h>
#include <stdio.h>

#define ESC_KEY 27

using namespace std;
using namespace cv;

void ConvertGray(Mat & _input, Mat & _output);


int main(void)
{
	// Load a input video
	String file_path("test2.mp4");
	VideoCapture input(file_path);

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
	namedWindow("OutputVideo", CV_WINDOW_AUTOSIZE);

	Mat input_frame;
	Mat output_frame(video_size, CV_8UC1);

	cout << "If you want stop, push 'ESC' key" << endl;
	for (;;) {
		input >> input_frame;

		/// The function below does the same thing
		/// In addition, it's faster than my code...
		/// cvtColor(input_frame, output_frame, COLOR_BGR2GRAY);
		ConvertGray(input_frame, output_frame);
		output << output_frame;

		imshow("InputVideo", input_frame);
		imshow("OutputVideo", output_frame);

		/// If you want break, push 'ESC' key
		if (waitKey(1) == ESC_KEY) break;
	}

	return 0;
}


void ConvertGray(Mat & _input, Mat & _output) {
	uint rows = _input.rows;
	uint cols = _input.cols;
	uint channel = _input.channels();

	// Convert to gray
	uchar rgb_to_gray;
	uchar img_blue, img_green, img_red;

	for (uint j = 0; j < rows; j++) {
		for (uint i = 0; i < cols; i++) {
			img_blue = _input.data[channel * (j * cols + i)];
			img_green = _input.data[channel * (j * cols + i) + 1];
			img_red = _input.data[channel * (j * cols + i) + 2];
			
			rgb_to_gray = (uchar)((img_blue + img_green + img_red) / 3);

			*(_output.ptr<uchar>(j, i)) = rgb_to_gray;
		}
	}
}