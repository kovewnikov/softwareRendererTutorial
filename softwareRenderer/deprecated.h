//
//  deprecated.h
//  softwareRenderer
//
//  Created by pkovewnikov on 16.05.15.
//  Copyright (c) 2015 kovewnikov. All rights reserved.
//

#ifndef __softwareRenderer__deprecated__
#define __softwareRenderer__deprecated__

#include <stdio.h>
#include <vector>
#include "geometry.h"
#include "tgaimage.h"


void lineOld(int x0, int y0, int x1, int y1, TGAImage &image,const TGAColor &color);
void rasterizeTriangleOld(std::vector<Vec3i> face, TGAImage *image);

#endif /* defined(__softwareRenderer__deprecated__) */
