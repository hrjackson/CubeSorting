//
//  GradDesc.h
//  CubeSorting
//
//  Created by Henry Jackson on 29/09/2015.
//  Copyright (c) 2015 Henry Jackson. All rights reserved.
//

#ifndef __CubeSorting__GradDesc__
#define __CubeSorting__GradDesc__

#include <stdio.h>
#include <vector>

namespace gd{
    
    typedef std::vector<double> vec;
    
    /*
     Computes the dot product of two vectors
     */
    double dot(std::vector<double> v, std::vector<double> w){
        double sum = 0;
        for (int i = 0; i < v.size(); ++i) {
            sum += v[i]*w[i];
        }
        return sum;
    }
    
    /*
     Computes the squared Euclidean distance between v and w.
     */
    double distSq(std::vector<double> v, std::vector<double> w){
        double dist = 0;
        double dElem; // temp variable to hold element differences in loop
        for (int i = 0; i < v.size(); ++i) {
            dElem = v[i] - w[i];
            dist += dElem*dElem;
        }
        return dist;
    }
    

    
    
    /*
     Numerically calculates the gradient of the function at the position theta.
     */
    template<typename Functor>
    std::vector<double> findGradient(Functor F,
                                     std::vector<double> theta){
        double dTheta = 0.0001; // Smaller -> better approximation
        double value = F(theta);
        vec newTheta = theta;
        double dValue;
        vec gradient = theta;
        for (int i = 0; i < theta.size(); ++i) {
            newTheta[i] = theta[i] + dTheta;
            dValue = F(newTheta) - value;
            gradient[i] = dValue/dTheta;
            newTheta[i] = theta[i];
        }
        return gradient;
    }
    
    
    /*
     Takes one step down the gradient of function at position theta,
     of step size rate.
     Input: 
     function - the function which we descend
     theta    - the argument of the function. Position we want to descend from
     rate     - multiplier of gradient. How far we step.
     Output: vector which is an update of theta
     */
    template<typename Functor>
    std::vector<double> stepDown(Functor F,
                                 std::vector<double> theta,
                                 std::vector<double> rate,
                                 std::vector<double>& grad){
        grad = findGradient<Functor>(F, theta);
        vec newTheta = theta;
        for (int i = 0; i < theta.size(); i++) {
            newTheta[i] -= rate[i]*grad[i];
        }
        return newTheta;
    }

    
    
    /*
     Performs standard gradient descent on the function passed in.
     Input: function  - the function to be optimised
     init             - initial guess for argument of function
     rate             - learning rate: gradient multiplied by this to give different descent rate
     tol              - how small the change in theta needs to be before alg can terminate
     Output: vector which satisfies argmin(function)
     */
    template<typename Functor>
    std::vector<double> gradientDescent(Functor F,
                                        std::vector<double> init,
                                        std::vector<double> rate,
                                        double tol){
        vec oldTheta = init;
        vec theta = init;
        vec grad = init;
        int i = 0;
        cv::Mat temp;
        while (true) {
            ++i;
            theta = stepDown<Functor>(F, oldTheta, rate, grad);
            oldTheta = theta;
            if (dot(grad,grad) < tol | i > 10000) {
                break;
            }
        }
        std::cout << "Gradient descent terminated in " << i << " iterations." << std::endl;
        return theta;
    }
    
    
} // namespace gd

#endif /* defined(__CubeSorting__GradDesc__) */
