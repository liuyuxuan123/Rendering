////
////  interface.cpp
////  3DModel
////
////  Created by 刘宇轩 on 2019/3/15.
////  Copyright © 2019 yuxuanliu. All rights reserved.
////
//

// Disable all GLUT warning
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

// Disable all EIGEN warning
#ifndef EIGEN_WARNINGS_DISABLED
#define EIGEN_WARNINGS_DISABLED
#endif

#include "interface.hpp"
using namespace std;

// initialize static variable
scene* interface::currentScene = nullptr;
zBufferEngine* interface::engine = nullptr;


int interface::screenWidth = 0;
int interface::screenHeight = 0;
bool interface::isFirstTimeRun = true;

bool interface::isMouseAction = false;
int interface::mousePosX = 0;
int interface::mousePosY = 0;




// manage keyboard event
void interface::keyboard(int key, int x, int y){
    currentScene->onKeyboard(key);
    currentScene->lookAtModels();
    engine->setNeedsUpdate();
    glutPostRedisplay();
}

void interface::reshape(int w, int h){
    //float ratio = screenWidth * 1.0 / screenHeight;
    // Reset Matrix
    glLoadIdentity();
    // Set the viewport to be the entire window
    glViewport(0, 0, screenWidth, screenHeight);
    // Set the correct perspective.
    //  gluPerspective(60.0f, ratio, 0.1f, 100.0f);
    // Get Back to the Modelview
    
    // Do something to the engine
    glMatrixMode(GL_MODELVIEW);
}

/************************************
*         state           state     *
*    0    left click      begin     *
*    1    right click     end       *
*************************************/

// supose its a (640, 640) interface
/*******************
 *(0, 0)           *
 *                 *
 *                 *
 *                 *
 *                 *
 *                 *
 *       (640, 640)*
 *******************/
void interface::mouse(int button, int state, int x, int y){
    if(state == GLUT_DOWN){
        isMouseAction = true;
        mousePosX = x;
        mousePosY = y;
        
    }else if(state == GLUT_UP){
        isMouseAction = false;
        double deltaX = mousePosX - x;
        double deltaY = mousePosY - y;
        mousePosX = 0;
        mousePosY = 0;
        
        currentScene->cameras[currentScene->mainCamera].rotateOnY(deltaY / screenHeight);
        currentScene->cameras[currentScene->mainCamera].rotateOnX(deltaX / screenWidth);
        
        currentScene->lookAtModels();
        glutPostRedisplay();
    }
    
}


// Render direction
/*****************
 *↘︎             *
 *  ↘︎           *
 *    ↘︎         *
 *      ↘︎       *
 *        ↘︎     *
 *          ↘︎   *
 *            ↘︎ *
 *****************/
void interface::loop(){
    // Necessary for MacOS.
    // Has to be different dimensions than in glutInitWindowSize();
    // Or We can delete glutInitWindowSize and do that initialization here
    if(isFirstTimeRun) {
        glutReshapeWindow( screenWidth, screenHeight);
        isFirstTimeRun = false;
    }
    
    
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    //cout << viewport[0] << " " << viewport[1] << " " << viewport[2] << " " << viewport[3] << " " << endl;
  
    
    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT );
    // Reset transformations
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, 0, screenHeight);
    
    // Run scanline algorithm every time
    model currenModel = currentScene->models[currentScene->mainModel];
    //engine->run(currenModel);
    engine->run(*currentScene);
    
    glBegin(GL_POINTS);
    for (int height = 0; height < screenHeight; height ++) {
        for (int width = 0; width < screenWidth; width ++) {

            int redValue = 255;
            int greenValue = 255;
            int blueValue = 255;

            if(engine->iBuffer[height][width]) {
                redValue = engine->rBuffer[height][width] ;
                greenValue = engine->gBuffer[height][width] ;
                blueValue = engine->bBuffer[height][width] ;
            }



            glColor3f(redValue / 255.0, greenValue / 255.0, blueValue / 255.0);
            
//            vector<double> colors = tex.getColor(height, width);
//            glColor3f((colors[0] + 1) / 2, (colors[1] + 1) / 2, (colors[2] + 1) / 2);
            glVertex2i( width, height);

        }
    }
    glEnd();

    //glFinish();
    // GLUT_DOUBLE mode -> using swap buffer to swap front buffer with back buffer 
    //glFlush();
    glutSwapBuffers();
}


// using scene and z-buffer engine to 
void interface::run(scene* scene, zBufferEngine* zBufferEngine){
    
    this->screenWidth = zBufferEngine->screenWidth;
    this->screenHeight = zBufferEngine->screenHeight;
    engine = zBufferEngine;
    currentScene = scene;
    

    // Problem !!!!!
    // glutInit must be called, but its input value don't influnce the result
    char *argv [1];
    int argc = 1;
    argv[0] = strdup("Texture");
    
    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Texture");

    // register callbacks
    glutDisplayFunc(loop);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    glutMouseFunc(mouse);
    
    glutMainLoop();
}



