//
//  utility.hpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/2/18.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef utility_hpp
#define utility_hpp

#include "texture.hpp"
#include <stdio.h>
#include <vector>
#include <GLUT/glut.h>
#include <Eigen/Dense>

const double EPS = 1e-6;

using namespace std;
using namespace Eigen;

// Determine whether two float-point numbers are equal
bool isEqual(float a, float b);
bool isEqual(double a, double b);


// Load file from current directory, store vertexes into "vertexes", store faces into "faces"
void loadModel(string fileName,
               vector<vector<double>>& vertexes,
               vector<vector<int>>& faces,
               vector<vector<int>>& colors);

void loadMatrix(vector<vector<double>>& vertexes,
                MatrixXf& mat);

void loadTexture(MatrixXf& mat, vector<vector<double>>& colors);


void loadVertexes(MatrixXf& mat,
                  vector<vector<double>>& vertexes);

void zBufferAlgo(vector<vector<double>>& vertexes,
                 vector<vector<int>>& faces,
                 vector<vector<int>>& colors,
                 int* zBuffer,
                 int* redBuffer,
                 int* greenBuffer,
                 int* blueBuffer,
                 int screenWidth,
                 int screenHeight);
//
//bool containPoint(vector<vector<double>>)


// According screen size to calculate bitmap 1d array's size
int  convertBitmapSize(int width,
                       int height);

// Convert 2d bitmap to 1d bitmap
void convertBitmap(int* pixels,
                   int width,
                   int height,
                   GLubyte* bitmap);

// Generate a random color component value
float randomColorComponentValue();

// Generate a random color
void randomColor(int* redComponentValue,
                 int* greenComponentValue,
                 int* blueComponentValue);


bool containPoint (double x,
                   double y,
                   double v1x,
                   double v1y,
                   double v2x,
                   double v2y,
                   double v3x,
                   double v3y);

void transform(MatrixXf& modelMatrix,
               double modelPositionX,
               double modelPositionY,
               double modelPositionZ,
               double cameraPositionX,
               double cameraPositionY,
               double cameraPositionZ,
               double cameraDirectionX,
               double cameraDirectionY,
               double cameraDirectionZ,
               double FOV,
               double nearPlaneSize,
               double nearPlaneDistance,
               double farPlaneDistance);

#endif /* utility_hpp */
