//
//  scanline.hpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/13.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef scanline_hpp
#define scanline_hpp

#include <stdio.h>
#include <vector>
#include <list>
#include <Eigen/Dense>
#include <algorithm>
#include "utility.hpp"
#include "edge.hpp"
#include "polygon.hpp"
#include "model.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "rasterizer.hpp"
#include "illumination.hpp"


using namespace std;


class zBufferEngine {
public:
    zBufferEngine(rasterizer&, shader&);
    ~zBufferEngine();
    
    int screenWidth;
    int screenHeight;
    bool needsUpdate;
    
    rasterizer currentRasterizer;
    shader currentShader;
    
    void setSize(int width, int height);
    void getSize(int& width, int& height);
    void run(model& model);
    void run(scene& scene);
    void setNeedsUpdate();
    
    // iBuffer determines whether this pixel using background color
    bool** iBuffer;
    double** zBuffer;
    int** rBuffer;
    int** gBuffer;
    int** bBuffer;

    vector<list<polygon>> polygonTable;
    vector<list<edge>> edgeTable;
    
    vector<polygon> activePolygonTable;
    void load(  model& model);
};

#endif /* scanline_hpp */
