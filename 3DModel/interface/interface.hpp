//
//  interface.hpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/15.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef interface_hpp
#define interface_hpp

#include <stdio.h>
#include <Eigen/Dense>
#include <GLUT/glut.h>
#include "model.hpp"
#include "scene.hpp"
#include "scanline.hpp"
#include "texture.hpp"


class interface {
public:

    static int screenWidth;
    static int screenHeight;
    static bool isFirstTimeRun;
    // mouse control variables
    static bool isMouseAction;
    static int mousePosX;
    static int mousePosY;
    // scene must be pointer(cannot be a reference)
    static scene* currentScene;
    static zBufferEngine* engine;

    
    interface(){        }
    void run(scene* scene, zBufferEngine* slzBuffer);
    
    
    static void loop();
    static void reshape(int newWidth, int newHeight);
    static void mouse(int button, int state, int x, int y);
    static void keyboard(int key, int x, int y);
    
};
#endif /* interface_hpp */
