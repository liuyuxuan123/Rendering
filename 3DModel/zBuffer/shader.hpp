//
//  shader.hpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/14.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#include <stdio.h>
#include "model.hpp"
#include "scene.hpp"
#include <Eigen/Dense>
#include <vector>

using namespace std;
using namespace Eigen;
class shader{
public:
    
    string lightName;
    Vector3f specularLightPos;
    Vector3f diffuseLightPos;

    Vector3f specularLight;
    Vector3f diffuseLight;

    // These two parameters are supposed to be a model's paramete;
    double kd;
    double ks;
    int n;
    
    
    shader(){
        this->specularLightPos = Vector3f(1.0, 0.5, 5.0);
        this->specularLight = Vector3f(1.0, 0.0, 0.0); // Default Red Specular Light
        this->diffuseLightPos = Vector3f(1.0, 0.5, 5.0);
        this->diffuseLight = Vector3f(1.0, 1.0, 1.0);     // Default White Specular Light

        this->kd = 1;
        this->ks = 1;
        this->n = 10;
    }
    
    shader(Vector3f specularLightPosition, Vector3f diffuseLightPosition, Vector3f specularLightColor, Vector3f diffuseLightColor, double diffuseReflectivity, double specularReflectivity, int shininessPower){
        
        this->specularLightPos = specularLightPosition;
        this->specularLight = specularLightColor;
        this->diffuseLightPos = diffuseLightPosition;
        this->diffuseLight = specularLightColor;
        
        this->kd = diffuseReflectivity;
        this->ks = specularReflectivity;
        this->n = shininessPower;
    }
    
    void shade(model& model);
    void shade(scene& scene);
    
    void gouroundShading(model& model);
    void gouroundShading(scene& scene);
    
    void phongShading(model& model);
    void phongShading(scene& scene);
    
};
#endif /* shader_hpp */
