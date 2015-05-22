//
//  model.h
//  softwareRenderer
//
//  Created by kovewnikov on 14.05.15.
//  Copyright (c) 2015 kovewnikov. All rights reserved.
//

#ifndef __softwareRenderer__model__
#define __softwareRenderer__model__

#include <vector>
#include "geometry.h"
#include "tgaimage.h"

typedef struct VertexInfo {
public:
    int vertexIdx;
    int uvIdx;
    VertexInfo(int vI, int uvI) : vertexIdx(vI), uvIdx(uvI) {}
//    Vec3f vertexCoordinates() {
//        return *_vcPntr;
//    }
//    Vec2f mainTextureUV() {
//        return *_uvPntr;
//    }
} VertexInfo;

class Model {
private:
    std::vector<Vec3f> _verts;
    std::vector<Vec2f> _uvsRaw;
    std::vector<Vec2i> _uvs;
    
    std::vector<std::vector<VertexInfo> > _faces;
    
    TGAImage _textureMap;
public:
    Model(const char* filename, const char *textureMapFilename);
    ~Model();
    int verticesCount();
    int facesCount();
    Vec3f vertexByIndex(int i) const;
    Vec2f uvRawByIndex(int idx) const;
    Vec2i uvByIndex(int idx) const;
    std::vector<VertexInfo> faceByIndex(int idx) const;
    TGAColor getTextureMapPixel(Vec2i uv);
    void sortFacesByZ();
};

#endif /* defined(__softwareRenderer__model__) */
