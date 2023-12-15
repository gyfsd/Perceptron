#pragma once
#include "3DObjects.h"
#include <math.h>
verticle3d rotateX3d(verticle3d v, double rx, double ry);
verticle3d rotateY3d(verticle3d v, double rx, double ry);
verticle3d rotateZ3d(verticle3d v, double rx, double ry);
verticle3f rotateX3f(verticle3f v, float rx, float ry);
verticle3f rotateY3f(verticle3f v, float rx, float ry);
verticle3f rotateZ3f(verticle3f v, float rx, float ry);
verticle3d perspective3d(verticle3d verticle, camera3dr camera);
camera3dr camera3dto3dr(camera3d cam);