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

class Model {
private:
    std::vector<Vec3f> _verts;
    std::vector<std::vector<int> > _faces;
    std::vector<Vec2f> _texCorrdinates;
    TGAImage _textureMap;
public:
    Model(const char* filename, const char *textureMapFilename);
    ~Model();
    int verticesCount();
    int facesCount();
    Vec3f vertexByIndex(int i) const;
    std::vector<int> faceByIndex(int idx) const;
    
    void sortFacesByZ();
};

#endif /* defined(__softwareRenderer__model__) */
