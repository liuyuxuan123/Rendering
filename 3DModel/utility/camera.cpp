//
//  camera.cpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/11.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#include "camera.hpp"

/*Display camera's setting
 ******************************************************
 *    Camera Settings                                 *
 *    Camera Postion         : (-1.21,  0.00,  1.07)  *
 *    Camera Direction       : ( 1.21,  0.00, -1.07)  *
 *    Filed of View          : 45.00                  *
 *    Width Height Ratio     :  1.00                  *
 *    Near Clipping Plane    :  2.00                  *
 *    Far Clipping Plane     : 20.00                  *
 *******************************************************/
void  camera::displayCameraSettings(){
    
    double x = cameraPos[0];
    double y = cameraPos[1];
    double z = cameraPos[2];
    double dx = cameraDir[0];
    double dy = cameraDir[1];
    double dz = cameraDir[2];
    
    printf("\n******************************************************\n");
    printf("*    Camera Settings                                 *\n");
    printf("*    Camera Postion         : (%5.2f, %5.2f, %5.2f)  *\n", x, y, z);
    printf("*    Camera Direction       : (%5.2f, %5.2f, %5.2f)  *\n", dx, dy, dz);
    printf("*    Filed of View          : %5.2f                  *\n", FOV / M_PI * 180);
    printf("*    Width Height Ratio     : %5.2f                  *\n", aspect);
    printf("*    Near Clipping Plane    : %5.2f                  *\n", zNear);
    printf("*    Far Clipping Plane     : %5.2f                  *\n", zFar);
    printf("******************************************************\n\n");
    
}

// According to camera's current position to recalculate model's display matrix
void camera::lookAt(model& currentModel){
    viewMatrix = viewTransformMatrix(cameraPos[0], cameraPos[1], cameraPos[2], cameraDir[0], cameraDir[1], cameraDir[2]);
    perspectiveMatrix = perspectiveTransformMatrix(FOV, aspect, zNear, zFar);
    
    currentModel.displayMatrix = perspectiveMatrix * viewMatrix * currentModel.modelMatrix;
    // Divide each row by last row
    currentModel.displayMatrix.array().rowwise() /= currentModel.displayMatrix.row(3).array();
}

// According to pressed keyboard to change camera's new position and camera's new direction
void camera::onKeyboard(int key){
    
    switch (key){
        case GLUT_KEY_UP:                       // Directional Key ⤒
            cameraPos += (cameraDir * stepSize); break;
        case GLUT_KEY_DOWN:                     // Directional Key ⤓
            cameraPos -= (cameraDir * stepSize); break;
        case GLUT_KEY_LEFT:                     // Directional Key ⇤
            cameraPos -= (cameraRight * stepSize); break;
        case GLUT_KEY_RIGHT:                    // Directional Key ⇥
            cameraPos += (cameraRight * stepSize); break;
    }
    
    cameraDir = refPos - cameraPos;
    Vector3f normalY = Vector3f(0, 1, 0);
    cameraRight = cameraDir.cross(normalY);
    cameraRight.normalize();
    cameraUp = cameraRight.cross(cameraDir);
    
}


void camera::rotateOnY(double delta){
    
    Matrix3f rotateMat;
    
    // 1 -> FOV
    // degree = delta * FOV
    delta = delta * FOV  / 180 * M_PI  ;
    delta *= 2;
    
    double nx = cameraRight[0];
    double ny = cameraRight[1];
    double nz = cameraRight[2];
    
    rotateMat << nx * nx * (1 - cos(delta)) + cos(delta),
                 nx * ny * (1 - cos(delta)) + nz * sin(delta),
                 nx * nz * (1 - cos(delta)) - ny * sin(delta),
                 nx * ny * (1 - cos(delta)) - nz * sin(delta),
                 ny * ny * (1 - cos(delta)) + cos(delta),
                 ny * nz * (1 - cos(delta)) + nx * sin(delta),
                 nx * nz * (1 - cos(delta)) + ny * sin(delta),
                 ny * nz * (1 - cos(delta)) - nx * sin(delta),
                 nz * nz * (1 - cos(delta)) + cos(delta);
    cameraDir = rotateMat * cameraDir;
    refPos = cameraPos + cameraDir;
    Vector3f normalY = Vector3f(0, 1, 0);
    cameraRight = cameraDir.cross(normalY);
    cameraRight.normalize();
    cameraUp = cameraRight.cross(cameraDir);
}

void camera::rotateOnX(double delta){
    Matrix3f rotateMat;
    delta = delta * FOV / 180 * M_PI ;
    delta *= 2;
    
    double nx = cameraUp[0];
    double ny = cameraUp[1];
    double nz = cameraUp[2];
    
    rotateMat << nx * nx * (1 - cos(delta)) + cos(delta),
    nx * ny * (1 - cos(delta)) + nz * sin(delta),
    nx * nz * (1 - cos(delta)) - ny * sin(delta),
    nx * ny * (1 - cos(delta)) - nz * sin(delta),
    ny * ny * (1 - cos(delta)) + cos(delta),
    ny * nz * (1 - cos(delta)) + nx * sin(delta),
    nx * nz * (1 - cos(delta)) + ny * sin(delta),
    ny * nz * (1 - cos(delta)) - nx * sin(delta),
    nz * nz * (1 - cos(delta)) + cos(delta);
    
    cameraDir = rotateMat * cameraDir;
    refPos = cameraPos + cameraDir;
    Vector3f normalY = Vector3f(0, 1, 0);
    cameraRight = cameraDir.cross(normalY);
    cameraRight.normalize();
    cameraUp = cameraRight.cross(cameraDir);
}
