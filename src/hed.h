#pragma once
#include <iostream>
#include <lodepng.h>
#include <vector>
#include <cstdint>
#include <string>
#include<limits>
#include <limits>
#include <algorithm>



enum position
{
    leftt = 0,
    rightt = 1,
    up = 2,
    none = 3,
};
struct Vertice
{
    bool vizit;
    int path_w;
    int mine_w;
    position early;
};