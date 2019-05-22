//
//  polygon.hpp
//  3DModel
//
//  Created by 刘宇轩 on 2019/3/17.
//  Copyright © 2019 yuxuanliu. All rights reserved.
//

#ifndef polygon_hpp
#define polygon_hpp

#include <stdio.h>
#include <vector>
#include "edge.hpp"
using namespace std;

class polygon{
public:
    int index;
    int spanY;
    vector<edge> edgeTable;
};

#endif /* polygon_hpp */
