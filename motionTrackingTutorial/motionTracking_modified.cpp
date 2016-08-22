//motionTracking.cpp

//Written by  Kyle Hounslow, January 2014
//modified by Brian Gravelle to track multiple objects - August 2016

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.

#include <opencv/cv.hpp>
//#include <opencv/highgui.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

//our sensitivity value to be used in the threshold() function
static int SENSITIVITY_VALUE = 20; // original 20
//size of blur used to smooth the image to remove possible noise and
//increase the size of the object we are trying to track. (Much like dilate and erode)
static int BLUR_SIZE = 10; // original 10
static double MIN_OBJ_AREA = 10;
//we'll have just one object to search for
//and keep track of its position.
int theObject[2] = {0,0};
//Rect objectBoundingRectangle = Rect(0,0,0,0);

void searchForMovement(Mat thresholdImage, Mat &cameraFeed);
int char_to_int(char* c);
string intToString(int number);
void show_help();


int main(int argc, char** argv){

	//some boolean variables for added functionality
	bool objectDetected = false;
	//these two can be toggled by pressing 'd' or 't'
	bool debugMode = false;
	bool trackingEnabled = false;
	//pause and resume code
	bool pause = false;
	//set up the matrices that we will need
	//the two frames we will be comparing
	Mat frame1,frame2;
	//their grayscale images (needed for absdiff() function)
	Mat grayImage1,grayImage2;
	//resulting difference image
	Mat differenceImage;
	//thresholded difference image (for use in findContours() function)
	Mat thresholdImage;
	//video capture object.
	VideoCapture capture;
	//if frame reads work or fail
	bool success;


  if(argc < 2) 
	  show_help();

	string vid_name = argv[1];
	
	if(argc > 2) 
		SENSITIVITY_VALUE = char_to_int(argv[2]);
	
	if(argc > 3) 
		BLUR_SIZE = char_to_int(argv[3]);


	while(1){

		//we can loop the video by re-opening the capture every time the video reaches its last frame
		capture.open(vid_name);
		if(!capture.isOpened()){
			cout<<"ERROR ACQUIRING VIDEO FEED\n";
			getchar();
			return -1;
		}

		//read first frame
		success = capture.read(frame1);
		if(!success){
			cout << endl << "ERROR: frame 1 failed to be read" << endl;
			exit(1);
		}
		//convert frame1 to gray scale for frame differencing
		cvtColor(frame1, grayImage1, COLOR_BGR2GRAY);
		//copy second frame
		success = capture.read(frame2);
		if(!success){
			cout << endl << "ERROR: frame 2 failed to be read" << endl;
			exit(1);
		}

		//while( !(frame2.rows == 0 || frame2.cols ==0) ) {
		while( success ) {

			//convert frame2 to gray scale for frame differencing
			cvtColor(frame2, grayImage2, COLOR_BGR2GRAY);

			//perform frame differencing with the sequential images. This will output an "intensity image"
			//do not confuse this with a threshold image, we will need to perform thresholding afterwards.
			absdiff(grayImage1, grayImage2, differenceImage);
			//threshold intensity image at a given sensitivity value
		  threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);
			if(debugMode==true){
				//show the difference image and threshold image
				imshow("Difference Image", differenceImage);
				imshow("Threshold Image", thresholdImage);
			}else{
				//if not in debug mode, destroy the windows so we don't see them anymore
				destroyWindow("Difference Image");
				destroyWindow("Threshold Image");
			}
			//use blur() to smooth the image, remove possible noise and
			//increase the size of the object we are trying to track. (Much like dilate and erode)
		  blur(thresholdImage, thresholdImage, Size(BLUR_SIZE,BLUR_SIZE));
			
			//threshold again to obtain binary image from blur output
		  threshold(thresholdImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);

			if(debugMode==true){
				//show the threshold image after it's been "blurred"
				imshow("Final Threshold Image", thresholdImage);
			}
			else {
				//if not in debug mode, destroy the windows so we don't see them anymore
				destroyWindow("Final Threshold Image");
			}

			//if tracking enabled, search for contours in our thresholded image
			if(trackingEnabled) {
				searchForMovement(thresholdImage, frame1);
			}

			//show our captured frame
			imshow("Frame1",frame1);
			//check to see if a button has been pressed.
			//this 10ms delay is necessary for proper operation of this program
			//if removed, frames will not have enough time to referesh and a blank 
			//image will appear.
			switch(waitKey(10)){
			case 1048603:
			// case 27: //'esc' key has been pressed, exit program.
				return 0;
			case 1048692:
			// case 116: //'t' has been pressed. this will toggle tracking
				trackingEnabled = !trackingEnabled;
				if(trackingEnabled == false) cout<<"Tracking disabled."<<endl;
				else cout<<"Tracking enabled."<<endl;
				break;
			case 1048676:
			// case 100: //'d' has been pressed. this will debug mode
				debugMode = !debugMode;
				if(debugMode == false) cout<<"Debug mode disabled."<<endl;
				else cout<<"Debug mode enabled."<<endl;
				break;
			case 1048688:
			// case 112: //'p' has been pressed. this will pause/resume the code.
				pause = !pause;
				if(pause == true){ cout<<"Code paused, press 'p' again to resume"<<endl;
					while (pause == true){
						//stay in this loop until 
						switch (waitKey()){
							//a switch statement inside a switch statement? Mind blown.
						case 1048688:
						// case 112: 
							//change pause back to false
							pause = false;
							cout<<"Code resumed."<<endl;
							break;
						}
					}
				}
			} //big switch statement

			frame2.copyTo(frame1);
			//convert frame1 to gray scale for frame differencing
			cvtColor(frame1, grayImage1, COLOR_BGR2GRAY);
			//copy second frame
			success = capture.read(frame2);
		} // inner while loop
		
		//release the capture before re-opening and looping again.
		capture.release();
	} // outer while loop (infinite)

	return 0;

}

void searchForMovement(Mat thresholdImage, Mat &cameraFeed){

	bool objectDetected=false;
	int obj_count = 0, i = 0;
	double obj_area = 0;
	Mat temp;
	vector<Rect2d> obj_rects;
	thresholdImage.copyTo(temp);
	
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	// retrieves external contours
	findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );

	if(contours.size() > 0){
		i = contours.size()-1;
		do {
			obj_rects.push_back( boundingRect(contours.at(i)) );
			obj_area = obj_rects.end()->area();

			//if(obj_area >= MIN_OBJ_AREA)
				obj_count++;

			i--;
		} while(i >= 0);
		// } while( (i >= 0) && (obj_area >= MIN_OBJ_AREA) );
		// cout << endl << "obj count: " << obj_count << endl;
		// cout << endl << "obj area:  " << obj_area << endl;
	}

	for(unsigned i = 0; i < obj_rects.size(); i++) {
	  rectangle( cameraFeed, obj_rects[i], Scalar( 255, 0, 0 ), 2, 1 ); // draw rectangle around object
	  int mid_x = obj_rects[i].x + (obj_rects[i].width / 2);
	  int mid_y = obj_rects[i].y - (obj_rects[i].height / 2);
	}

	// //make a bounding rectangle around the largest contour then find its centroid
	// //this will be the object's final estimated position.
	// //make some temp x and y variables so we dont have to type out so much
	// int xpos = obj_rects.at(0).x + obj_rects.at(0).width/2;  //for label i think
	// int ypos = obj_rects.at(0).y + obj_rects.at(0).height/2; //for label i think
	// theObject[0] = xpos , theObject[1] = ypos;
	// int x = theObject[0];
	// int y = theObject[1];
	// //draw some crosshairs on the object
	// circle(cameraFeed,Point(x,y),20,Scalar(0,255,0),2);
	// line(cameraFeed,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
	// line(cameraFeed,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
	// line(cameraFeed,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
	// line(cameraFeed,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
	// putText(cameraFeed,"Tracking object at (" + intToString(x)+","+intToString(y)+")",Point(x,y),1,1,Scalar(255,0,0),2);



}

int char_to_int(char* c) {
	int i = 0; 
	int ret = 0;
	int mult = 1;

	while(c[i] !='\0')
		i++;
	i--;

	for (i; i >= 0; i--) {
		ret += mult * ((int)c[i] - (int)48);
		mult *= 10;
	}
	return ret;
}

//int to string helper function
string intToString(int number){
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void show_help() {
  cout << endl << 
  " Usage: ./motionTracking_modified.out <video_name> [SENSITIVITY_VALUE] [BLUR_SIZE]\n"
  " examples:\n"
  " ./motionTracking_modified.out /home/pi/videos/my_vid.h264\n"
  " ./motionTracking_modified.out /home/pi/videos/my_vid.h264 20 10\n"
  << endl << endl;
  exit(1); 
}
