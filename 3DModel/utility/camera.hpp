//
//  camera.hpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/11.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include <stdio.h>
#include "transform.hpp"
#include "model.hpp"
#include <Eigen/Dense>

class camera {
public:
    
    // Clipping Plane Distance
    double zNear;
    double zFar;
    double FOV;
    double aspect;
    
    // Camera Position in World Coordinate
    Vector3f cameraPos;
    Vector3f refPos;
    
    // Camera Position in World Coordinate
    Vector3f cameraDir;
    Vector3f cameraUp;
    Vector3f cameraRight;
    
    Matrix4f viewMatrix;
    Matrix4f perspectiveMatrix;
    double stepSize = 0.1;
    
    camera(){   }
    camera(Vector3f cameraPosition, Vector3f refPosition, double fieldOfView, double screenRatio, double nearClippingPlane, double farClippingPlane ){
       
        this->FOV = fieldOfView;
        aspect = screenRatio;
        zNear = nearClippingPlane;
        zFar = farClippingPlane;
    
        cameraPos = cameraPosition;
        refPos = refPosition;
        cameraDir = refPosition - cameraPosition;
        
        Vector3f normalY = Vector3f(0, 1, 0);
        cameraRight = cameraDir.cross(normalY);
        cameraRight.normalize();
        cameraUp = cameraRight.cross(cameraDir);
    }
    
    void displayCameraSettings();
    void onKeyboard(int key);
    void lookAt(model&);
    
    void rotateOnY(double delta);
    void rotateOnX(double delta);
private:
    
};




#endif /* camera_hpp */
