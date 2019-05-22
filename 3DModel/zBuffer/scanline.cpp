//
//  scanline.cpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/13.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#include "scanline.hpp"
#include <stdio.h>

/*
 1. We will process the polygon edge after edge, and store in the edge Table.
 2. Storing is done by storing the edge in the same scanline edge tuple as the lowermost point's y-coordinate value of the edge.
 3. After addition of any edge in an edge tuple, the tuple is sorted using insertion sort, according to its xofymin value.
 4. After the whole polygon is added to the edge table, the figure is now filled.
 5. Filling is started from the first scanline at the bottom, and continued till the top.
 6. Now the active edge table is taken and the following things are repeated for each scanline:
 i. Copy all edge buckets of the designated scanline to the active edge tuple
 ii. Perform an insertion sort according to the xofymin values
 iii. Remove all edge buckets whose ymax is equal or greater than the scanline
 iv. Fillup pairs of edges in active tuple, if any vertex is got, follow these instructions:
 🏳️ If both lines intersecting at the vertex are on the same side of the scanline, consider it as two points.
 🏳️ If lines intersecting at the vertex are at opposite sides of the scanline, consider it as only one point.
 v. Update the xofymin by adding slopeinverse for each bucket.
 */

zBufferEngine::zBufferEngine(rasterizer& aRasterizer, shader& aShader){
    
    this->zBuffer = NULL;
    this->iBuffer = NULL;
    
    this->rBuffer = NULL;
    this->gBuffer = NULL;
    this->bBuffer = NULL;
    
    this->currentRasterizer = aRasterizer;
    this->currentShader = aShader;
}

zBufferEngine::~zBufferEngine(){
    if (zBuffer != NULL){
        for (int i = 0; i < screenHeight; ++i) {
            delete[] zBuffer[i];
            zBuffer[i] = NULL;
        }
        delete[] zBuffer;
        zBuffer = NULL;
    }
    if (iBuffer != NULL){
        for (int i = 0; i < screenHeight; i++) {
            delete[] iBuffer[i];
            iBuffer[i] = NULL;
        }
        delete[] iBuffer;
        iBuffer = NULL;
    }
    if (rBuffer != NULL){
        for (int i = 0; i < screenHeight; i++) {
            delete[] rBuffer[i];
            rBuffer[i] = NULL;
        }
        delete[] rBuffer;
        rBuffer = NULL;
    }
    
    if (gBuffer != NULL){
        for (int i = 0; i < screenHeight; i ++) {
            delete[] gBuffer[i];
            gBuffer[i] = NULL;
        }
        delete[] gBuffer;
        gBuffer = NULL;
    }
    
    if (bBuffer != NULL){
        for (int i = 0; i < screenHeight; i ++) {
            delete[] bBuffer[i];
            bBuffer[i] = NULL;
        }
        delete[] bBuffer;
        bBuffer = NULL;
    }
    
}

void zBufferEngine::setSize(int width, int height) {
    
    if (width == this->screenWidth && height == this->screenHeight){
        return;
    }
    // If new size is not equal to original size
    // Than we should release all original data
    this->screenWidth = width;
    this->screenHeight = height;
    needsUpdate = true;
    
    if (zBuffer != NULL){
        for (int i = 0; i < screenHeight; i ++) {
            delete[] zBuffer[i];
            zBuffer[i] = NULL;
        }
        delete[] zBuffer;
        zBuffer = NULL;
    }
    if (iBuffer != NULL){
        for (int i = 0; i < screenHeight; i ++) {
            delete[] iBuffer[i];
            iBuffer[i] = NULL;
        }
        delete[] iBuffer;
        iBuffer = NULL;
    }
    if (rBuffer != NULL){
        for (int i = 0; i < screenHeight; i ++) {
            delete[] rBuffer[i];
            rBuffer[i] = NULL;
        }
        delete[] rBuffer;
        rBuffer = NULL;
    }
    
    if (gBuffer != NULL){
        for (int i = 0; i < screenHeight; i ++) {
            delete[] gBuffer[i];
            gBuffer[i] = NULL;
        }
        delete[] gBuffer;
        gBuffer = NULL;
    }
    
    if (bBuffer != NULL){
        for (int i = 0; i < screenHeight; i ++) {
            delete[] bBuffer[i];
            bBuffer[i] = NULL;
        }
        delete[] bBuffer;
        bBuffer = NULL;
    }
    
    
    zBuffer = new double*[height];
    iBuffer = new bool*[height];
    
    rBuffer = new int*[height];
    gBuffer = new int*[height];
    bBuffer = new int*[height];
    
    for (int i = 0; i < height; i ++) {
        zBuffer[i] = new double[width];
        iBuffer[i] = new bool[width];
        
        rBuffer[i] = new int[width];
        gBuffer[i] = new int[width];
        bBuffer[i] = new int[width];
    }
    this->currentRasterizer.setSize(width, height);
}

// get value
void zBufferEngine::getSize(int& width, int& height){
    width = this->screenWidth;
    height = this->screenHeight;
}

void zBufferEngine::setNeedsUpdate(){
    this->needsUpdate = true;
}



//    Why it is " - " (point1[0] - point2[0]) / (point1[1] - point2[1]) ?
//
//    .       <--- highPoint  |          .    <--- highPoint
//     \                      |         /
//      \                     |        /
//       \                    |       /
//        \                   |      /
//         .  <--- lowPoint   |     .         <--- lowPoint
//           dx > 0           |              dx < 0

// set up scanline info
void zBufferEngine::load(model& model){
    
    // clean up polygons and edges information
    polygonTable.clear();
    edgeTable.clear();
    // set up information size
    polygonTable.resize(screenHeight);
    edgeTable.resize(screenHeight);

    // translate every face information into a polygon
    for (int i = 0; i < model.displayFaces.size(); i ++){
        
        // get minY value and maxY value to calculate this polygon's Y span
        // calculate how many scan lines this polygon(triangle) spans
        float minY = INT_MAX;
        float maxY = INT_MIN;
        
        const vector<int> face = model.displayFaces[i];
        polygon polygon;
        polygon.index = i;

        // Plane equation
        // A * x + B * y + C * z + D = 0;
        Vector3f vertexA = Vector3f(model.rasterizedMatrix(0, face[1]),
                                    model.rasterizedMatrix(1, face[1]),
                                    model.rasterizedMatrix(2, face[1]));
        Vector3f vertexB = Vector3f(model.rasterizedMatrix(0, face[2]),
                                    model.rasterizedMatrix(1, face[2]),
                                    model.rasterizedMatrix(2, face[2]));
        Vector3f vertexC = Vector3f(model.rasterizedMatrix(0, face[3]),
                                    model.rasterizedMatrix(1, face[3]),
                                    model.rasterizedMatrix(2, face[3]));
        Vector3f faceNormal = (vertexC - vertexB).cross(vertexB - vertexA);
        // No need to normalize it since
        //faceNormal.normalize();
    
        // Add all the edges of a polygon into edgeTable
        for (int j = 1; j < face.size(); j ++){
            // ~ to do:
            //   utility function to get this Vector3f value
            Vector3f highPoint = Vector3f(model.rasterizedMatrix(0, face[j]),
                                          model.rasterizedMatrix(1, face[j]),
                                          model.rasterizedMatrix(2, face[j]));
            
//            int highPointRedVal =   model.vertexColors[face[j]][0];
//            int highPointGreenVal = model.vertexColors[face[j]][1];
//            int highPointBlueVal =  model.vertexColors[face[j]][2];
//
//            int lowPointRedVal;
//            int lowPointGreenVal;
//            int lowPointBlueVal;
        
            Vector3f highPointColorVal = model.vertexColors[face[j]];
            Vector3f lowPointColorVal;
            Vector3f highPointNormalVal = model.vertexNormals[face[j]];
            Vector3f lowPointNormalVal;
            
            Vector3f lowPoint;
            if(j == face.size() - 1){
                lowPoint = Vector3f(model.rasterizedMatrix(0, face[1]),
                                    model.rasterizedMatrix(1, face[1]),
                                    model.rasterizedMatrix(2, face[1]));
//                lowPointRedVal =   model.vertexColors[face[1]][0];
//                lowPointGreenVal = model.vertexColors[face[1]][1];
//                lowPointBlueVal =  model.vertexColors[face[1]][2];
                lowPointColorVal = model.vertexColors[face[1]];
                lowPointNormalVal = model.vertexNormals[face[1]];
            }else{
                lowPoint = Vector3f(model.rasterizedMatrix(0, face[j + 1]),
                                    model.rasterizedMatrix(1, face[j + 1]),
                                    model.rasterizedMatrix(2, face[j + 1]));
//                lowPointRedVal =   model.vertexColors[face[j + 1]][0];
//                lowPointGreenVal = model.vertexColors[face[j + 1]][1];
//                lowPointBlueVal =  model.vertexColors[face[j + 1]][2];
                lowPointColorVal = model.vertexColors[face[1]];
                lowPointNormalVal = model.vertexNormals[face[j + 1]];
            }
            
            // Guarentee lowPoint is lower than highPoint
            if(lowPoint[1] > highPoint[1]){
                swap(lowPoint, highPoint);
//                swap(lowPointRedVal,   highPointRedVal);
//                swap(lowPointGreenVal, highPointGreenVal);
//                swap(lowPointBlueVal,  highPointBlueVal);
                swap(lowPointColorVal, highPointColorVal);
                swap(lowPointNormalVal,highPointNormalVal);
            }
            
            minY = min(lowPoint[1], minY);
            maxY = max(highPoint[1], maxY);
            // Unnecessary: point1[1] > point2[1]
            // minY = min(point1[1], minY);
            // Unnecessary: point2[1] < point1[1]
            //maxY = max(point2[1], maxY);
            
            // Just in case
            // --> 0 <= Vertex's y value < height
            assert(lowPoint[1] >= 0 );
            assert(round(lowPoint[1]) < screenHeight);
            assert(highPoint[1] >= 0 );
            assert(round(highPoint[1]) < screenHeight);
            
            edge edge;
            edge.spanY = round(highPoint[1]) - round(lowPoint[1]);
            
            if (edge.spanY <= 0){
                continue;
            }
    
            // edge.lowPointX = edge.highPointX + edge.spanY * edge.deltaX
            edge.index = i;
            edge.highPointX    = highPoint[0];
            edge.highPointZ    = highPoint[2];
//            edge.highPointRed  = highPointRedVal;
//            edge.highPointGreen = highPointGreenVal;
//            edge.highPointBlue  = highPointBlueVal;
            edge.highPointColor = highPointColorVal;
            edge.highPointNormal = highPointNormalVal;
            
            edge.slopeInverseXY = -(highPoint[0] - lowPoint[0]) / (highPoint[1] - lowPoint[1]);
//            edge.slopeInverseRedY = -(highPointRedVal - lowPointRedVal) / (highPoint[1] - lowPoint[1]);
//            edge.slopeInverseGreenY = -(highPointGreenVal - lowPointGreenVal) / (highPoint[1] - lowPoint[1]);
//            edge.slopeInverseBlueY = -(highPointBlueVal - lowPointBlueVal) / (highPoint[1] - lowPoint[1]);
            edge.slopeInverseColorY = -(highPointColorVal - lowPointColorVal) / (highPoint[1] - lowPoint[1]);
            edge.slopeInverseNormalY = -(highPointNormalVal - lowPointNormalVal) /(highPoint[1] - lowPoint[1]);
            
            edge.slopeInverseXZ = -(faceNormal[0] / faceNormal[2]);
            edge.slopeInverseYZ = faceNormal[1] / faceNormal[2];
            
            edgeTable[round(highPoint[1])].push_back(edge); // put edge into edge table
        }
        // Attention:
        // spanY is an integer
        // recording the Y-span of the polygon
        polygon.spanY = round(maxY) - round(minY);
        if (polygon.spanY > 0 && minY > 0 && maxY < screenHeight) {
            this->polygonTable[round(maxY)].push_back(polygon);
        }
    }
}

// perform z-buffer algorithm to model
void zBufferEngine::run(model& model){
    
    this->currentRasterizer.rasterize(model);
    //this->currentShader.shade(model);
    this->currentShader.gouroundShading(model);
    this->currentShader.phongShading(model);
    this->activePolygonTable.clear();
    
    texture perlinTexture = texture(640, 640);
    perlinTexture.perlin();
    
    illumination phongIllumination = illumination(model);
    Matrix4f viewMatrix = viewTransformMatrix(1, 0.5, 5.0, -1.0, -0.5, -5.0);
    Matrix4f perspectiveMatrix = perspectiveTransformMatrix(M_PI / 4, 1.0, 2, 20);
    // Here !!
    Matrix4f transformMatrix = perspectiveMatrix * viewMatrix;
    Matrix4f inverseTransformMatrix = transformMatrix.inverse();

    this->load(model);
    for (int height = screenHeight - 1; height >= 0; height -- ) {
        // Move all active polygons into active polygon table
        // for each different height process those polygons
        // Those polygon will only be appended to this table when this polygon's height == current processing height
        for(auto currentPolygon: this->polygonTable[height]){
            this->activePolygonTable.push_back(currentPolygon);
        }
    
        // move edge(s) from ET bucket y whose ymin = y (entering edges) to ATE, maintaining ATE sort order on x
        // Process current active polygons
        for (auto& currentActivePolygon : activePolygonTable) {
            // Add edges that belongs to this polygon to
            for (auto& currentEdge: edgeTable[height]) {
                if (currentEdge.index == currentActivePolygon.index) {
                    currentActivePolygon.edgeTable.push_back(currentEdge);
                    currentEdge.index = -1;
                }
            }
            sort(currentActivePolygon.edgeTable.begin(), currentActivePolygon.edgeTable.end(), [](const edge& lEdge, const edge& rEdge){
                if (round(lEdge.highPointX) < round(rEdge.highPointX)){
                    return true;
                }else if (round(lEdge.highPointX) == round(rEdge.highPointX)){
                    if (round(lEdge.slopeInverseXY) < round(rEdge.slopeInverseXY))
                        return true;
                }
                return false;
            });
            
            vector<edge>& currentEdgeTable = currentActivePolygon.edgeTable;
            for (vector<edge>::iterator activeEdgeIter = currentEdgeTable.begin(), end = currentEdgeTable.end(); activeEdgeIter != end; activeEdgeIter ++ ) {
                edge& edge1 = *activeEdgeIter;
                edge& edge2 = *(++activeEdgeIter);
                
                double depth = edge1.highPointZ;
                
                // Constant Shading
//                Vector3f currentConstantColor = edge1.highPointColor;
//                Vector3f finalConstantColor = edge2.highPointColor;
                
                // Gouraud shading
                
//                Vector3f rasterizedVertexPosition1 = Vector3f(edge1.highPointX, height, edge1.highPointZ);
//                Vector3f rawDisplayVertexPosition1 = currentRasterizer.inverseRasterize(rasterizedVertexPosition1);
//                Vector4f displayVertexPosition1 = Vector4f(rawDisplayVertexPosition1[0], rawDisplayVertexPosition1[1], rawDisplayVertexPosition1[2], 1);
//                Vector4f modelVertexPosition1 = inverseTransformMatrix * displayVertexPosition1;
//                modelVertexPosition1 /= modelVertexPosition1[3];
//                Vector3f currentGouraudColor = phongIllumination.phongIllumination(Vector3f(modelVertexPosition1[0], modelVertexPosition1[1], modelVertexPosition1[2]), currentNormal, Vector3f(1, 0.5, 2));
//                currentGouraudColor *= 255;
//
//                Vector3f rasterizedVertexPosition2 = Vector3f(edge2.highPointX, height, edge2.highPointZ);
//                Vector3f rawDisplayVertexPosition2 = currentRasterizer.inverseRasterize(rasterizedVertexPosition2);
//                Vector4f displayVertexPosition2 = Vector4f(rawDisplayVertexPosition2[0], rawDisplayVertexPosition2[1], rawDisplayVertexPosition2[2], 1);
//                Vector4f modelVertexPosition2 = inverseTransformMatrix * displayVertexPosition2;
//                modelVertexPosition2 /= modelVertexPosition2[3];
//                Vector3f finalGouraudColor = phongIllumination.phongIllumination(Vector3f(modelVertexPosition2[0], modelVertexPosition2[1], modelVertexPosition2[2]), finalNormal, Vector3f(1, 0.5, 2));
//                finalGouraudColor *= 255;
               
                
                // Phong Shading
                Vector3f currentNormal = edge1.highPointNormal;
                Vector3f finalNormal = edge2.highPointNormal;
            
                int spanX = round(edge2.highPointX) - round(edge1.highPointX);
//                Vector3f colorInc = (finalConstantColor - currentConstantColor) / spanX;
//                Vector3f gouraundColorInc = (finalGouraudColor - currentGouraudColor) / spanX;
                Vector3f normalInc = (finalNormal - currentNormal) / spanX;

                
            
//                vector<double> leftTextureColor = model.textureColors[edge1.index];
//                vector<double> rightTextureColor = model.textureColors[ edge2.index];
//                Vector3f currentTextureColor = Vector3f(leftTextureColor[0],
//                                                        leftTextureColor[1],
//                                                        leftTextureColor[2]);
//                Vector3f finalTextureColor = Vector3f(rightTextureColor[0],
//                                                      rightTextureColor[1],
//                                                      rightTextureColor[2]);
//                Vector3f textureColorInc = (finalTextureColor - currentTextureColor) / spanX;
                
                
                
                for (int x = round(edge1.highPointX); x < round(edge2.highPointX); x ++ ){
                    if (depth < zBuffer[height][x]) {
                        zBuffer[height][x] = depth;
                        iBuffer[height][x] = true;

                        
                        Vector3f rasterizedVertexPosition = Vector3f(x, height, depth);
                        Vector3f rawDisplayVertexPosition = currentRasterizer.inverseRasterize(rasterizedVertexPosition);
                       
                        
                        Vector4f displayVertexPosition = Vector4f(rawDisplayVertexPosition[0], rawDisplayVertexPosition[1], rawDisplayVertexPosition[2], 1);
                        Vector4f modelVertexPosition = inverseTransformMatrix * displayVertexPosition;
                        modelVertexPosition /= modelVertexPosition[3];
                        
                        Vector3f currentPhongColor = phongIllumination.phongIllumination(Vector3f(modelVertexPosition[0], modelVertexPosition[1], modelVertexPosition[2]), currentNormal, Vector3f(1, 0.5, 2));
                        //double currentPhoneIntensity = phongIllumination.phongIlluminationIntensity(Vector3f(modelVertexPosition[0], modelVertexPosition[1], modelVertexPosition[2]), currentNormal, Vector3f(1, 0.5, 2));
                        
                        currentPhongColor = currentPhongColor * 255;
                        //currentPhongColor = currentPhoneIntensity * currentTextureColor * 255;
                
                
                        
//                        rBuffer[height][x] = currentConstantColor[0];
//                        gBuffer[height][x] = currentConstantColor[1];
//                        bBuffer[height][x] = currentConstantColor[2];
                        
//                        rBuffer[height][x] = currentGouraudColor[0];
//                        gBuffer[height][x] = currentGouraudColor[1];
//                        bBuffer[height][x] = currentGouraudColor[2];

                        rBuffer[height][x] = currentPhongColor[0];
                        gBuffer[height][x] = currentPhongColor[1];
                        bBuffer[height][x] = currentPhongColor[2];
                    }
                    depth += edge1.slopeInverseXZ;
//                    currentConstantColor += colorInc;
//                    currentGouraudColor += gouraundColorInc;
                    currentNormal += normalInc;
                    //currentTextureColor += textureColorInc;
                    
                }
                
                edge1.spanY --;
                edge2.spanY --;
                edge1.highPointX += edge1.slopeInverseXY;
                edge2.highPointX += edge2.slopeInverseXY;
                
                edge1.highPointColor += edge1.slopeInverseColorY;
                edge2.highPointColor += edge2.slopeInverseColorY;
                
                edge1.highPointNormal += edge1.slopeInverseNormalY;
                edge2.highPointNormal += edge2.slopeInverseNormalY;
                
                edge1.highPointZ += edge1.slopeInverseXZ * edge1.slopeInverseXY + edge1.slopeInverseYZ;
                edge2.highPointZ += edge2.slopeInverseXZ * edge2.slopeInverseXY + edge2.slopeInverseYZ;
                
            }
        
            // Update edge information in current active polygon
            int lastValidEdge = 0;
            int totalEdgeNum = (int)currentEdgeTable.size();
            for (int i = 0; i < totalEdgeNum; i ++,lastValidEdge ++){
                while (currentEdgeTable[i].spanY <= 0){
                    i ++;
                    if (i >= totalEdgeNum)  break;
                }
                if (i >= totalEdgeNum)  break;
                currentEdgeTable[lastValidEdge] = currentEdgeTable[i];
            }
            currentEdgeTable.resize(lastValidEdge);
            currentActivePolygon.spanY --;
        }
        // Update polygon information
        int lastValidPolygon = 0;
        int totalPolygonNum = (int)activePolygonTable.size();
        for (int i = 0; i < totalPolygonNum; i ++, lastValidPolygon ++){
            while (activePolygonTable[i].spanY < 0){
                i ++ ;
                if (i >= totalPolygonNum)   break;
            }
            if (i >= totalPolygonNum)   break;
            activePolygonTable[lastValidPolygon] = activePolygonTable[i];
        }
        activePolygonTable.resize(lastValidPolygon);
    }
}

// perfomr z-buffer algorithm to all models in this scene
void zBufferEngine::run(scene& scene){
    
    // guarantee this function only work when needed
    if (!needsUpdate)   return;
    
    // Initialize iBuffer and zBuffer
    // --> No need to initialize rBuffer, gBuffer and bBuffer
    for(int height = 0; height < screenHeight; height ++){
        for(int width = 0; width < screenWidth;width ++){
            iBuffer[height][width] = false;
            zBuffer[height][width] = INT_MAX;
        }
    }
    
    // Run z-buffer algorithm to each model in this scene
    for(auto& model: scene.models){
        this->run(model.second);
    }
    needsUpdate = true;
}
