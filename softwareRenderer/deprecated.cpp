//
//  deprecated.cpp
//  softwareRenderer
//
//  Created by pkovewnikov on 16.05.15.
//  Copyright (c) 2015 kovewnikov. All rights reserved.
//

#include "deprecated.h"

//судя по всему этот метод не такой быстрый
void rasterizeTriangleOld(std::vector<Vec3i> face, TGAImage *image) {
    Vec3i someLeftV = face[0];
    Vec3i someRightV = face[0];
    Vec3i someTopV = face[0];
    Vec3i someBtmV = face[0];
    for(int i=1; i<face.size(); i++) {
        Vec3i vertex = face[i];
        if(vertex.x < someLeftV.x) {
            someLeftV = vertex;
        }
        if(vertex.x > someRightV.x) {
            someRightV = vertex;
        }
        if(vertex.y > someTopV.y) {
            someTopV = vertex;
        }
        if(vertex.y < someBtmV.y) {
            someBtmV = vertex;
        }
    }
    
    for(int i=someLeftV.x; i<someRightV.x; i++) {
        for(int j=someBtmV.y; j<someTopV.y; j++) {
            Vec3i checkVertex(i,j, 0);
            Vec3i faceVertices[3];
            for(int k=0; k<3;k++) {
                faceVertices[k] = face[k] - checkVertex;
                faceVertices[k].z = 0;
            }
            float accum=0.;
            for(int k=0; k<3;k++) {
                float scalarProduct = faceVertices[k].normalized() * faceVertices[(k+1)%3].normalized();
                accum+=(1.-scalarProduct);
            }
            
            if(accum >= 4.) {
                image->set(i, j, TGAColor(255, 0,   0,   255));
            }
        }
    }
}


void lineOld(int x0, int y0, int x1, int y1, TGAImage &image,const TGAColor &color) {
    if(y1 < y0) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int totalHeight = y1-y0;
    for(int y=y0; y<=y1; y++) {
        float coef = ((y-y0)/(float)totalHeight);
        int x = x0 + (x1-x0)*coef;
        image.set(x,y, color);
    }
}