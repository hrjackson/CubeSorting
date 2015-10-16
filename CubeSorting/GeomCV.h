//
//  GeomCV.h
//  CubeSorting
//
//  Created by Henry Jackson on 02/10/2015.
//  Copyright (c) 2015 Henry Jackson. All rights reserved.
//

#ifndef __CubeSorting__GeomCV__
#define __CubeSorting__GeomCV__

#include <stdio.h>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "Geometry.h"


void CallBackFunc(int event, int x, int y, int flags, void* input);

void drawCube(cv::Mat image, geom::Cube cube);

#endif /* defined(__CubeSorting__GeomCV__) */
