//
//  transform.hpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/9.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef transform_hpp
#define transform_hpp


#include <stdio.h>
#include <math.h>
#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

// Get model transform matrix using model position
Matrix4f modelTransformMatrix(double modelX,
                              double modelY,
                              double modelZ);
// Get view transfrom matrix using camera position and its reference point position(the point that camera point to)
Matrix4f viewTransformMatrix(double cameraX,
                             double cameraY,
                             double cameraZ,
                             double refX,
                             double refY,
                             double refZ);
// Get perspective transform matrix using FOV(field of view), far clipping plane distance, near clipping plane distance
Matrix4f perspectiveTransformMatrix(double FOV,
                                    double aspect,
                                    double zNear,
                                    double zFar);


#endif /* transform_hpp */
