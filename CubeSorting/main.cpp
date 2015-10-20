//
//  main.cpp
//  CubeSorting
//
//  Created by Henry Jackson on 28/09/2015.
//  Copyright (c) 2015 Henry Jackson. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "GradDesc.h"
#include "Geometry.h"
#include "GeomCV.h"

using namespace cv;


// Print the usage instructions to the console.
int usage();


int main(int argc, const char * argv[]) {
    // Get user input
    // Stringstream for parsing arguments.
    std::stringstream ss;
    char switchChar;
    std::string inputDirectory = "";
    std::string outputDirectory = "";
    int width = 480;
    int height = 640;
    
    if(argc == 1) return usage();
    
    for (int i = 1; i < argc; ++i  ) {
        if (argv[i][0] != '-') {
            return usage();
        }
        switchChar = argv[i][1];
        ss.clear();
        ss.str(argv[++i]);
        if (argc <= i) {
            usage();
        }
        switch ( switchChar ) {
            case 'i':
                ss >> inputDirectory;
            
            case 'o':
                ss >> outputDirectory;
                break;
                
            case 'w':
                ss >> width;
                break;
                
            case 'h':
                ss >> height;
                break;
                
            default:
                usage();
        }
    }
    
    std::string inFile = "";
    std::string outFile = "";
    
    // Prepare outfile
    std::ofstream output;
    outFile = "out.csv";
    output.open(outputDirectory + outFile);
    
    for (int i = 1;;i++) {
        // Read image
        inFile = std::to_string(i) + ".jpg";
        Mat img = imread(inputDirectory + inFile);
        if (! img.data) {
            break;
        }
        
        Mat image;
        resize(img, image, Size(width, height));
        
        // Check
        if (image.empty()) {
            std::cout << "Error loading image" << std::endl;
            return -1;
        }
        
        // Get user input
        namedWindow("Cube");
        
        std::vector<geom::Point2d> points;
        std::vector<void*> data;
        data.push_back((void*)&points);
        data.push_back((void*)&image);
        setMouseCallback("Cube", CallBackFunc, (void*)&data);
        
        imshow("Cube", image);
        waitKey();
        
        // Process user input
        geom::Objective F(points);  // Construct objective function with seen data
        
        double pi = std::acos(-1);
        std::vector<double> init = {0, -pi/4, -pi/4, -1, 1000, (double)width/2, (double)height/2};
        std::vector<double> secondRate = {0.000001, 0.000001, 0.000001, 0.0001, 10, 0.01, 0.01};
        std::vector<double> theta = gd::gradientDescent<geom::Objective>(F, init, secondRate, 10);
        
        geom::Cube fitCube(theta);
        
        drawCube(image, fitCube);
        for (int i = 0; i < theta.size(); ++i) {
            std::cout << theta[i] << " ";
        }
        std::cout << std::endl;
        
        imshow("Cube", image);
        int k = waitKey();
        
        if (k == 13 || k == 32){
            // accept the fitted cube
            std::cout << "Exporting data..." << std::endl;
            std::vector<geom::Point2d> projected = fitCube.projectPoints();
            geom::vec params = fitCube.getParams();
            output << std::to_string(i);
            for (int i = 0; i < projected.size(); i++) {
                output << "," << projected[i].xy[0] << " " << projected[i].xy[1];
            }
            output << "\n";
            for (int i = 0; i < params.size(); i++) {
                output << "," << params[i];
            }
            output << "\n";
            
        }
        else{
            // reject the fitted cube
            std::cout << "Moving swiftly on..." << std::endl;
        }
    }
    // save output
    output.close();
    
    // all done!
    return 0;
}


int usage(){
    std::cout << "Usage: CubeSorting [options] (defaults in brackets)" << std::endl;
    std::cout << "-i [input directory]" << std::endl;
    std::cout << "-o [output directory]" << std::endl;
    std::cout << "-w [resize width] (480)" << std::endl;
    std::cout << "-h [resize height] (640)" << std::endl;
    return 1;
}
