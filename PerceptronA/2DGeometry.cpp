#include "2DGeometry.h"
float ConvertCords(float from, float to, float c) {
	float h = (to - from) / 2;
	return (c * h) + from + h;
}