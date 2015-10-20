//
//  Geometry.h
//  CubeSorting
//
//  Created by Henry Jackson on 30/09/2015.
//  Copyright (c) 2015 Henry Jackson. All rights reserved.
//

#ifndef __CubeSorting__Geometry__
#define __CubeSorting__Geometry__

#include <stdio.h>
#include <vector>

namespace geom {
    typedef std::vector<double> vec;
    
    struct Point3d{
        Point3d(double x, double y, double z);
        double xyz[3];
        Point3d operator*(double lambda);
    };
    
    Point3d operator*(double lambda, Point3d rhs);
    
    struct Point2d{
        Point2d();  // Constructs the point (0,0)
        Point2d(double x, double y);
        double xy[2];
        Point2d operator+(Point2d& p);
        Point2d operator-(Point2d& p);
        Point2d operator*(double lambda);
        double operator*(Point2d& p);  // Dot product
    };
    
    Point2d operator*(double lambda, Point2d rhs);
    
    /*
     Not intended to be created on its own. Inherited by Objective and Cube objects.
     */
    class VirtualGeom {
    protected:
        /*
         Returns the point p rotated by angle theta around dimension dim.
         The dimension should be an integer:
           0 = x
           1 = y
           2 = z
         */
        Point3d _rotate(Point3d p,
                        double theta,  // theta in radians
                        int dim);
        
        /*
         Returns the point p rotated by angle theta around the z, y then x axes. Theta should be taken as representing
                 theta = {th_x, th_y, th_z}
         */
        Point3d _rotate(Point3d p,
                        vec theta);
        /*
         Projects the point p onto a plane orthogonal to the x-axis. This is done by drawing a straight line joining the point p and the x-axis at cameraDist. The plane is taken to be a fixed amount in front of the camera (currently 1). Usually, cameraDist is taken as negative.
         */
        Point2d _project(Point3d p,
                         double cameraDist);
        /* 
         Rotates the point p through angles theta around the z, y, then x axes. Then projects the point using the function above. Theta should still be taken as representing
                 theta = {th_x, th_y, th_z},
             the rotations around the x, y, and z axes respectively.
         */
        Point2d _rotateThenProject(Point3d p,
                                   vec theta,
                                   double cameraDist);
        
        /* 
         Takes in a parameter vector and assigns the parts to theta, cameraDist, scale and p. All but params get changed by calling the function. 
         
         Params should represent
             (thetaX, thetaY, thetaZ, cameraDist, scale, centreX, centreY)
         */
        void _extractParams(vec params,
                            vec& theta,
                            double& cameraDist,
                            double& scale,
                            Point2d& p);
    };
    
    /*
     Function object to find the most suitable parameters to fit a cube to user input.
     
     Construct functor with a vector<double> (of length 7) representing user input. The order needs to be be: "central" vertex, top vertex, then around the others in anti-clockwise order. Recall that the y-axis is going down (because in an image), so the top vertex is the one seen with the smallest y-value.
     
     The operator() takes a vector<double> of parameters representing:
        (thetaX, thetaY, thetaZ, cameraDist, scale, centreX, centreY)
     */
    class Objective : VirtualGeom {
    public:
        /*
         Takes in user input. Vector should have length 7. Also constructs the vector of visible vertices in the correct order.
         */
        Objective(std::vector<Point2d> userInput);
        double operator()(vec params);
    private:
        double _squaredDist(int vertIndex, vec params);
        std::vector<Point2d> _observedPoints;
        std::vector<Point3d> _vertices;
    };
    
    /*
     Represents a cube. Hence the name.
     
     Constructed with a vector of parameters representing:
        (thetaX, thetaY, thetaZ, cameraDist, scale, centreX, centreY)
     It should work with the output of gradientDescent on an Objective function object.
     */
    class Cube : VirtualGeom {
    public:
        Cube(vec params);
        std::vector<Point2d> projectPoints();
        vec getParams();
    private:
        /*
         Returns rotated, but not scaled, vertices of the cube. Vertices are the images of those of the unit cube. The order of vertices corresponds to the binary representation of their coordinate. For example, 
                 vertex[3] is the image of (0,1,1), 
                 because 011 bin = 3 dec.
         */
        std::vector<Point3d> _generateVertices();
        vec params;
        vec theta;
        double cameraDist;
        double scale;
        Point2d centre;
        std::vector<Point2d> projected;
        std::vector<Point3d> vertices;
    };
    
} // namespace geom

#endif /* defined(__CubeSorting__Geometry__) */
