#include "opencv2/opencv.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

vector<string> parse_comma_separated_string(string input)
{
    stringstream ss( input );
    vector<string> result;

    while( ss.good() )
    {
        string substr;
        getline( ss, substr, ',' );
        result.push_back( substr );
    }
    return result;
}

string get_str_between_two_str(const string s,
        const string start,
        const string stop)
{
    unsigned first_delim_pos = s.find(start);
    unsigned end_pos_of_first_delim = first_delim_pos + start.length();
    unsigned last_delim_pos = s.find_first_of(stop, end_pos_of_first_delim);
    
    return s.substr(end_pos_of_first_delim,
            last_delim_pos - end_pos_of_first_delim);
}


int main() 
{
    int numFrames;
    VideoCapture cap("../../20220127_IMU_data/Alagut_3cam_4FPS_GPS_IMU_preview.mp4");
    numFrames = cap.get(CAP_PROP_FRAME_COUNT);

    // system("pause");
    // cap.release();

    // There are 4963 files in total...
    // Create a text string, which is used to output the text file
    string line;
    vector<string> gps;
    vector<float> accX;
    vector<float> accY;
    vector<float> accZ;
    vector<float> altitude; // m
    vector<float> speed; // km/h

    for(int i = 1; i <= 1000; i++) // this should be 1 -> numFrames
    {
        string path = "../../20220127_IMU_data/alagut_pol/test_fn" + to_string(i) + ".pol";
        // Read from the text file
        ifstream File(path);

        for (int lineno = 0; getline (File,line) && lineno < 7; lineno++)
        {
            if (lineno == 0) // gps
            {
                gps.push_back(line);
            }
            if (lineno == 1)
            {
                string spd = get_str_between_two_str(line, " ", "k");
                replace(spd.begin(), spd.end(), ',', '.');
                speed.push_back(stof(spd));
            }
            if (lineno == 2)
            {
                string alt = get_str_between_two_str(line, " ", "m");
                replace(alt.begin(), alt.end(), ',', '.');
                altitude.push_back(stof(alt));
            }
            if (lineno == 5)
            {
                vector<string> result = parse_comma_separated_string(line);
                accX.push_back(stof(result[0]));
                accY.push_back(stof(result[1]));
                accZ.push_back(stof(result[2]));
            } 
        }

        // Close the file
        File.close();
    }

    // print all elements
    // cout << "vector elements are: ";
    // for (int i = 0; i < speed.size(); ++i) {
    //     cout << speed[i] << ' ';
    // }
    // cout << endl;
    // cout << "Total Number of frames are:" << numFrames << endl;

    // Create a VideoCapture object and open the input file
    // If the input is the web camera, pass 0 instead of the video file name
    // VideoCapture cap("../../20220127_IMU_data/Alagut_3cam_4FPS_GPS_IMU_preview.mp4");

    // Check if camera opened successfully
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

        // If the frame is empty, break immediately
        if(frame.empty())
            break;

        putText(frame, 
            "Speed: " + to_string(speed[i]),
            cv::Point(15,15), // Coordinates (Bottom-left corner of the text string in the image)
            cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
            1.0, // Scale. 2.0 = 2x bigger
            cv::Scalar(255,255,255), // BGR Color
            1, // Line Thickness (Optional)
            cv:: LINE_AA); // Anti-alias (Optional, see version note)

        // Display the resulting frame
        imshow( "Frame", frame );

        // Press  ESC on keyboard to exit
        char c = (char)waitKey(25);
        if(c == 27)
            break;

        i++;
    }

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}

