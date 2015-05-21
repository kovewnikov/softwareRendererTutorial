//
//  helpers.h
//  softwareRenderer
//
//  Created by pkovewnikov on 16.05.15.
//  Copyright (c) 2015 kovewnikov. All rights reserved.
//

#ifndef __softwareRenderer__helpers__
#define __softwareRenderer__helpers__

#include <stdio.h>
#include "geometry.h"
#include <vector>
#include "model.h"

//алгоритм художника
void  mergeSortFaces(std::vector<std::vector<int> > *faces,
                     int num,
                     Model *model,
                     int (*compareFunc)(const std::vector<int> *a, const std::vector<int> *b, const Model *model));


#endif /* defined(__softwareRenderer__helpers__) */
