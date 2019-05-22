//
//  textureMapping.hpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/4/29.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef textureMapping_hpp
#define textureMapping_hpp

#include <stdio.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;
class textureMapping {
private:
    
    double minX = INT_MAX;
    double minY = INT_MAX;
    double minZ = INT_MAX;
    double maxX = INT_MIN;
    double maxY = INT_MIN;
    double maxZ = INT_MIN;

    double centerX;
    double centerY;
    double centerZ;
    
    double height;
    double radius;
    
    double maxU = INT_MIN;
    double maxV = INT_MIN;
    double minU = INT_MAX;
    double minV = INT_MAX;
    
public:
    textureMapping(){
        
    }
    
//    textureMapping(model& model){
//        double minX = INT_MAX;
//        double minY = INT_MAX;
//        double minZ = INT_MAX;
//        double maxX = INT_MIN;
//        double maxY = INT_MIN;
//        double maxZ = INT_MIN;
//
//        for(int i = 0; i < model.modelMatrix.cols(); i ++){
//
//            double currentX = model.modelMatrix(0, i);
//            double currentY = model.modelMatrix(1, i);
//            double currentZ = model.modelMatrix(2, i);
//
//            minX = min(minX, currentX);
//            minY = min(minY, currentY);
//            minZ = min(minZ, currentZ);
//            maxX = max(maxX, currentX);
//            maxY = max(maxY, currentY);
//            maxZ = max(maxZ, currentZ);
//
//        }
//        this->centerX = (minX + maxX) / 2;
//        this->centerY = (minY + maxY) / 2;
//        this->centerZ = (minZ + maxZ) / 2;
//
//        double r = 0;
//        for(int i = 0; i < model.modelMatrix.cols(); i ++){
//
//            double currentX = model.modelMatrix(0, i);
//            double currentY = model.modelMatrix(1, i);
//            double currentZ = model.modelMatrix(2, i);
//
//            double distance = pow(currentX - centerX, 2) + pow(currentY - centerY, 2) + pow(currentZ - centerZ, 2);
//            if(distance > r * r){
//                r = sqrt(distance);
//            }
//        }
//
//        this->height = maxZ - minZ;
//        //this->radius = max(max(maxX - minX, maxY - minY), maxZ - minZ)/ 2;
//
//        //cout <<  "u: "<< maxX << " " << minX << " " << maxY << " " << minY << " " << maxZ << " " << minZ << endl;
//
//        this->radius = r;
//    }
    
    // Sphere
    vector<double> getTextureCoordinate(Vector3f surfacePosition, Vector3f surfaceNormal){
        
        vector<double> coordinate(2, 0);
        //Vector3f intermediateSurfacePosition = surfacePosition;
        Vector3f intermediateSurfacePosition = surfaceNormal;

//        vec3 oc = r.origin() - center;
//        float a = dot(r.direction(), r.direction());
//        float b = dot(oc, r.direction());
//        float c = dot(oc, oc) - radius*radius;
//        float discriminant = b*b - a*c;
//        if (discriminant > 0) {
//            float temp = (-b - sqrt(discriminant))/a;
//            if (temp < t_max && temp > t_min) {
//                rec.t = temp;
//                rec.p = r.point_at_parameter(rec.t);
//                rec.normal = (rec.p - center) / radius;
//                rec.mat_ptr = mat_ptr;
//                return true;
//            }
//            temp = (-b + sqrt(discriminant)) / a;
//            if (temp < t_max && temp > t_min) {
//                rec.t = temp;
//                rec.p = r.point_at_parameter(rec.t);
//                rec.normal = (rec.p - center) / radius;
//                rec.mat_ptr = mat_ptr;
//                return true;
//            }
//        }
        
        
        intermediateSurfacePosition.normalize();

        double v = acos(intermediateSurfacePosition[1]);
        double u = acos(intermediateSurfacePosition[0] / sin(v));
  
//        coordinate[0] = (u + M_PI / 2) / M_PI;
//        coordinate[1] = v / M_PI;

//        coordinate[0] = (u - 0.755609)/ 1.630371;
//        coordinate[1] = (v - 0.755609)/ 1.630371;
        
        coordinate[0] = u / M_PI;
        coordinate[1] = v / M_PI;

        maxU = max(coordinate[0], maxU);
        minU = min(coordinate[0], minU);
        maxV = max(coordinate[1], maxV);
        minV = min(coordinate[1], minV);
        return coordinate;
        
    }
    
    
    
};

#endif /* textureMapping_hpp */
