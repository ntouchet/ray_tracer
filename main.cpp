#include <iostream>
#include "rayTracer.h"



int main(int argc, char* argv[])
{
    char* file = argv[1];
    rayTracer scene(file);
    scene.trace();
    return 0;
}
