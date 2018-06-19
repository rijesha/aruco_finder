#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <aruco/aruco.h>
#include <map>
#include <vector>
#include "argparse.hpp"

using namespace boost::filesystem;
using namespace cv;
using namespace std;

string str_check_1 = ".png";
string str_check_2 = "cam_1.";

int main(int argc, const char** argv) {
    ArgumentParser parser;

    // add some arguments to search for
    parser.addArgument("-o", "--output_file",1);
    parser.addArgument("-c", "--cam_num",1);
    parser.addFinalArgument("input_directory");

    // parse the command-line arguments - throws if invalid format
    parser.parse(argc, argv);

    // if we get here, the configuration is valid
    string results_file_name = "marker_results.txt";
    string test_string = parser.retrieve<string>("o");
    if (test_string.length() != 0)
        results_file_name = test_string;

    test_string = parser.retrieve<string>("c");
    if (test_string.length() != 0){
        str_check_2 = "cam_";
        str_check_2.append(test_string);
        str_check_2.append(".");
    }

    ofstream resultsFile(results_file_name, ofstream::out);

    path p(parser.retrieve<string>("input_directory"));

    map<int, vector<int>> by_frame;
    map<int, vector<int>> by_marker;

    aruco::MarkerDetector MDetector;
    MDetector.setDictionary("ARUCO_MIP_16h3");

    cout << "Iterating" << endl;
    
    directory_iterator end_itr;

    for (directory_iterator itr(p); itr != end_itr; ++itr)
    {
        if (is_regular_file(itr->path())) {
            string current_file = itr->path().string();
            string file_name = itr->path().filename().string();
            size_t found_str_check_1 = current_file.find(str_check_1);
            size_t found_str_check_2 = current_file.find(str_check_2);
            
            if (found_str_check_1!=string::npos && found_str_check_2!=string::npos){
                Mat image = imread(current_file, CV_LOAD_IMAGE_COLOR);
                string framenum = file_name.substr (6,5);
                auto pos = framenum.rfind('_');
                if (pos != string::npos) {
                    framenum.erase(pos);
                }

                if(image.data )
                {
                    vector<int> markernumber;
                    std::vector<aruco::Marker> markers=MDetector.detect(image);
                    if (markers.size() > 0) {
                        for(size_t i=0;i<markers.size();i++){
                            markernumber.push_back(markers[i].id);
                            cout << "frame: " << framenum << " has marker number: " << to_string(markernumber[0]) << endl;                        
                        }
                        by_frame.insert({stoi(framenum),markernumber});
                    }

                } else {
                    cout <<  "Could not open or find the image: " << file_name << endl ;
                }
            }
        }
    }
    
    resultsFile << "By FileNumber: " << endl;
    cout << "Processing Marker List" << endl;
    stringstream ss;
    
    for (auto frame : by_frame)
    {
        ss << frame.first << ':';
        for (auto marker : frame.second){
            ss << marker << ", ";
            auto search = by_marker.find(marker);

            if(search != by_marker.end()) {
                vector<int> temp = search->second;                
                search->second.push_back(frame.first);
            } else {
                vector<int> fun;
                fun.push_back(frame.first);
                by_marker.insert({marker, fun});
            }   
        }
        ss << endl;
    }

    ss << endl << endl;
    ss << "--------------------------------------" << endl;
    ss << "By MARKER:" << endl;
    for (auto marker : by_marker)
    {
        ss << marker.first << ':';
        for (auto frame : marker.second){
            ss << frame << ", ";           
        }

        ss << endl ;
    } 

    resultsFile << ss.str();
    resultsFile.close();
    return 0;
}