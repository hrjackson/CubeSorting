//
//  Geometry.cpp
//  CubeSorting
//
//  Created by Henry Jackson on 30/09/2015.
//  Copyright (c) 2015 Henry Jackson. All rights reserved.
//

#include "Geometry.h"
#include <cmath>
#include <iostream>

namespace geom {
    typedef std::vector<double> vec;
    
    /*--- Point member functions ---*/
    Point3d::Point3d(double x, double y, double z){
        xyz[0] = x;
        xyz[1] = y;
        xyz[2] = z;
    }
    
    Point3d Point3d::operator*(double lambda){
        return Point3d(lambda*this->xyz[0], lambda*this->xyz[1], lambda*this->xyz[2]);
    }
    
    Point3d operator*(double lambda, Point3d rhs){
        return rhs*lambda;
    }
    
    Point2d::Point2d(){
        xy[0] = 0;
        xy[1] = 0;
    }
    
    Point2d::Point2d(double x, double y){
        xy[0] = x;
        xy[1] = y;
    }
    
    Point2d Point2d::operator+(Point2d& p){
        return Point2d(this->xy[0] + p.xy[0], this->xy[1] + p.xy[1]);
    }
    
    Point2d Point2d::operator-(Point2d& p){
        return Point2d(this->xy[0] - p.xy[0], this->xy[1] - p.xy[1]);
    }
    
    Point2d Point2d::operator*(double lambda){
        return Point2d(lambda*this->xy[0], lambda*this->xy[1]);
    }
    
    double Point2d::operator*(Point2d& p){
        return this->xy[0]*p.xy[0] + this->xy[1]*p.xy[1];
    }
    
    Point2d operator*(double lambda, Point2d rhs){
        return rhs*lambda;
    }
    
    /*--- VirtualGeom member functions ---*/
    
    Point3d VirtualGeom::_rotate(Point3d p, double theta, int dim){
        Point3d q = p;

        // Indeces which aren't == dim
        int lowerInd = 0;
        int higherInd = 2;
        if (dim == 0) {
            lowerInd = 1;
        } else if (dim == 2){
            higherInd = 1;
        }
        
        // Rotation
        q.xyz[lowerInd] = cos(theta)*p.xyz[lowerInd] - sin(theta)*p.xyz[higherInd];
        q.xyz[higherInd] = sin(theta)*p.xyz[lowerInd] + cos(theta)*p.xyz[higherInd];
        return q;
    }
    
    Point3d VirtualGeom::_rotate(Point3d p, vec theta){
        Point3d q = p;
        for (int i = 2; i >= 0; --i) {
            q = _rotate(q, theta[i], i);
        }
        return q;
    }
    
    Point2d VirtualGeom::_project(Point3d p, double cameraDist){
        Point2d v(p.xyz[1], p.xyz[2]);
        double lambda = (-cameraDist/10)/(p.xyz[0] - cameraDist);
        v = lambda*v;
        return v;
    }
    
    Point2d VirtualGeom::_rotateThenProject(Point3d p,
                                            vec theta,
                                            double cameraDist){
        p = _rotate(p, theta);
        return _project(p, cameraDist);
    }
    
    void VirtualGeom::_extractParams(vec params,
                                     vec &theta,
                                     double &cameraDist,
                                     double &scale,
                                     geom::Point2d &p){
        theta = {params[0], params[1], params[2]};
        cameraDist = params[3];
        scale = params[4];
        p.xy[0] = params[5];
        p.xy[1] = params[6];
    }
    
    /*--- Objective member functions ---*/
    Objective::Objective(std::vector<Point2d> userInput)
    : _observedPoints(userInput) {
        _vertices.push_back(Point3d(0, 0, 0));
        _vertices.push_back(Point3d(1, 1, 0));
        _vertices.push_back(Point3d(1, 0, 0));
        _vertices.push_back(Point3d(1, 0, 1));
        _vertices.push_back(Point3d(0, 0, 1));
        _vertices.push_back(Point3d(0, 1, 1));
        _vertices.push_back(Point3d(0, 1, 0));
    }
    
    double Objective::operator()(vec params){
        double sum = 0;
        for (int i = 0; i < _observedPoints.size(); ++i) {
            sum += _squaredDist(i, params);
        }
        return sum;
    }
    
    double Objective::_squaredDist(int vertIndex, vec params){
        vec theta;
        double cameraDist;
        double scale;
        Point2d p;
        _extractParams(params, theta, cameraDist, scale, p);
        Point2d v = _rotateThenProject(_vertices[vertIndex],
                                       theta,
                                       cameraDist);
        v = scale*v + p;
        Point2d difference = v - _observedPoints[vertIndex];
        return difference*difference;
    }
    
    
    /*--- Cube member functions ---*/
    Cube::Cube(vec params)
    :params(params) {
        _extractParams(params, theta, cameraDist, scale, centre);
        vertices = _generateVertices();
        for (auto it = vertices.begin(); it != vertices.end(); ++it) {
            projected.push_back(scale*_project((*it), cameraDist) + centre);
        }
    }
    
    std::vector<Point2d> Cube::projectPoints(){
        return projected;
    }
    
    vec Cube::getParams(){
        return params;
    }
    
    std::vector<Point3d> Cube::_generateVertices(){
        std::vector<Point3d> verts;
        for (double x = 0; x < 2; ++x) {
            for (double y = 0; y < 2; ++y) {
                for (double z = 0; z < 2; ++z) {
                    verts.push_back(_rotate(Point3d(x,y,z), theta));
                }
            }
        }
        return verts;
    }
    
}  // namespace geom































