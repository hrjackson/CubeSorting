//
//  GeomCV.cpp
//  CubeSorting
//
//  Created by Henry Jackson on 02/10/2015.
//  Copyright (c) 2015 Henry Jackson. All rights reserved.
//

#include "GeomCV.h"


void CallBackFunc(int event, int x, int y, int flags, void* input){
    if(event == cv::EVENT_LBUTTONDOWN){
        std::cout << "Left click at " << x << ", " << y << std::endl;
        std::vector<void*>* data = (std::vector<void*>*)input;
        std::vector<geom::Point2d>* points = (std::vector<geom::Point2d>*)(*data)[0];
        cv::Mat* image = (cv::Mat*)(*data)[1];
        points->push_back(geom::Point2d(x,y));
        cv::circle(*image, cv::Point(x, y), 5, cv::Scalar(255,0,0), -1);
        imshow("Cube", *image);
    }
}

void drawFace(cv::Mat image,
              geom::Point2d a,
              geom::Point2d b,
              geom::Point2d c,
              geom::Point2d d,
              cv::Scalar colour) {
    cv::line(image, cv::Point(a.xy[0], a.xy[1]), cv::Point(b.xy[0], b.xy[1]), colour, 4);
    cv::line(image, cv::Point(c.xy[0], c.xy[1]), cv::Point(b.xy[0], b.xy[1]), colour, 4);
    cv::line(image, cv::Point(c.xy[0], c.xy[1]), cv::Point(d.xy[0], d.xy[1]), colour, 4);
    cv::line(image, cv::Point(a.xy[0], a.xy[1]), cv::Point(d.xy[0], d.xy[1]), colour, 4);
}

void drawCube(cv::Mat image, geom::Cube cube){
    std::vector<geom::Point2d> points = cube.projectPoints();
    cv::Scalar dark(100,100,100);
    cv::Scalar light(255,255,255);
    
    drawFace(image, points[1], points[3], points[7], points[5], dark);
    drawFace(image, points[7], points[6], points[4], points[5], dark);
    drawFace(image, points[3], points[7], points[6], points[2], dark);
    drawFace(image, points[0], points[4], points[6], points[2], light);
    drawFace(image, points[1], points[3], points[2], points[0], light);
    drawFace(image, points[1], points[5], points[4], points[0], light);
    
    //for (auto point = points.begin(); point != points.end(); ++point) {
    //    cv::circle(image, cv::Point(point->xy[0], point->xy[1]), 5, cv::Scalar(0,255,0), -1);
    //}
}