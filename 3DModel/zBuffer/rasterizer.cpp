//
//  rasterizer.cpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/14.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#include "rasterizer.hpp"
using namespace std;
rasterizer::rasterizer(){
//    this->width = 640;
//    this->height = 640;
}

void rasterizer::setSize(int width, int height){
    this->width = width;
    this->height = height;
}

void rasterizer::getSize(int &width, int &height){
    width = this->width;
    height = this->height;
}

void rasterizer::rasterize(model &model){
    
    double xScale = this->width / 2;
    double yScale = this->height / 2;
    double zScale = 1;      // Keep original z value 
    
    model.rasterizedMatrix.resize(3, model.displayMatrix.cols());
//    double maxX = INT_MIN;
//    double minX = INT_MAX;
//    double maxY = INT_MIN;
//    double minY = INT_MAX;
    for(int i = 0; i < model.displayMatrix.cols(); i ++){
        
        model.rasterizedMatrix(0, i) = model.displayMatrix(0, i) * xScale + xScale;
        model.rasterizedMatrix(1, i) = model.displayMatrix(1, i) * yScale + yScale;
        model.rasterizedMatrix(2, i) = model.displayMatrix(2, i) * zScale;
        
        assert(model.rasterizedMatrix(0, i) >= 0);
        assert(model.rasterizedMatrix(1, i) >= 0);
        
//        maxX = model.rasterizedMatrix(0, i) > maxX ? model.rasterizedMatrix(0, i) : maxX;
//        maxY = model.rasterizedMatrix(1, i) > maxY ? model.rasterizedMatrix(1, i) : maxY;
//        minX = model.rasterizedMatrix(0, i) < minX ? model.rasterizedMatrix(0, i) : minX;
//        minY = model.rasterizedMatrix(1, i) < minY ? model.rasterizedMatrix(1, i) : minY;
        
//        maxX = max(maxX, model.rasterizedMatrix(0, i));
//        maxY = max(maxY, model.rasterizedMatrix(1, i));
//        minX = min(minX, model.rasterizedMatrix(0, i));
//        minY = min(minY, model.rasterizedMatrix(1, i));
    }
    
//    cout << "maxX: " << maxX << endl;
//    cout << "maxY: " << maxY << endl;
//    cout << "minX: " << minX << endl;
//    cout << "minY: " << minY << endl;
}

Vector3f rasterizer::inverseRasterize(Vector3f rasterizedPos){
    Vector3f inversedPos = Vector3f(0, 0, 0);
    double xScale = this->width / 2;
    double yScale = this->height / 2;
    double zScale = 1;
    
    inversedPos[0] = (rasterizedPos[0] - xScale) / xScale;
    inversedPos[1] = (rasterizedPos[1] - yScale) / yScale;
    inversedPos[2] = rasterizedPos[2] / zScale;
    return inversedPos;
}
