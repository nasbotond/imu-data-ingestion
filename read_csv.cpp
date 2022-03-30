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
                if(word.length() < 1)
                {
                    row.push_back("NULL");
                }
                else
                {
				    row.push_back(word);
                }
			content.push_back(row);
		}
	}
	else
		cout<<"Could not open the file\n";
 
    // ID;Lat;Lon;Alt;Vel;Ax;Ay;Az;Mx;My;Mz

	// for(int i=0; i<content.size(); i++)
    // for(int i=0; i<10; i++)
	// {
	// 	for(int j=0; j<content[i].size(); j++)
	// 	{
	// 		cout<<content[i][j]<<" ";
	// 	}
	// 	cout<<"\n";
	// }

	vector<float> accX; // -999.99 for invalid
    vector<float> accY; // -999.99 for invalid
    vector<float> accZ; // -999.99 for invalid

	for(int i=0; i<content.size(); i++)
	{
		for(int j=0; j<content[i].size(); j++)
		{
			accX.push_back(stof(content[i][5]));
			accY.push_back(stof(content[i][6]));
			accZ.push_back(stof(content[i][7]));

			// cout<<content[i][j]<<" ";
		}
		// cout<<"\n";
	}
 
	return 0;
}