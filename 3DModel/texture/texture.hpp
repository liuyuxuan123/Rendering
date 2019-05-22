//
//  texture.hpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/4/28.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef texture_hpp
#define texture_hpp

#include <stdio.h>
#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <list>
#include <math.h>
#include <GLUT/GLUT.h>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include "pngImage.h"


#define PI 3.14159265358979
using namespace std;
using namespace Eigen;
class texture {
private:
    // store the color of each pixel in the texture
    PngImage* image = NULL;
    vector<vector<double>> noise; // the random number used for perlin noise
    vector<vector<double>> turbulance; // the turbulance used ofr perlin noise
    // 0 = horizontal turbulance
    // 1 = vertical turbulance
    // 2 = both turbulance
    void setTurbulance(){
        
        turbulance.resize(getHeight());
        for(int i = 0; i < turbulance.size(); i++){
            turbulance[i].resize(getWidth());
            for(int j = 0; j < turbulance[i].size(); j++){
                
                
                for(int k = 1; k <= getWidth(); k *= 2){
                    turbulance[i][j] += (noise[(i * k) % (noise.size())][(j * k)%(noise[i].size())]/k);
                }
                
            }
        }
        
    }
public:
    
    vector<vector<vector<double>>> teximage;
    texture(){
        
    }
    
    texture(int height, int width){
        // initialize the texture image
        teximage.resize(height);
        for(int i=0; i < teximage.size(); i++){
            teximage[i].resize(width);
            for(int j=0; j < teximage[i].size(); j++){
                teximage[i][j].resize(3);
            }
        }
        srand(time(NULL));
        // initialize the noise
        noise.resize(height * 4);
        for(int i = 0; i < noise.size(); i++){
            noise[i].resize(width * 4);
            for(int j = 0; j < noise[i].size(); j++){
                noise[i][j]=(rand() % 100) / 100.0;
            }
        }
    }
    

    // get the color using the texture coordinates directly
    Vector3f getColor(int x, int y){
        Vector3f color;
        
        x = min(x, getHeight() - 1);
        x = max(x, 0);
        y = min(y, getWidth() - 1);
        y = max(y, 0);
        
        color[0] = teximage[x][y][0];
        color[1] = teximage[x][y][1];
        color[2] = teximage[x][y][2];
        
        return color;
    }

    
    // float to int
    int getHeight(){
        return (int)teximage.size();
    }
    int getWidth(){
        return teximage.size() > 0? (int)teximage[0].size() : 0;
    }
    
    void perlin(){
        setTurbulance();
        for(int i = 0; i < teximage.size(); i ++){
            for(int j = 0; j < teximage[i].size(); j ++){
                teximage[i][j][0] = sin((i + j + 100 / turbulance[i][j]) / 30);
                teximage[i][j][1] = sin((i + j + 100 / turbulance[i][j]) / 30);
                teximage[i][j][2] = sin((i + j + 100 / turbulance[i][j]) / 30);
            }
        }
    }
    
    
    void imageTexture(){
        image = new PngImage("texture/grass.png");
    }
    
    Vector3f getImageColor(int x, int y){
        Vector3f color;
        
        x = min(x, getHeight() - 1);
        x = max(x, 0);
        y = min(y, getWidth() - 1);
        y = max(y, 0);
        
        int red;
        int green;
        int blue;
        image->getRGB(x, y, red, green, blue);
        color[0] = red;
        color[1] = green;
        color[2] = blue;
      
        return color;
    }
    
};

#endif /* texture_hpp */
