//
//  scene.hpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/11.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef scene_hpp
#define scene_hpp

#include <stdio.h>
#include <map>
#include <vector>
#include <iostream>
#include "camera.hpp"
#include "model.hpp"
#include "transform.hpp"
#include <Eigen/Dense>
using namespace std;

class scene {
public:
    // a map where the keys are integers and the values are strings
    map<string, model> models;
    map<string, camera> cameras;
    string mainCamera;
    string mainModel;
    
    scene(){    }
    
    void insertModelAtPosition(string, Vector3f);
    void insertCameraWithName(string, camera);
   
    void backfaceCulling();
    void lookAtModels();
    void onKeyboard(int);
    
    void displayCamerasSettings();
};

#endif /* scene_hpp */
