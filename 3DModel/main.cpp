// Disable all GLUT warning
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

// Disable all EIGEN warning
#ifndef EIGEN_WARNINGS_DISABLED
#define EIGEN_WARNINGS_DISABLED
#endif

#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <Eigen/Dense>
#include "utility/utility.hpp"
#include "utility/transform.hpp"
#include "utility/camera.hpp"
#include "utility/scene.hpp"
#include "zBuffer/scanline.hpp"
#include "interface/interface.hpp"
#include "texture/texture.hpp"
using namespace std;

int main(int argc, char **argv){


    scene displayScene = scene();
    
    
    Vector3f cameraPosition(1, 0.5, 2);
    Vector3f referencePosition(0, 0, 0);
    camera localCamera = camera(cameraPosition, referencePosition, M_PI / 4, 1.0, 2, 20);
    
    // Insert model into scene at specific position
    // local coordinate -> world coordinate
    Vector3f modelPosition1 = Vector3f(0, 0.0, 0.0);
    displayScene.insertModelAtPosition("better-ball.d.txt", modelPosition1);
    
    //Vector3f carPosition2 = Vector3f(0.12, 0.12, 0.3);
    //displayScene.insertModelAtPosition("car.d.txt", carPosition2);
    
    // Insert camera into scene with a specific camera name
    // A scene can have multiple cameras
    // the display view, however, is the view in the main camera
    displayScene.insertCameraWithName("localCamera", localCamera);
    // lookAtModels -->
    // perform lookAtModels function to perform transformation matrix to the model
    displayScene.lookAtModels();
    // Display camera basic infomation
    displayScene.cameras[displayScene.mainCamera].displayCameraSettings();
    
    rasterizer localRasterizer = rasterizer();
    shader constantShader = shader();
    
    // Setup scan line conversion engine
    // using rasterizer to rasterize the scene
    // using shader to determine color of a face
    // Attentions:
    //    this architecture work only for constant shading and gouraund shading
    zBufferEngine engine = zBufferEngine(localRasterizer, constantShader);
    engine.setSize(640, 640);
    
    // Setup interface class
    interface dispalyInterface = interface();
    dispalyInterface.run(&displayScene, &engine);
    
    return 1;
}


