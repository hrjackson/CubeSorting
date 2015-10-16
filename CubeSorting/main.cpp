//
//  main.cpp
//  CubeSorting
//
//  Created by Henry Jackson on 28/09/2015.
//  Copyright (c) 2015 Henry Jackson. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

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
                break;
                
            case 'o':
                ss >> outputDirectory;
                
            case 'w':
                ss >> width;
                
            case 'h':
                ss >> height;
                
            default:
                usage();
        }
    }
    
    // Read image
    Mat img = imread(inputDirectory);
    
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
    
    drawCube(image, geom::Cube(theta));
    for (int i = 0; i < theta.size(); ++i) {
        std::cout << theta[i] << " ";
    }
    std::cout << std::endl;
    
    imshow("Cube", image);
    waitKey();

    return 0;
}


int usage(){
    std::cout << "Usage: CubeSorting [options] (defaults in brackets)" << std::endl;
    std::cout << "-i [input directory]" << std::endl;
    std::cout << "-o [output directory]" << std::endl;
    std::cout << "-w [resize width] (480)" << std::endl;
    std::cout << "-h [resize height] (640" << std::endl;
    return 1;
}
