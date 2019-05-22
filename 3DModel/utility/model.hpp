//
//  model.hpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/12.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef model_hpp
#define model_hpp

#include "utility.hpp"
#include <stdio.h>
#include <iostream>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;

class model{
public:
    string modelName;
    
    MatrixXf modelMatrix;
    MatrixXf displayMatrix;
    MatrixXf rasterizedMatrix;
    
    vector<vector<int>> faces;
    vector<vector<int>> displayFaces;
    vector<vector<int>> colors;
    vector<vector<int>> displayColors;
    vector<vector<double>> textureColors;
    
    vector<Vector3f> vertexColors;
    vector<Vector3f> vertexNormals;
    
    Vector3f center;
    double r;
    
    model(){    }
    
    model(string fileName){
        vector<vector<double>> vertexes;
        loadModel(fileName, vertexes, faces, colors);
        loadMatrix(vertexes, modelMatrix);

        displayFaces = faces;
        displayColors = colors;
    }
    
    void applyModelTransformMatrix(Matrix4f mat){
        modelMatrix = mat * modelMatrix;
    }
    
    void loadTexture() {
        double minX = INT_MAX;
        double minY = INT_MAX;
        double minZ = INT_MAX;
        double maxX = INT_MIN;
        double maxY = INT_MIN;
        double maxZ = INT_MIN;
        
        double centerX;
        double centerY;
        double centerZ;
        
        MatrixXf mat = modelMatrix;
        int cols = (int)mat.cols();
        for(int i = 0; i < cols; i ++){
            double currentX = mat(0, i);
            double currentY = mat(1, i);
            double currentZ = mat(2, i);
            
            minX = min(minX, currentX);
            minY = min(minY, currentY);
            minZ = min(minZ, currentZ);
            maxX = max(maxX, currentX);
            maxY = max(maxY, currentY);
            maxZ = max(maxZ, currentZ);
        }
        
        centerX = (minX + maxX) / 2;
        centerY = (minY + maxY) / 2;
        centerZ = (minZ + maxZ) / 2;
        center = Vector3f(centerX, centerY, centerZ);
        
        for(int i = 0; i < cols; i ++){
            double currentX = mat(0, i);
            double currentY = mat(1, i);
            double currentZ = mat(2, i);
            
            double distance = pow(currentX - centerX, 2) + pow(currentY - centerY, 2) + pow(currentZ - centerZ, 2);
            if(distance > r * r){
                r = sqrt(distance);
            }
        }
    }
};

#endif /* model_hpp */
