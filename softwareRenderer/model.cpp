//
//  model.cpp
//  softwareRenderer
//
//  Created by kovewnikov on 14.05.15.
//  Copyright (c) 2015 kovewnikov. All rights reserved.
//

#include "model.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "helpers.h"


Model::Model(const char* filename, const char *textureMapFilename) {
    _textureMap = TGAImage();
    if(!_textureMap.read_tga_file(textureMapFilename)) {
        std::cout << "Ошибка при чтении файла текстуры";
        return;
    }
    
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if(in.fail()) {
        std::cout << "Ошибка при открытии файла";
        return;
    }
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if(!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f vertex;
            for(int i=0; i<3; i++) { iss >> vertex.raw[i]; }
            _verts.push_back(vertex);
        } else if(!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f texCoord;
            for(int i=0; i<2; i++) {
                iss >> texCoord.raw[i];
            }
            float trashf;
            iss >> trashf;
            _texCorrdinates.push_back(texCoord);
        } else if(!line.compare(0, 2, "f ")) {
            std::vector<int> face;
            int trashi, vIndex;
            iss >> trash;
            while (iss >> vIndex >> trash >> trashi >> trash >> trashi) {
                vIndex--; // in wavefront obj all indices start at 1, not zero
                face.push_back(vIndex);

            }
            _faces.push_back(face);
        }
    }
    std::cerr << "v# " << _verts.size() << " f#" << _faces.size() << "\n";
    
}
Model::~Model() {}
int Model::verticesCount() {
    return (int)_verts.size();
}
int Model::facesCount() {
    return (int)_faces.size();
}
Vec3f Model::vertexByIndex(int idx) const {
    return _verts[idx];
}
std::vector<int> Model::faceByIndex(int idx) const {
    return _faces[idx];
}
int compareFacesByZFunc(const std::vector<int> *face1, const std::vector<int> *face2, const Model *model);
void Model::sortFacesByZ() {
    mergeSortFaces(&_faces, (int)_faces.size(), this, compareFacesByZFunc);
}

int compareFacesByZFunc(const std::vector<int> *face1, const std::vector<int> *face2, const Model *model) {
    
    float someFarZ = 0.0;
    int retVal=-1;
    for(int i=0; i<face1->size(); i++) {
        Vec3f vertex = model->vertexByIndex(face1->at(i));
        if(i==0 || vertex.z < someFarZ) {
            someFarZ = vertex.z;
        }
    }
    for(int i=0; i<face2->size();i++) {
        Vec3f vertex = model->vertexByIndex(face2->at(i));
        if(vertex.z < someFarZ) {
            retVal = 1;
            break;
        } else if(vertex.z == someFarZ) {
            retVal = 0;
        }
    }
    return retVal;
}
