//
//  utility.cpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/2/18.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#include "utility.hpp"
#include <sstream>
#include <fstream>
#include <cmath>
#include <iostream>


bool isEqual(float a, float b){
    return fabs(a - b) < EPS;
}

bool isEqual(double a, double b){
    return fabs(a - b) < EPS;
}

void loadModel(string fileName, vector<vector<double>>& vertexes, vector<vector<int>>& faces, vector<vector<int>>& colors){

    ifstream modelFile("models/" + fileName);
    if(!modelFile) {
        cout << "Cannot open input file.\n";
        exit(1);
    }

    string line;
    int lineNumber = 0;
    vector<int> fileInfo;

    while (getline(modelFile, line)) {
        istringstream lineBuffer = istringstream(line);
        if(lineNumber == 0) {
            for(string word; lineBuffer >> word; )
                if(word.find("data") == string::npos){
                    fileInfo.push_back(stoi(word));
                }
            
        }else{
            if(lineNumber <= fileInfo[0]){
                vector<double> temp;
                for(string word; lineBuffer >> word; )
                    temp.push_back(stof(word) / 4);
                vertexes.push_back(temp);
            }else{
                vector<int> faceTemp;
                for(string word; lineBuffer >> word; )
                    faceTemp.push_back(stoi(word) - 1);
                faces.push_back(faceTemp);
                
                int red, green, blue;
                vector<int> colorTemp;
                randomColor(&red, &green, &blue);
                
                
//                red = 255;
//                green = 0;
//                blue = 0;
                
                colorTemp.push_back(red);
                colorTemp.push_back(green);
                colorTemp.push_back(blue);
                colors.push_back(colorTemp);
                
            }
        }
        lineNumber ++;
    }
//    cout << "vertex number: " << vertexes.size() << endl;
//    cout << "face number: " << faces.size() << endl;
    modelFile.close();
}


void loadMatrix(vector<vector<double>>& vertexes, MatrixXf& mat){
    // Resize the matrix
    mat.resize(4, vertexes.size());
    for(int i = 0;i < vertexes.size(); i++){
        mat(0, i) = vertexes[i][0];
        mat(1, i) = vertexes[i][1];
        mat(2, i) = vertexes[i][2];
        mat(3, i) = 1;
    }
}

void loadVertexes(MatrixXf& mat, vector<vector<double>>& vertexes){
    for(int i = 0; i < mat.cols(); i++){
        vector<double> temp = vector<double>();
        temp.push_back(mat(0, i));
        temp.push_back(mat(1, i));
        temp.push_back(mat(2, i));
        vertexes.push_back(temp);
    }
}


double sign (double p1x, double p1y, double p2x, double p2y, double p3x, double p3y){
    return (p1x - p3x) * (p2y - p3y) - (p2x - p3x) * (p1y - p3y);
}

bool containPoint (double x, double y, double v1x, double v1y, double v2x, double v2y, double v3x, double v3y){
    float d1, d2, d3;
    bool has_neg, has_pos;
    
    d1 = sign(x, y, v1x, v1y, v2x, v2y);
    d2 = sign(x, y, v2x, v2y, v3x, v3y);
    d3 = sign(x, y, v3x, v3y, v1x, v1y);
    
    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    
    return !(has_neg && has_pos);
}

// Generate a random color component value
float randomColorComponentValue() {
    return rand() % 256 / 255.0;
}

// Generate a random color
void randomColor(int* redComponentValue, int* greenComponentValue, int* blueComponentValue){
    *redComponentValue = rand() % 256;
    *greenComponentValue = rand() % 256;
    *blueComponentValue = rand() % 256;
}

void transform(MatrixXf& modelMatrix, double modelPositionX, double modelPositionY, double modelPositionZ, double cameraPositionX, double cameraPositionY, double cameraPositionZ, double cameraDirectionX, double cameraDirectionY, double cameraDirectionZ, double FOV, double nearPlaneSize, double nearPlaneDistance, double farPlaneDistance){
    Matrix4f modelTransformMatrix;
    Matrix4f viewRotationMatrix;
    Matrix4f viewTranslationMatrix;
    Matrix4f perspectiveTransformMatrix;
    
    modelTransformMatrix << 1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1;
    

    modelTransformMatrix(0, 3) = modelPositionX;
    modelTransformMatrix(1, 3) = modelPositionY;
    modelTransformMatrix(2, 3) = modelPositionZ;
    
    cout << "Model Transform Matrix: " << endl;
    cout << modelTransformMatrix << endl;
    
    Vector3f normVec = Vector3f(0.0, 1.0, 0.0);
    Vector3f cameraDirectionN = Vector3f(cameraDirectionX, cameraDirectionY, cameraDirectionZ);
    cameraDirectionN /= cameraDirectionN.norm();
    
    Vector3f cameraDirectionU = cameraDirectionN.cross(normVec);
    cameraDirectionU /= cameraDirectionU.norm();
    
    Vector3f cameraDirectionV = cameraDirectionU.cross(cameraDirectionN);
    
    viewRotationMatrix << 0, 0, 0, 0,
                          0, 0, 0, 0,
                          0, 0, 0, 0,
                          0, 0, 0, 1;
    
    viewRotationMatrix(0, 0) = cameraDirectionU[0];
    viewRotationMatrix(0, 1) = cameraDirectionU[1];
    viewRotationMatrix(0, 2) = cameraDirectionU[2];
    
    viewRotationMatrix(1, 0) = cameraDirectionV[0];
    viewRotationMatrix(1, 1) = cameraDirectionV[1];
    viewRotationMatrix(1, 2) = cameraDirectionV[2];
    
    viewRotationMatrix(2, 0) = cameraDirectionN[0];
    viewRotationMatrix(2, 1) = cameraDirectionN[1];
    viewRotationMatrix(2, 2) = cameraDirectionN[2];
    
    cout << "Perspective Rotation Matrix:" << endl;
    cout << viewRotationMatrix << endl;
    
    
    viewTranslationMatrix << 1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1;
    viewTranslationMatrix(0, 3) = -cameraPositionX;
    viewTranslationMatrix(1, 3) = -cameraPositionY;
    viewTranslationMatrix(2, 3) = -cameraPositionZ;
    
    cout << "Perspective Translation Matrix:" << endl;
    cout << viewTranslationMatrix << endl;
    
    
    double d = nearPlaneDistance;
    double f = farPlaneDistance;
    double h = nearPlaneSize * tan(FOV * M_PI / 180);
    perspectiveTransformMatrix << 0, 0, 0, 0,
                                  0, 0, 0, 0,
                                  0, 0, 0, 0,
                                  0, 0, 1, 0;
    perspectiveTransformMatrix(0, 0) = d / h;
    perspectiveTransformMatrix(1, 1) = d / h;
    perspectiveTransformMatrix(2, 2) = f / (f - d);
    perspectiveTransformMatrix(2, 3) = - d * f / (f - d);
    
    cout << "Projection Transform Matrix: " << endl;
    cout << perspectiveTransformMatrix << endl;
    
    // modelMatrix = modelTransformMatrix * modelMatrix;
    modelMatrix = modelTransformMatrix * viewRotationMatrix * viewTranslationMatrix * perspectiveTransformMatrix * modelMatrix;
    
}


void loadTexture(MatrixXf& mat, vector<vector<double>>& colors){
    
    double minX = INT_MAX;
    double minY = INT_MAX;
    double minZ = INT_MAX;
    double maxX = INT_MIN;
    double maxY = INT_MIN;
    double maxZ = INT_MIN;
    
    double centerX;
    double centerY;
    double centerZ;
    
    texture perlinTexture = texture(640, 640);
    perlinTexture.perlin();
    
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
    
    double r = 0;
    
    for(int i = 0; i < cols; i ++){
        double currentX = mat(0, i);
        double currentY = mat(1, i);
        double currentZ = mat(2, i);
        
        double distance = pow(currentX - centerX, 2) + pow(currentY - centerY, 2) + pow(currentZ - centerZ, 2);
        if(distance > r * r){
            r = sqrt(distance);
        }
    }
//    double maxU = INT_MIN;
//    double maxV = INT_MIN;
    for(int i = 0; i < cols; i ++){
        double currentX = mat(0, i);
        double currentY = mat(1, i);
        double currentZ = mat(2, i);
    
        Vector3f direction = Vector3f(currentX - centerX, currentY - centerY, currentZ - centerZ);
        double a = direction.dot(direction);
    
        double t = sqrt(pow(r, 2) / a);
        Vector3f mapPosition = t * direction;
        mapPosition.normalize();
        
        double v = acos(mapPosition[1]);
        double u = acos(mapPosition[0] / sin(v));
        u /= M_PI;
        v /= M_PI;
        Vector3f textureColor = perlinTexture.getColor(640 * u, 640 * v);
        vector<double> color = {
            textureColor[0],
            textureColor[1],
            textureColor[2]
        };
        colors.push_back(color);
    }
}


