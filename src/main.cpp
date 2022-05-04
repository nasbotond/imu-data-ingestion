#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string>

#include "csv_reader.hpp"

int main() 
{
    // Test input
    std::string testFile = "../../20220322_IMU_data/ELTEkor_20220322.csv";
    CsvReader reader = CsvReader(testFile);

    try 
    {
        // Fill vectors
        reader.retrieveFileItems();
        reader.processData();

        // Write to new video
        std::string videoInput = "../../20220322_IMU_data/ELTEkor_20220322_preview.mp4";
        reader.writeToVideo(videoInput);

        // Display frames
        reader.showFrames();
    } 
    catch(const std::exception &e) 
    {
        std::cerr << e.what() << '\n';
        return -1;
    }

    return 0;
}