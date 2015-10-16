//
//  GradDesc.cpp
//  CubeSorting
//
//  Created by Henry Jackson on 29/09/2015.
//  Copyright (c) 2015 Henry Jackson. All rights reserved.
//
#include <iostream>

#include "GradDesc.h"

typedef std::vector<double> vec;

vec gradientDescent(double (*function)(vec args),
                    vec init,
                    double rate,
                    double tol){
    vec oldTheta = init;
    vec theta = init;
    double err;
    double tolSq = tol*tol;
    while (true) {
        theta = stepDown(function, oldTheta, rate);
        std::cout << "theta = " << theta[0]  << ", " << theta[1] << std::endl;
        err = distSq(theta, oldTheta);
        std::cout << "err = " << err << std::endl;
        if (err < tolSq) {
            break;
        }
        oldTheta = theta;
    }
    return theta;
}


vec stepDown(double (*function)(vec args),
             vec theta,
             double rate){
    vec grad = {1};
    grad = findGradient(function, theta);
    std::cout << "gradient = " << grad[0] << ", " << grad[1] << std::endl;
    vec newTheta = theta;
    for (int i = 0; i < theta.size(); i++) {
        newTheta[i] -= rate*grad[i];
    }
    return newTheta;
}


vec findGradient(double (*function)(vec args),
                 vec theta){
    double dTheta = 0.0000001; // Smaller -> better approximation
    double value = function(theta);
    vec newTheta = theta;
    double dValue;
    vec gradient;
    gradient.reserve(theta.size());
    for (int i = 0; i < theta.size(); ++i) {
        newTheta[i] = theta[i] + dTheta;
        dValue = function(newTheta) - value;
        gradient[i] = dValue/dTheta;
        newTheta[i] = theta[i];
    }
    return gradient;
}


double distSq(vec v, vec w){
    //assert(v.size() == w.size());
    double dist = 0;
    double dElem; // temp variable to hold element differences in loop
    for (int i = 0; i < v.size(); ++i) {
        dElem = v[i] - w[i];
        dist += dElem*dElem;
    }
    return dist;
}