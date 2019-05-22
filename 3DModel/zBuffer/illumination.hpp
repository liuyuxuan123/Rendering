//
//  illumination.hpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/29.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef illumination_hpp
#define illumination_hpp


#include <stdio.h>
#include <Eigen/Dense>
#include <vector>
#include <cmath>
#include "model.hpp"
#include "scene.hpp"
#include "texture.hpp"
#include "textureMapping.hpp"

using namespace std;
using namespace Eigen;
class illumination{
public:
    
    string lightName;
    Vector3f specularLightPos;
    Vector3f diffuseLightPos;
    
    Vector3f specularLight;
    Vector3f diffuseLight;
    texture textureImage;
    textureMapping textureGenerate;
    
    // These two parameters are supposed to be a model's paramete;
    double kd;
    double ks;
    int n;
    
    
    illumination(model& model){
        this->specularLightPos = Vector3f(1.0, 0.5, 5.0);
        this->specularLight =    Vector3f(1.0, 1.0, 1.0); // Default Red Specular Light
        this->diffuseLightPos =  Vector3f(1.0, 0.5, 5.0);
        this->diffuseLight =     Vector3f(1.0, 0.0, 0.0);     // Default White Specular Light
        
        this->kd = 0.8;
        this->ks = 0.2;
        this->n = 10;
        
        // texture size is the texture image size
        this->textureImage = texture(640, 640);
        this->textureImage.perlin();
        this->textureImage.imageTexture();
        //this->textureGenerate = textureMapping(model);
        
    }
    
//    illumination(Vector3f specularLightPosition, Vector3f diffuseLightPosition, Vector3f specularLightColor, Vector3f diffuseLightColor, double diffuseReflectivity, double specularReflectivity, int shininessPower){
//        
//        this->specularLightPos = specularLightPosition;
//        this->specularLight = specularLightColor;
//        this->diffuseLightPos = diffuseLightPosition;
//        this->diffuseLight = specularLightColor;
//        
//        this->kd = diffuseReflectivity;
//        this->ks = specularReflectivity;
//        this->n = shininessPower;
//        
//        this->textureImage = texture(640, 640);
//        this->textureImage.perlin();
//        //this->textureMapping = textureMapping();
//    }
    int count = 0;
    
    
    Vector3f phongIllumination(Vector3f position, Vector3f normal, Vector3f cameraPosition){
        
        Vector3f DiffuseColor;
        Vector3f SpecularColor;
        
        double DiffuseFactor;
        double SpecularFactor;
        
        Vector3f N = normal;
        Vector3f L = diffuseLightPos - position;
        N.normalize();
        L.normalize();
        
        double diffuseFactor = N.dot(L);
        if(diffuseFactor > 0){
            DiffuseColor = kd * diffuseLight * diffuseFactor;
            DiffuseFactor = kd * diffuseFactor;
        }else{
            DiffuseColor = Vector3f(0, 0, 0);
            DiffuseFactor = 0;
        }
        
        Vector3f V = cameraPosition - position;
        V.normalize();
        Vector3f R = 2 * N * (N.dot(L)) - L;
        R.normalize();
        
        double specularFactor = V.dot(R);
        if(specularFactor > 0) {
            SpecularColor = ks * specularLight * pow(specularFactor, n);
            SpecularFactor = ks * pow(specularFactor, n);
        }else{
            SpecularColor = Vector3f(0, 0, 0);
            SpecularFactor = 0;
        }
        
        vector<double> uv = textureGenerate.getTextureCoordinate(position, normal);
//        Vector3f colors = textureImage.getColor(uv[0] * 640, uv[1] * 640);
        Vector3f colors = textureImage.getImageColor(uv[0] * 640, uv[1] * 640);
        colors /= 255;
        return (DiffuseFactor + SpecularFactor) * colors;
        //cout << "Specular : " << SpecularColor << endl;
        //return DiffuseColor + SpecularColor;
    }
    
    double phongIlluminationIntensity(Vector3f position, Vector3f normal, Vector3f cameraPosition){

        
        double DiffuseFactor;
        double SpecularFactor;
        
        Vector3f N = normal;
        Vector3f L = diffuseLightPos - position;
        N.normalize();
        L.normalize();
        
        double diffuseFactor = N.dot(L);
        if(diffuseFactor > 0){
            DiffuseFactor = kd * diffuseFactor;
        }else{
            DiffuseFactor = 0;
        }
        
        Vector3f V = cameraPosition - position;
        V.normalize();
        Vector3f R = 2 * N * (N.dot(L)) - L;
        R.normalize();
        
        double specularFactor = V.dot(R);
        if(specularFactor > 0) {
            SpecularFactor = ks * pow(specularFactor, n);
        }else{
            SpecularFactor = 0;
        }

        return (DiffuseFactor + SpecularFactor);
    }

};


#endif /* illumination_hpp */
