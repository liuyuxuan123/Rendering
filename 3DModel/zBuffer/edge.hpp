//
//  edge.hpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/17.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef edge_hpp
#define edge_hpp

#include <stdio.h>
#include <Eigen/Dense>
using namespace Eigen;

class edge {
public:
    // double highPointY;
    // No need to store highPointY
    //         --> edge will be stored in edgeTable(vector) according to its Y value
    double highPointX;
    double highPointZ;
    
    double slopeInverseXY;
    double slopeInverseXZ;
    double slopeInverseYZ;
    
    int spanY;
    int index;
    
    // Goround Shading
    double highPointRed;
    double highPointGreen;
    double highPointBlue;
//
//    double slopeInverseRedY;
//    double slopeInverseBlueY;
//    double slopeInverseGreenY;
    
    Vector3f highPointNormal;
    Vector3f slopeInverseNormalY;
    
    Vector3f highPointColor;
    Vector3f slopeInverseColorY;
    
};

#endif /* edge_hpp */
