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
private:
    Vec3f* _vcPntr;
    Vec2f* _uvPntr;
public:
    VertexInfo(Vec3f* vcPntrIni, Vec2f* uvPntrIni) : _vcPntr(vcPntrIni), _uvPntr(uvPntrIni) {}
    Vec3f vertexCoordinates() {
        return *_vcPntr;
    }
    Vec2f mainTextureUV() {
        return *_uvPntr;
    }
} VertexInfo;

class Model {
private:
    std::vector<Vec3f> _verts;
    std::vector<std::vector<VertexInfo> > _faces;
    std::vector<Vec2f> _texCorrdinates;
    TGAImage _textureMap;
public:
    Model(const char* filename, const char *textureMapFilename);
    ~Model();
    int verticesCount();
    int facesCount();
    Vec3f vertexByIndex(int i) const;
    std::vector<VertexInfo> faceByIndex(int idx) const;
    
    void sortFacesByZ();
};

#endif /* defined(__softwareRenderer__model__) */
