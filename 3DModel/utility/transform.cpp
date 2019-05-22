//
//  transform.cpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/9.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#include "transform.hpp"

Matrix4f modelTransformMatrix(double modelX, double modelY, double modelZ){
    Matrix4f modelMatrix;
    modelMatrix << 1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, 1, 0,
                   0, 0, 0, 1;
    
    modelMatrix(0, 3) = modelX;
    modelMatrix(1, 3) = modelY;
    modelMatrix(2, 3) = modelZ;
    
//    cout << "Model Transform Matrix" << endl;
//    cout << modelMatrix << endl;
    
    return modelMatrix;
}

Matrix4f viewTransformMatrix(double cameraX, double cameraY, double cameraZ, double dirX, double dirY, double dirZ){
    Matrix4f translateMatrix;
    Matrix4f rotateMatrix;
    
    translateMatrix << 1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1;
    translateMatrix(0, 3) = -cameraX;
    translateMatrix(1, 3) = -cameraY;
    translateMatrix(2, 3) = -cameraZ;
    
    rotateMatrix << 1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1;
    
    double cameraNX = dirX;
    double cameraNY = dirY;
    double cameraNZ = dirZ;
    double cameraNN = sqrt(pow(cameraNX, 2.0) + pow(cameraNY, 2.0) + pow(cameraNZ, 2.0));
    cameraNX /= cameraNN;
    cameraNY /= cameraNN;
    cameraNZ /= cameraNN;
    
    double cameraUX = -cameraNZ;
    double cameraUY = 0;
    double cameraUZ = cameraNX;
    double cameraUN = sqrt(pow(cameraUX, 2.0) + pow(cameraUY, 2.0) + pow(cameraUZ, 2.0));
    
    cameraUX /= cameraUN;
    cameraUY /= cameraUN;
    cameraUZ /= cameraUN;

    double cameraVX = cameraUY * cameraNZ - cameraUZ * cameraNY;
    double cameraVY = cameraUZ * cameraNX - cameraUX * cameraNZ;
    double cameraVZ = cameraUX * cameraNY - cameraUY * cameraNX;
    
    rotateMatrix(0, 0) = cameraUX;
    rotateMatrix(0, 1) = cameraUY;
    rotateMatrix(0, 2) = cameraUZ;
    rotateMatrix(1, 0) = cameraVX;
    rotateMatrix(1, 1) = cameraVY;
    rotateMatrix(1, 2) = cameraVZ;
    rotateMatrix(2, 0) = cameraNX;
    rotateMatrix(2, 1) = cameraNY;
    rotateMatrix(2, 2) = cameraNZ;
    
//    cout << "View Matrix:" << endl;
//    cout << rotateMatrix * translateMatrix << endl;
    
    return rotateMatrix * translateMatrix;
}

Matrix4f perspectiveTransformMatrix(double FOV, double aspect, double zNear, double zFar){

    Matrix4f perspectiveMatrix;
    perspectiveMatrix << 0, 0, 0, 0,
                         0, 0, 0, 0,
                         0, 0, 0, 0,
                         0, 0, 0, 0;

    perspectiveMatrix(0, 0) = 1 / (aspect * tan(FOV / 2));
    perspectiveMatrix(1, 1) = 1 / tan(FOV / 2);
    perspectiveMatrix(2, 2) = zFar / (zFar - zNear);
    perspectiveMatrix(2, 3) = -(zNear * zFar) / (zFar - zNear);
    perspectiveMatrix(3, 2) = 1;
    
//    cout << "Perspective Matrix:" << endl;
//    cout << perspectiveMatrix << endl;
//
    return perspectiveMatrix;
}




