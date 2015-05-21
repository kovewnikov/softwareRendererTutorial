//
//  detailedFunctions.h
//  softwareRenderer
//
//  Created by pkovewnikov on 16.05.15.
//  Copyright (c) 2015 kovewnikov. All rights reserved.
//

#ifndef softwareRenderer_detailedFunctions_h
#define softwareRenderer_detailedFunctions_h


//Model *model =  new Model("obj/african_head.obj");
//for (int i=0; i<model->facesCount(); i++) {
//    std::vector<int> faceRaw = model->faceByIndex(i);
//    std::vector<Vec3i> faceForRasterizing;
//    
//    float halfWidth = width/2.;
//    
//    Vec2i preparedVertices[3];
//    Vec3f rawV0 = model->vertexByIndex(faceRaw[0]);
//    preparedVertices[0] = Vec2i((rawV0.x+1.)*halfWidth, (rawV0.y+1.)*halfWidth);
//    Vec3f rawV1 = model->vertexByIndex(faceRaw[1]);
//    preparedVertices[1] = Vec2i((rawV1.x+1.)*halfWidth, (rawV1.y+1.)*halfWidth);
//    Vec3f rawV2 = model->vertexByIndex(faceRaw[2]);
//    preparedVertices[2] = Vec2i((rawV2.x+1.)*halfWidth, (rawV2.y+1.)*halfWidth);
//    
//    
//    Vec3f faceNormVec1 = (rawV2-rawV0)^(rawV1-rawV0);
//    std::cout << "2, 1: " << faceNormVec1 << "";
//    Vec3f faceNormVec2 = (rawV2-rawV1)^(rawV0-rawV1);
//    std::cout << "2, 0: " << faceNormVec2 << "";
//    Vec3f faceNormVec3 = (rawV1-rawV2)^(rawV0-rawV2);
//    std::cout << "1, 0: " << faceNormVec3 << "";
//    
//    std::cout << "length1: " << faceNormVec1.length() << "\n";
//    std::cout << "length2: " << faceNormVec2.length() << "\n";
//    std::cout << "length3: " << faceNormVec3.length() << "\n";
//    
//    Vec3f temp = faceNormVec1.normalized();
//    std::cout << "normalized 2, 1: " << temp << "";
//    temp = faceNormVec2.normalized();
//    std::cout << "normalized 2, 0: " << temp << "";
//    temp = faceNormVec3.normalized();
//    std::cout << "normalized 1, 1: " << temp << "\n";
//    
//    Vec3f lightVec(0,0,-1);
//    
//    float intensity = lightVec * faceNormVec1.normalized();
//    
//    
//    if (intensity>0) {
//        triangle(preparedVertices, image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
//    }
//
//}
//    
//    
//    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
//    image.write_tga_file("output.tga");
//    
//    delete model;


#endif
