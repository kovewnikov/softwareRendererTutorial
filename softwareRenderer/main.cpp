//
//  main.cpp
//  softwareRenderer
//
//  Created by kovewnikov on 14.05.15.
//  Copyright (c) 2015 kovewnikov. All rights reserved.
//

#include <iostream>
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include "deprecated.h"
#include "helpers.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);
const TGAColor blue   = TGAColor(0, 0, 255,   255);

//юзаем алгоритм Брезенхема для растеризации линии
void line (int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if(std::abs(x1-x0) < std::abs(y1-y0)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if(x0 > x1) {
        std::swap(x0,x1);
        std::swap(y0,y1);
    }
    int dx = std::abs(x1-x0);
    int dy = std::abs(y1-y0);
    int dirSign = y1>=y0 ? 1 : -1;
    // float dError = dy/(float)dx;
    float dError = dy;
    float error = 0;
    int y = y0;
    for(int x=x0; x<x1; x++) {
        if(!steep) {
            image.set(x,y, color);
        } else {
            image.set(y,x, color);
        }
        
        
        error+=dError;
        
        if(error+error > dx) {
            y+=1*dirSign;
            error-=dx;
        }
    }
}





void triangle(Vec3i *faces, Vec2i *uvs, Model *model, TGAImage &renderImage, float lightIntensity, int *zBuffer) {
    if (faces[0].y==faces[1].y && faces[0].y==faces[2].y) return; // i dont care about degenerate triangles
    
    int imgWidth = renderImage.get_width();
    int imgHeight = renderImage.get_height();
    int zBufferSize = imgWidth*imgHeight;
    
    //выстраиваем вершины по возрастанию y
    for(int i=0; i<3; i++) {
        int swapTargetIdx = i;
        for (int j=i+1; j<3; j++) {
            
            if(faces[swapTargetIdx].y > faces[j].y) {
                swapTargetIdx = j;
            }
        }
        if(swapTargetIdx!=i) {
            std::swap(faces[i], faces[swapTargetIdx]);
            std::swap(uvs[i], uvs[swapTargetIdx]);
        }

    }
    
    
    int totalHeight = faces[2].y - faces[0].y;
    
    for(int y=faces[0].y; y<=faces[2].y; y++) {
        bool seg2Yet = y >= faces[1].y;
        int segmentHeight = seg2Yet ? faces[2].y - faces[1].y : faces[1].y - faces[0].y;
        
        float totalCoef = (y-faces[0].y) / (float)totalHeight;
        float segmentCoef;
        if(!seg2Yet) {
            segmentCoef = (y-faces[0].y) / (float)(segmentHeight == 0 ? 1 : segmentHeight);
        } else {
            segmentCoef = (y-faces[1].y) / (float)(segmentHeight == 0 ? 1 : segmentHeight);
        }
        
        int xA = faces[0].x + (faces[2].x-faces[0].x)*totalCoef;
        int xB;
        if(!seg2Yet) {
            xB = faces[0].x + (faces[1].x-faces[0].x)*segmentCoef;
        } else {
            xB = faces[1].x + (faces[2].x-faces[1].x)*segmentCoef;
        }
        
        int uA = uvs[0].x + (uvs[2].x-uvs[0].x)*totalCoef;
        int uB;
        if(!seg2Yet) {
            uB = uvs[0].x + (uvs[1].x-uvs[0].x)*segmentCoef;
        } else {
            uB = uvs[1].x + (uvs[2].x-uvs[1].x)*segmentCoef;
        }
        
        

        if(xA > xB) { std::swap(xA, xB); std::swap(uA, uB);}
        for(int x=xA; x<=xB; x++) {
            int z = faces[0].z + (faces[2].z-faces[0].z)*totalCoef;
            int zAddr = y*imgWidth + x;
            if(0 <= zAddr && zAddr < zBufferSize && z >= zBuffer[zAddr]) {
                
                float uvXCoef = (x-xA)/(xB-xA == 0 ? 1 : xB-xA);
                int u = uA + (uB-uA) * uvXCoef;
                int v = uvs[0].y + (uvs[2].y - uvs[0].y) * totalCoef;
                //                renderImage.set(x,y,TGAColor(intensity*255, intensity*255, intensity*255, 255));
                TGAColor color = model->getTextureMapPixel(Vec2i(u,v));
                renderImage.set(x,y,TGAColor(color.r*lightIntensity, color.g*lightIntensity, color.b*lightIntensity, color.a));
                zBuffer[zAddr] = z;
            

                
            }
        }
     }

}

void drawSkeleton(Model *model, TGAColor color, TGAImage *image) {
    float halfWidth = image->get_width()/2.;
    
    for (int i=0; i<model->facesCount(); i++) {
        std::vector<VertexInfo> faceRaw = model->faceByIndex(i);
        for(int j=0; j<3; j++) {
            VertexInfo vi0 = faceRaw[j];
            VertexInfo vi1 = faceRaw[(j+1)%3];
            
            
            int x0 = (model->vertexByIndex(vi0.vertexIdx).x+1.) * halfWidth;
            int y0 = (model->vertexByIndex(vi0.vertexIdx).y+1.) * halfWidth;
            int x1 = (model->vertexByIndex(vi1.vertexIdx).x+1.) * halfWidth;
            int y1 = (model->vertexByIndex(vi1.vertexIdx).y+1.) * halfWidth;


            line(x0, y0, x1, y1, *image, color);
        }
    }
}
void drawModel(Model *model, TGAImage *image, bool needSortFaces) {
    if(needSortFaces) {
        model->sortFacesByZ();
    }
    int imgW = image->get_width();
    int imgH = image->get_height();
    int screenBufferSize = imgW*imgH;
    float halfWidth = imgW/2.;
    
    int *zBuffer = new int[screenBufferSize];
    for (int i=0; i<screenBufferSize; i++) {
        zBuffer[i] = std::numeric_limits<int>::min();
    }
    
    for (int i=0; i<model->facesCount(); i++) {
        std::vector<VertexInfo> face = model->faceByIndex(i);
        
        
        Vec3i preparedVertices[3];
        
        Vec3f rawV0 = model->vertexByIndex(face[0].vertexIdx);
        preparedVertices[0] = Vec3i((rawV0.x+1.)*halfWidth, (rawV0.y+1.)*halfWidth, (rawV0.z+1.)*halfWidth);
        Vec3f rawV1 = model->vertexByIndex(face[1].vertexIdx);
        preparedVertices[1] = Vec3i((rawV1.x+1.)*halfWidth, (rawV1.y+1.)*halfWidth, (rawV1.z+1.)*halfWidth);
        Vec3f rawV2 = model->vertexByIndex(face[2].vertexIdx);
        preparedVertices[2] = Vec3i((rawV2.x+1.)*halfWidth, (rawV2.y+1.)*halfWidth, (rawV2.z+1.)*halfWidth);
        
        Vec2i uvs[3];
        uvs[0] = model->uvByIndex(face[0].uvIdx);
        uvs[1] = model->uvByIndex(face[1].uvIdx);
        uvs[2] = model->uvByIndex(face[2].uvIdx);
        
        Vec3f faceNormVec = (rawV2-rawV0)^(rawV1-rawV0);
        Vec3f lightVec(0,0,-1);
        float intensity = lightVec * faceNormVec.normalized();
        
        
        
        if (intensity>0) {
            triangle(preparedVertices, uvs, model, *image, intensity, zBuffer);
        }
    }
    
    delete [] zBuffer;
}


int main(int argc, const char * argv[]) {
    

    int width = 1000;
    int height = 1000;

    TGAImage image(width, height, TGAImage::RGB);
    image.set(52, 41, red);

    
    
    Model *model =  new Model("resources/african_head.obj", "resources/african_head_diffuse.tga");
    //drawSkeleton(model, red, &image);
    drawModel(model, &image, false);
    
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    
    delete model;
    
    Mati m1(3,3);
    Mati m2(3,3);
    Mati result(3,3);
    m2.set(0, 0, 5);
    m2.set(1, 1, 12);
    m2.set(2, 2, 52);
    
    
    Mati::multiplyMatrices(&m1, &m2, &result);
    
    std::cout << result;
    
    return 0;
}



