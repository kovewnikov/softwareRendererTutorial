//
//  helpers.cpp
//  softwareRenderer
//
//  Created by pkovewnikov on 16.05.15.
//  Copyright (c) 2015 kovewnikov. All rights reserved.
//

#include "helpers.h"
#include <vector>
#include <queue>
//declaration
void mergeSortRecursive(std::vector<std::vector<VertexInfo> > *faces,
                        int low,
                        int high,
                        Model *model,
                        int (*compareFunc)(const std::vector<VertexInfo> *a, const std::vector<VertexInfo> *b, const Model *model));

void merge (std::vector<std::vector<VertexInfo> > *faces,
            int low,//3
            int middle,//3
            int high,//4
            Model *model,
            int (*compareFunc)(const std::vector<VertexInfo> *a, const std::vector<VertexInfo> *b, const Model *model));






//implementation
void  mergeSortFaces(std::vector<std::vector<VertexInfo> > *faces,
                     int num,
                     Model *model,
                     int (*compareFunc)(const std::vector<VertexInfo> *a, const std::vector<VertexInfo> *b, const Model *model)) {
    mergeSortRecursive(faces, 0, num-1, model, compareFunc);
}

void mergeSortRecursive(std::vector<std::vector<VertexInfo> > *faces,
                        int low,
                        int high,
                        Model *model,
                        int (*compareFunc)(const std::vector<VertexInfo> *a, const std::vector<VertexInfo> *b, const Model *model)) {
    if(low<high) {
        int middle = low+(high-low)/2;
        mergeSortRecursive(faces, low, middle, model, compareFunc);
        mergeSortRecursive(faces, middle+1, high, model, compareFunc);
        merge(faces, low, middle, high, model, compareFunc);
    }
}
void merge (std::vector<std::vector<VertexInfo> > *faces,
            int low,//3
            int middle,//3
            int high,//4
            Model *model,
            int (*compareFunc)(const std::vector<VertexInfo> *a, const std::vector<VertexInfo> *b, const Model *model)) {
    std::queue<std::vector<VertexInfo> > bufferLow;
    std::queue<std::vector<VertexInfo> > bufferHigh;
    for(int i=low; i<=middle; i++) {
        bufferLow.push(faces->at(i));
    }
    for(int i=middle+1; i<=high; i++) {
        bufferHigh.push(faces->at(i));
    }
    
    int facesIterator = low;
    while(!bufferLow.empty() && !bufferHigh.empty()) {
        std::vector<VertexInfo> lowEl = bufferLow.front();
        std::vector<VertexInfo> HighEl = bufferHigh.front();

        if(compareFunc(&lowEl, &HighEl, model) > 0) {
            faces->at(facesIterator) = HighEl;
            bufferHigh.pop();
        } else {
            faces->at(facesIterator) = lowEl;
            bufferLow.pop();
        }
        
        facesIterator++;
    }
    std::queue<std::vector<VertexInfo> > remainingBuffer;
    if(!bufferLow.empty()) {
        remainingBuffer = bufferLow;
    } else {
        remainingBuffer = bufferHigh;
    }
    while(!remainingBuffer.empty()) {
        std::vector<VertexInfo> face = remainingBuffer.front();
        remainingBuffer.pop();
        faces->at(facesIterator) = face;
        facesIterator++;
    }
}

