#include "csv_reader.hpp"

void CsvReader::retrieveFileItems() 
{
    std::fstream file(this->fileName, std::ios::in);
    std::string line;
    if(file.is_open()) 
    {
        while(getline(file, line)) 
        {
            this->content.push_back(split(line, ';'));
        }
    }
    else 
    {
        throw std::runtime_error("Error: failed to open file");
    }
}

void CsvReader::processData() 
{
    float sumX = 0.0;
	float sumY = 0.0;
	float sumZ = 0.0;
	int count = 0;
	int rowIndex = 1;

    int lastID = stoi(content.back().at(0));

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

			} catch(const std::exception& e) 
            {
				std::cerr << "Exception: " << e.what() << " on line " + std::to_string(i) << '\n';
            }

			if(rowIndex == (content.size()-1)) 
            {
				break;

			} else 
            {
				rowIndex++;
			}
		}

		if(count == 0) 
        {
			// For FRAME indices that had no IMU data recorded for them (skipped frames)
			accX.push_back(sumX);
			accY.push_back(sumY);
			accZ.push_back(sumZ);

		} else 
        {
			accX.push_back(sumX/count);
			accY.push_back(sumY/count);
			accZ.push_back(sumZ/count);
		}
	}
}

void CsvReader::writeToVideo(const std::string &fileName) 
{
    int numFrames;
    cv::VideoCapture cap(fileName);
    numFrames = cap.get(cv::CAP_PROP_FRAME_COUNT);
    std::cout << "Number of frames: " + std::to_string(numFrames) << '\n';

    // Default resolutions of the frame are obtained (system dependent)
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    // Define the codec and create VideoWriter object
    cv::VideoWriter video(newVideoName, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 2, cv::Size(frame_width, frame_height));

    if(!cap.isOpened()) 
    {
        throw std::runtime_error("error: failed to open video stream");
    }

    int i = 0;
    while(true) 
    {
        cv::Mat frame;
        cap >> frame;
        if(frame.empty()) break;

        cv::putText(frame, 
			"AccY: " + std::to_string(accY[i]),
			cv::Point(15, 105), // Coordinates (Bottom-left corner of the text string in the image)
			cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
			1.0, // Scale
			cv::Scalar(255, 255, 255), // BGR Color
			1, // Line Thickness
			cv::LINE_AA); // Anti-alias

		cv::putText(frame, 
			"AccX: " + std::to_string(accX[i]),
			cv::Point(15, 125), // Coordinates (Bottom-left corner of the text string in the image)
			cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
			1.0, // Scale
			cv::Scalar(255, 255, 255), // BGR Color
			1, // Line Thickness
			cv::LINE_AA); // Anti-alias

		cv::putText(frame, 
			"AccZ: " + std::to_string(accZ[i]),
			cv::Point(15, 145), // Coordinates (Bottom-left corner of the text string in the image)
			cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
			1.0, // Scale
			cv::Scalar(255, 255, 255), // BGR Color
			1, // Line Thickness
			cv::LINE_AA); // Anti-alias

        // DRAW AXES in upper right corner of the frame
		// Origin at x: 1660, y: 350
        cv::Point p1(1510, 350), p2(1810, 350);
        cv::Point p3(1660, 200), p4(1660, 500);
        
        drawLine(frame, p1, p2, cv::Scalar(255, 255, 255));
        drawLine(frame, p3, p4, cv::Scalar(255, 255, 255));

		cv::Point origin(1660, 350);
		cv::Point accPoint(1660-(1000.0*accY[i]), 350-(1000.0*accX[i]));

		drawLine(frame, origin, accPoint, cv::Scalar(0, 255, 0));
            
        // Write the frame into the file 'outcpp.avi'
        video.write(frame);

        // Press  ESC on keyboard to exit
        char c = (char)cv::waitKey(25);
        if(c == 27) break;
        i++;
    }

    // When everything done, release the video capture object and video writer object
    cap.release();
    video.release();
}

void CsvReader::drawLine(cv::Mat &img, const cv::Point &start, const cv::Point &end, const cv::Scalar color) const 
{
    int thickness = 5;
    int lineType = cv::LINE_8;
    cv::line(img, start, end, color, thickness, lineType);
}

std::vector<std::string> CsvReader::split(const std::string &s, char delim) const 
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while(getline(ss, item, delim)) 
    {
		if(item.length() < 1) 
        {
			result.push_back("NULL");

		} else 
        {
			result.push_back(item);
		}
    }

    return result;
}

void CsvReader::showFrames() const 
{
    cv::VideoCapture cap(newVideoName);

    // Check if stream opened successfully
    if(!cap.isOpened()) 
    {
        throw std::runtime_error("Error: failed to open video stream in function showframes()");
    }

    while(true) 
    {
        // Capture frame-by-frame
        cv::Mat frame;
        cap >> frame;

        // If the frame is empty, break immediately
        if(frame.empty()) break;        

        // Display the resulting frame
        cv::imshow("Frame", frame);

        // Press  ESC on keyboard to exit
        char c = (char)cv::waitKey(25);
        if(c == 27) break;
    }

    cap.release();
    // Closes all the frames
    cv::destroyAllWindows();
}