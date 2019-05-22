//
//  shader.cpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/14.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#include "shader.hpp"
#include "illumination.hpp"

// Keep original randomly generated color
void shader::shade(model &model){
    // Do nothing
    // Using random color generated for each face
    // this color is generated when load model

}

void shader::shade(scene& scene){
    for(auto& model: scene.models){
        this->shade(model.second);
    }
}

void shader::gouroundShading(model& model){

    illumination phongIllumination = illumination(model);
    
    vector<Vector3f> faceNormals(model.displayFaces.size(), Vector3f(0, 0, 0));
    vector<Vector3f> vertexNormals(model.modelMatrix.cols(), Vector3f(0, 0, 0));

    for(int i = 0; i < model.displayFaces.size(); i ++){
        vector<int> face = model.displayFaces[i];
        Vector3f vertexA = Vector3f(model.modelMatrix(0, face[1]),
                                    model.modelMatrix(1, face[1]),
                                    model.modelMatrix(2, face[1]));
        Vector3f vertexB = Vector3f(model.modelMatrix(0, face[2]),
                                    model.modelMatrix(1, face[2]),
                                    model.modelMatrix(2, face[2]));
        Vector3f vertexC = Vector3f(model.modelMatrix(0, face[3]),
                                    model.modelMatrix(1, face[3]),
                                    model.modelMatrix(2, face[3]));
        Vector3f faceNormal = (vertexC - vertexB).cross(vertexB - vertexA);
        faceNormals[i] = faceNormal.normalized();
    }
    for(int i = 0; i < model.displayFaces.size(); i ++){
        for(int j = 1; j < model.displayFaces[i].size(); j ++){
            vertexNormals[model.displayFaces[i][j]] += faceNormals[i];
        }
    }
    for(int i = 0; i < vertexNormals.size(); i ++){
        
        vertexNormals[i].normalize();
        Vector3f point = Vector3f(model.modelMatrix(0, i),
                                  model.modelMatrix(1, i),
                                  model.modelMatrix(2, i));
        Vector3f color = phongIllumination.phongIllumination(point, vertexNormals[i], Vector3f(1, 0.5, 5));
//        model.displayColors[i][0] = static_cast<int>(color[0] * 255);
//        model.displayColors[i][1] = static_cast<int>(color[1] * 255);
//        model.displayColors[i][2] = static_cast<int>(color[2] * 255);
        
//        model.displayColors[i][0] *= color[0];
//        model.displayColors[i][1] *= color[1];
//        model.displayColors[i][2] *= color[2];
        
        vertexNormals[i] = color * 255;
        //cout << color << endl;
    }
    model.vertexColors = vertexNormals;
}



void shader::phongShading(model& model){
    vector<Vector3f> faceNormals(model.displayFaces.size(), Vector3f(0, 0, 0));
    vector<Vector3f> vertexNormals(model.modelMatrix.cols(), Vector3f(0, 0, 0));
    
    for(int i = 0; i < model.displayFaces.size(); i ++){
        vector<int> face = model.displayFaces[i];
        Vector3f vertexA = Vector3f(model.modelMatrix(0, face[1]),
                                    model.modelMatrix(1, face[1]),
                                    model.modelMatrix(2, face[1]));
        Vector3f vertexB = Vector3f(model.modelMatrix(0, face[2]),
                                    model.modelMatrix(1, face[2]),
                                    model.modelMatrix(2, face[2]));
        Vector3f vertexC = Vector3f(model.modelMatrix(0, face[3]),
                                    model.modelMatrix(1, face[3]),
                                    model.modelMatrix(2, face[3]));
        Vector3f faceNormal = (vertexC - vertexB).cross(vertexB - vertexA);
        faceNormals[i] = faceNormal.normalized();
    }
    for(int i = 0; i < model.displayFaces.size(); i ++){
        for(int j = 1; j < model.displayFaces[i].size(); j ++){
            vertexNormals[model.displayFaces[i][j]] += faceNormals[i];
        }
    }
    for(int i = 0;i < vertexNormals.size(); i ++){
        vertexNormals[i].normalize();
    }
    model.vertexNormals = vertexNormals;
}

