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

int main()
{
    string fname;
	// cout<<"Enter the file name: ";
	// cin>>fname;
    fname = "../../20220322_IMU_data/ELTEkor_20220322.csv";
 
	vector<vector<string>> content;
	vector<string> row;
	string line, word;
 
	fstream file (fname, ios::in);
	if(file.is_open())
	{
		while(getline(file, line))
		{
			row.clear(); 
			stringstream str(line);
 
			while(getline(str, word, ';'))
			{
                if(word.length() < 1)
                {
                    row.push_back("NULL");
                }
                else
                {
				    row.push_back(word);
                }
			}
			content.push_back(row);
		}
	}
	else
	{
		cout<<"Could not open the file\n";
	}
    // ID;Lat;Lon;Alt;Vel;Ax;Ay;Az;Mx;My;Mz

	vector<float> accX; // -999.99 for invalid
    vector<float> accY; // -999.99 for invalid
    vector<float> accZ; // -999.99 for invalid
	// TODO
	vector<float> altitude; // m ... -999.99 for invalid
    vector<float> speed; // km/h ... -999.99 for invalid
    vector<string> gps; // is this even necessary ?? TBD

	float sumX;
	float sumY;
	float sumZ;
	int count;
	int lastID = stoi(content.back().at(0));

	for(int i = 1; i <= lastID; i++)
	{
		sumX = 0.0;
		sumY = 0.0;
		sumZ = 0.0;
		count = 0;
		while(stoi(content[i][0]) == i)
		{
			sumX += stof(content[i][5]);
			sumY += stof(content[i][6]);
			sumZ += stof(content[i][7]);
			count++;
		}
		cout<<"FRAME\n"<<" ";
		cout<<i <<" ";
		cout<<sumX/count<<" ";
		cout<<sumY/count<<" ";
		cout<<sumZ/count<<" ";

		accX.push_back(sumX/count);
		accY.push_back(sumY/count);
		accZ.push_back(sumZ/count);
	}

	// for(int i=0; i<content.size(); i++)
	// {
	// 	for(int j=0; j<content[i].size(); j++)
	// 	{
	// 		accX.push_back(stof(content[i][5]));
	// 		accY.push_back(stof(content[i][6]));
	// 		accZ.push_back(stof(content[i][7]));

	// 		// cout<<content[i][j]<<" ";
	// 	}
	// 	// cout<<"\n";
	// }
 
	return 0;
}