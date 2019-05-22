//
//  rasterizer.hpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/14.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef rasterizer_hpp
#define rasterizer_hpp

#include <stdio.h>
#include <algorithm>
#include <Eigen/Dense>
#include "model.hpp"
#include <cmath>
#include <algorithm>

using namespace std;
using namespace Eigen;

class rasterizer {
public:
    // Display window's size
    // !!! Attention !!!
    // Window's size must be integer
    // And based on main.cpp -> default value is 640 * 640
    int width;
    int height;
    
    rasterizer();
    // Using reference to get width and height
    void setSize(int width, int height);
    void getSize(int& width, int& height);
    void rasterize(model& model);
    Vector3f inverseRasterize(Vector3f);
    
};

#endif /* rasterizer_hpp */
