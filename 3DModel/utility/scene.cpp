//
//  scene.cpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/11.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#include "scene.hpp"
#include "utility.hpp"

// Place model at world coordinate system using its file's name
void scene::insertModelAtPosition(string fileName, Vector3f position){
    vector<vector<double>> vertexes;

    model currentModel = model(fileName);
    currentModel.loadTexture();
    Matrix4f modelMatrix = modelTransformMatrix(position[0], position[1], position[2]);
    currentModel.applyModelTransformMatrix(modelMatrix);
    
    //savedName --> "car.d.txt"
    //          --> "car"
    //savedName --> "face.d.txt"
    //          --> "face"
    string savedName = fileName.substr(0, fileName.length() - 6);
    int suffix = 1;
    string newSavedName = savedName;
    while(models.count(newSavedName)){
        newSavedName = savedName + to_string(suffix ++);
    }
    currentModel.modelName = newSavedName;
    models[newSavedName] = currentModel;
    
    // Set newest insert model as mainModel
    this->mainModel = newSavedName;
}

// Place camera at world coordinate system
// Camera position is stored in camera
void scene::insertCameraWithName(string cameraName, camera localCamera){
    cameras[cameraName] = localCamera;
    this->mainCamera = cameraName;
    //backfaceCulling();
}

// utility function to perform backface culling
// called in -> insertCameraWithName
//           -> lookAtModels
void scene::backfaceCulling(){
    camera localCamera = cameras[this->mainCamera];
    for(auto model : models){
        vector<vector<int>> backCulledFaces;
        vector<vector<int>> backCulledColors;
        for(int i = 0;i < model.second.faces.size();i ++){
            vector<int> face = model.second.faces[i];
            vector<int> color = model.second.colors[i];
            
            Vector3f point1 = Vector3f(model.second.modelMatrix(0, face[1]),
                                       model.second.modelMatrix(1, face[1]),
                                       model.second.modelMatrix(2, face[1]));
            Vector3f point2 = Vector3f(model.second.modelMatrix(0, face[2]),
                                       model.second.modelMatrix(1, face[2]),
                                       model.second.modelMatrix(2, face[2]));
            Vector3f point3 = Vector3f(model.second.modelMatrix(0, face[3]),
                                       model.second.modelMatrix(1, face[3]),
                                       model.second.modelMatrix(2, face[3]));
            
            Vector3f vec1 = point2 - point1;
            Vector3f vec2 = point3 - point2;
            Vector3f faceNormal = vec2.cross(vec1);
            Vector3f toCamera = localCamera.cameraPos - point2;
            
            //cout << toCamera << endl;
            if(faceNormal.transpose() * toCamera > 0){
                backCulledFaces.push_back(face);
                backCulledColors.push_back(color);
            }
        }
//        models[model.first].displayFaces = backCulledFaces;
//        models[model.first].displayColors = backCulledColors;
    }
}

// let camera look at all the models in the scene
void scene::lookAtModels(){
    
    camera localCamera = cameras[this->mainCamera];
    for(auto model : models){
        localCamera.lookAt(models[model.first]);
    }
    //backfaceCulling();
    
    
}

// Perform keyboard operation to every camera
void scene::onKeyboard(int key){
    cameras[this->mainCamera].onKeyboard(key);
}


// Display all cameras settings in this scene
void scene::displayCamerasSettings(){
    for(auto& camera: cameras){
        cout << camera.first << endl;;
        camera.second.displayCameraSettings();
    }
}
