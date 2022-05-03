#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "opencv2/opencv.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
using namespace cv;

vector<string> split(const string &s, char delim) 
{
    vector<string> result;
    stringstream ss(s);
    string item;

    while(getline(ss, item, delim)) 
	{
		if(item.length() < 1)
		{
			result.push_back("NULL");
		}
		else
		{
			result.push_back(item);
		}
    }
    return result;
}

void drawLine(Mat img, Point start, Point end, Scalar color)
{
    int thickness = 5;
    int lineType = LINE_8;
    line(img,
        start,
        end,
        color,
        thickness,
        lineType);
}

// ID;Lat;Lon;Alt;Vel;Ax;Ay;Az;Mx;My;Mz
int main()
{
    string fname;
	// cout<<"Enter the file name: ";
	// cin>>fname;
    fname = "../../20220322_IMU_data/ELTEkor_20220322.csv";
 
	vector<vector<string>> content;
	string line;
 
	fstream file(fname, ios::in);
	if(file.is_open())
	{
		while(getline(file, line))
		{
			content.push_back(split(line, ';'));
		}
	}
	else
	{
		cout<<"Could not open the file\n";
	}

	int lastID = stoi(content.back().at(0));

	vector<float> accX;
    vector<float> accY;
    vector<float> accZ;

	float sumX = 0.0;
	float sumY = 0.0;
	float sumZ = 0.0;
	int count = 0;
	int rowIndex = 1;

	for(int i = 1; i <= lastID; i++)
	{
		sumX = 0.0;
		sumY = 0.0;
		sumZ = 0.0;
		count = 0;

		while(stoi(content[rowIndex][0]) == i)
		{
			try
			{
				sumX += stof(content[rowIndex][5]);
				sumY += stof(content[rowIndex][6]);
				sumZ += stof(content[rowIndex][7]);

				count++;
			}
			catch(const std::exception& e)
			{
				std::cerr << "Exception: " << e.what() << " on line " + to_string(i) << '\n';
			}			

			if(rowIndex == (content.size()-1))
			{
				break;
			}
			else
			{
				rowIndex++;
			}
		}

		if(count == 0)
		{
			// for FRAME indices that had no IMU data recorded for them (skipped frames)
			accX.push_back(sumX);
			accY.push_back(sumY);
			accZ.push_back(sumZ);
		}
		else
		{
			accX.push_back(sumX/count);
			accY.push_back(sumY/count);
			accZ.push_back(sumZ/count);
		}
	}

	int numFrames;
    VideoCapture cap("../../20220322_IMU_data/ELTEkor_20220322_preview.mp4");
    numFrames = cap.get(CAP_PROP_FRAME_COUNT);
    cout << "Number of frames: " + to_string(numFrames) << endl;
    // system("pause");
    // cap.release();

	// Default resolutions of the frame are obtained.The default resolutions are system dependent.
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    
    // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file.
    VideoWriter video("eltekor.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 2, Size(frame_width, frame_height));

    // Check if stream opened successfully
    if(!cap.isOpened())
    {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    int i = 0;
	while(1)
    {
        Mat frame;
        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediacdtely
        if(frame.empty())
		{
			break;
		}

		cv::putText(frame, 
			"AccY: " + to_string(accY[i]),
			Point(15, 105), // Coordinates (Bottom-left corner of the text string in the image)
			FONT_HERSHEY_COMPLEX_SMALL, // Font
			1.0, // Scale. 2.0 = 2x bigger
			Scalar(255, 255, 255), // BGR Color
			1, // Line Thickness (Optional)
			LINE_AA); // Anti-alias (Optional, see version note)

		putText(frame, 
			"AccX: " + to_string(accX[i]),
			Point(15, 125), // Coordinates (Bottom-left corner of the text string in the image)
			FONT_HERSHEY_COMPLEX_SMALL, // Font
			1.0, // Scale. 2.0 = 2x bigger
			Scalar(255, 255, 255), // BGR Color
			1, // Line Thickness (Optional)
			LINE_AA); // Anti-alias (Optional, see version note)

		putText(frame, 
			"AccZ: " + to_string(accZ[i]),
			Point(15, 145), // Coordinates (Bottom-left corner of the text string in the image)
			FONT_HERSHEY_COMPLEX_SMALL, // Font
			1.0, // Scale. 2.0 = 2x bigger
			Scalar(255, 255, 255), // BGR Color
			1, // Line Thickness (Optional)
			LINE_AA); // Anti-alias (Optional, see version note)

		// putText(frame, 
		// 	// to_string(axr) + "\n" + to_string(ayr) + "\n" + to_string(azr),
		// 	"Acc Magnitude: " + to_string(mag),
		// 	Point(15, 165), // Coordinates (Bottom-left corner of the text string in the image)
		// 	FONT_HERSHEY_COMPLEX_SMALL, // Font
		// 	1.0, // Scale. 2.0 = 2x bigger
		// 	Scalar(255, 255, 255), // BGR Color
		// 	1, // Line Thickness (Optional)
		// 	LINE_AA); // Anti-alias (Optional, see version note)

        // DRAW AXES in upper right corner of the frame
		// origin at x: 1660, y: 350
        Point p1(1510, 350), p2(1810, 350);
        Point p3(1660, 200), p4(1660, 500);
        
        drawLine(frame, p1, p2, Scalar(255, 255, 255));
        drawLine(frame, p3, p4, Scalar(255, 255, 255));

		Point origin(1660, 350);
		Point accPoint(1660-(1000.0*accY[i]), 350-(1000.0*accX[i]));

		drawLine(frame, origin, accPoint, Scalar(0, 255, 0));
            
        // Write the frame into the file 'outcpp.avi'
        video.write(frame);

        // Press  ESC on keyboard to exit
        char c = (char)waitKey(25);
        if(c == 27)
        {
            break;
        }
        i++;
		// TODO: check if vector lengths are the same as the number of frames, so the indices line up correctly
    }

    // When everything done, release the video capture object and video writer object
    cap.release();
    video.release();

    VideoCapture cap1("./eltekor.avi");

    // Check if stream opened successfully
    if(!cap1.isOpened())
    {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    while(1)
    {
        Mat frame;
        // Capture frame-by-frame
        cap1 >> frame;

        // If the frame is empty, break immediately
        if(frame.empty())
		{
			break;
		}            

        // Display the resulting frame
        imshow("Frame", frame);
        // sleep(1);

        // Press  ESC on keyboard to exit
        char c = (char)waitKey(25);
        if(c == 27)
        {
            break;
        }
    }

    cap1.release();
    // Closes all the frames
    destroyAllWindows();

	return 0;
}