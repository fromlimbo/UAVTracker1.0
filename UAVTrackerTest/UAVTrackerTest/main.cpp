#include <iostream>
#include <sstream>
#include<time.h>
#include<iomanip>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "utils.h"
#include "UAVTracker.h"


using namespace cv;
using namespace std;
//Global variables
Rect box;
bool drawing_box = false;
bool gotBB = false;
bool tl = true;
bool tk = true;	//卡尔曼开关
bool rep = false;
bool fromfile=false;
string video;

int main(int argc, char * argv[]){//int argc, char * argv[]     "-p" "parameters.yml" "-s" 0 "-r"
  VideoCapture capture;
 //capture.open(0);
  FileStorage fs;
  ifstream fv;
  fs.open("parameters.yml", FileStorage::READ);
  fv.open("database.txt", ios::in);
  if (!fv.is_open())
  {
	  cout << "打开文件错误！" << endl; exit(1);
  }
  string line;
  vector<vector<string>> storage;
  vector<int>time;
  vector<float>trackaccuracy;
  while (getline(fv, line))
  {
	  stringstream ss(line);
	  string str;
	  vector<string> st;
	  while (getline(ss, str,','))
	  {
		  st.push_back(str);
	  }
	  storage.push_back(st);
  }
  for (int i = 0; i < storage.size();i++)
  {
	  
	  string st1 = storage[i][0];
	  capture.open(storage[i][0]);

	  if (!capture.isOpened())
	  {
		  cout << "capture device failed to open!" << endl;
		  return 1;
	  }
	  fromfile = true;
	  box.x = atoi(storage[i][1].c_str());
	  box.y = atoi(storage[i][2].c_str());
	  box.width = atoi(storage[i][3].c_str());
	  box.height = atoi(storage[i][4].c_str());
	  gotBB = true;

	  TLD tld;
	  tld.read(fs.getFirstTopLevelNode());
	  Mat frame;
	  Mat last_gray;
	  Mat first;
	  if (fromfile){
		  capture >> frame;
		  cvtColor(frame, last_gray, CV_RGB2GRAY);
		  frame.copyTo(first);
	  }
	  else{
		  capture.set(CV_CAP_PROP_FRAME_WIDTH, 340);
		  capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	  }

	  ///Initialization
	  //GETBOUNDINGBOX:
	  FILE  *bb_file = fopen("missingframe.txt", "w");
	  tld.init(first, box, bb_file);

	  //Run
	  Mat current_gray;
	  BoundingBox pbox;

	  bool status = true;//status lastboxfound
	  int frames = 1;
	  int trackedframe = 1;
	  int t = 0;
	  Mat current;
	  while (capture.read(frame))
	  {

		 
		  clock_t t1 = clock();
		  //get frame
		  cvtColor(frame, current_gray, CV_RGB2GRAY);
		  //Process Frame
		  clock_t t3 = clock();
		  tld.processFrame(frame, last_gray, current_gray, pbox, status, tl, tk, bb_file);
		  clock_t t4 = clock();
		  //Draw Points
		  if (status)
		  {

			  frame.copyTo(current);
			  drawBox(frame, pbox);
			 
		  }
		  //Display
		  imshow("Output", frame);
		  //swap points and images
		  swap(last_gray, current_gray);
		  frames++;
		  if (cvWaitKey(2) == 'q')
			  break;
		  clock_t t2 = clock();
		  //cout << "Time is " << t2 - t1 << "ms"<<endl;
		  if (tld.Rtrack)
		  {
			  t += (t2 - t1);
			  trackedframe++;
		  }
		  
	  }
	  time.push_back(t / trackedframe);
	  trackaccuracy.push_back((float)trackedframe / (float)frames);
	  fclose(bb_file);
	  capture.release();
	
  }
  ofstream fout("results.txt");

  cout << left;
  fout << left;

  cout << endl;
  cout << "************************************************************" << endl;
  fout << "************************************************************" << endl;
  cout << setw(24) << "video name" << setw(15) << "fps" << setw(15) << "tracking accuracy(%)" << endl;
  fout << setw(24) << "video name" << setw(15) << "fps" << setw(15) << "tracking accuracy(%)" << endl;
  cout << setprecision(3) << fixed;
  fout << setprecision(3) << fixed;
  for (int i = 0; i < storage.size(); i++)
  {
	  cout << setw(24) << storage[i][0] << setw(15) << 1000 / time[i] << setw(25);
	  fout << setw(24) << storage[i][0] << setw(15) << 1000 / time[i] << setw(25);
	  cout << trackaccuracy[i] * 100 << endl;
	  fout << trackaccuracy[i] * 100 << endl;
  }
  cout << "************************************************************" << endl;
  fout << "************************************************************" << endl;
  return 0;
}
