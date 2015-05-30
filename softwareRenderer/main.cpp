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
#include "helpers.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);
const TGAColor blue   = TGAColor(0, 0, 255,   255);

mat lookAt(Vec3f eye, Vec3f center, Vec3f up) {
    Vec3f newEZ = (eye-center).normalized();
    Vec3f newEX = (up^newEZ).normalized();
    Vec3f newEY = (newEZ^newEX).normalized();
    
    
    mat view(4,4);
    for(int i=0; i<3; i++) {
        view.set(0, i, newEX.raw[i]);
        view.set(1, i, newEY.raw[i]);
        view.set(2, i, newEZ.raw[i]);
    }
    return view;
    //return view.inversed();
}

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





void triangle(Vec3i *face, Vec2i *uvs, Model *model, TGAImage &renderImage, float lightIntensity, int *zBuffer) {
    if (face[0].y==face[1].y && face[0].y==face[2].y) return; // i dont care about degenerate triangles
    
    int imgWidth = renderImage.get_width();
    int imgHeight = renderImage.get_height();
    int zBufferSize = imgWidth*imgHeight;
    
    //выстраиваем вершины по возрастанию y
    for(int i=0; i<3; i++) {
        int swapTargetIdx = i;
        for (int j=i+1; j<3; j++) {
            
            if(face[swapTargetIdx].y > face[j].y) {
                swapTargetIdx = j;
            }
        }
        if(swapTargetIdx!=i) {
            std::swap(face[i], face[swapTargetIdx]);
            std::swap(uvs[i], uvs[swapTargetIdx]);
        }

    }
    
    
    int totalHeight = face[2].y - face[0].y;
    
    for(int y=face[0].y; y<=face[2].y; y++) {
        bool seg2Yet = y >= face[1].y;
        int segmentHeight = seg2Yet ? face[2].y - face[1].y : face[1].y - face[0].y;
        
        float totalCoef = (y-face[0].y) / (float)totalHeight;
        float segmentCoef;
        if(!seg2Yet) {
            segmentCoef = (y-face[0].y) / (float)(segmentHeight == 0 ? 1 : segmentHeight);
        } else {
            segmentCoef = (y-face[1].y) / (float)(segmentHeight == 0 ? 1 : segmentHeight);
        }
        
        int xA = face[0].x + (face[2].x-face[0].x)*totalCoef;
        int xB;
        if(!seg2Yet) {
            xB = face[0].x + (face[1].x-face[0].x)*segmentCoef;
        } else {
            xB = face[1].x + (face[2].x-face[1].x)*segmentCoef;
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
            int z = face[0].z + (face[2].z-face[0].z)*totalCoef;
            int zAddr = y*imgWidth + x;
            if(0 <= zAddr && zAddr < zBufferSize && z >= zBuffer[zAddr]) {
                
                float uvXCoef = (x-xA)/(float)(xB-xA == 0 ? 1 : xB-xA);
                int u = uA + (uB-uA) * uvXCoef;
                int v = uvs[0].y + (uvs[2].y - uvs[0].y) * totalCoef;
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

        Vec3f rawV[3];
        mat transformMat = model->transformMat();
        Vec3i preparedVertices[3];
        Vec2i uvs[3];
        for(int j=0; j<3; j++) {
            rawV[j] = model->vertexByIndex(face[j].vertexIdx);
            //трансформим
            rawV[j] = (transformMat * Vec4f(rawV[j])).projectTo3D();
            //перобразуем в координаты канвваса
            preparedVertices[j] = Vec3i((rawV[j].x+1.)*halfWidth, (rawV[j].y+1.)*halfWidth, (rawV[j].z+1.)*halfWidth);
            //заполняем uv
            uvs[j] = model->uvByIndex(face[j].uvIdx);
        }
        
        Vec3f faceNormVec = (rawV[2]-rawV[0])^(rawV[1]-rawV[0]);
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
    mat transform = lookAt(Vec3f(1,0,0), Vec3f(), Vec3f(0,1,0));
    std::cout << "look at matrix\n";
    std::cout << transform;
//    std::cout << "look at matrix inversed\n";
//    transform = transform.inversed();
//    std::cout << transform;
    
    
    std::cout << "test\n";
    mat m1(4,4);
    for(int i=0; i<m1.size()-m1.nDimension(); i++) {
        if(i==3 || i==7 || i==11) continue;
        m1[i] = rand()%55;
    }
    
    mat m2 = m1.minorByRowAndCol(3, 3);
    std::cout << "m1\n";
    std::cout << m1;
    std::cout << "m1 -1\n";
    m1 = m1.inversed();
    std::cout << m1 << "\n";
    std::cout << "m1\n";
    m1 = m1.inversed();
    std::cout << m1<< "\n";
//
//    std::cout << "m2\n";
//    
//    std::cout << m2;
//    std::cout << "m2 -1\n";
//    m2 = m2.inversed();
//    std::cout << m2 << "\n";
//    std::cout << "m2 det\n";
//    std::cout << m2.det() << "\n";
    
    
    
    
    //перспективное искажение
    //transform[14] = -0.3;
    //test
//    transform[6] = 0.9;
    
    
//    mat transform2(4,4);
//    transform2[0] = cosf(M_PI/2);
//    transform2[2] = -sinf(M_PI/2);
//    transform2[8] = sinf(M_PI/2);
//    transform2[10] = cosf(M_PI/2);
////    transform2[5] = cosf(M_PI/2);
////    transform2[6] = -sinf(M_PI/2);
////    transform2[9] = sinf(M_PI/2);
////    transform2[10] = cosf(M_PI/2);
//    
//    
////    transform.set(0, 0, 0.5);
////    transform.set(1, 1, 0.5);
////    transform.set(2, 2, 0.5);
    model->loadTransformMatrix(transform);
    //drawSkeleton(model, red, &image);
    drawModel(model, &image, false);
    
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");

    delete model;
    
    
    
    return 0;
}



