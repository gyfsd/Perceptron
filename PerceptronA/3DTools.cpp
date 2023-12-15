#include "3DTools.h"
verticle3d rotateX3d(verticle3d v,double rx,double ry) {
	verticle3d out;
	out.x = v.x;
	out.y = (ry * v.y) - (rx * v.z);
	out.z = (rx * v.y) + (ry * v.z);
	return out;
}
verticle3d rotateY3d(verticle3d v, double rx, double ry) {
	verticle3d out;
	out.x = (ry * v.x) + (rx * v.z);
	out.y = v.y;
	out.z = -(rx * v.x) + (ry * v.z);
	return out;
}
verticle3d rotateZ3d(verticle3d v, double rx, double ry) {
	verticle3d out;
	out.x = (ry * v.x) - (rx * v.y);
	out.y = (rx * v.x) + (ry * v.y);
	out.z = v.z;
	return out;
}
verticle3f rotateX3f(verticle3f v, float rx, float ry) {
	verticle3f out;
	out.x = v.x;
	out.y = (ry * v.y) - (rx * v.z);
	out.z = (rx * v.y) + (ry * v.z);
	return out;
}
verticle3f rotateY3f(verticle3f v, float rx, float ry) {
	verticle3f out;
	out.x = ry * v.x + rx * v.z;
	out.y = v.y;
	out.z = -rx * v.x + ry * v.z;
	return out;
}
verticle3f rotateZ3f(verticle3f v, float rx, float ry) {
	verticle3f out;
	out.x = ry * v.x - rx * v.y;
	out.y = rx * v.x + ry * v.y;
	out.z = v.z;
	return out;
}
verticle3d perspective3d(verticle3d verticle,camera3dr camera) {
	verticle3d out;
	out.x = verticle.x - camera.pos.x;
	out.y = verticle.y - camera.pos.y;
	out.z = verticle.z - camera.pos.z;
	verticle3d buf = rotateY3d(out, camera.rotx.y, camera.roty.y);
	buf = rotateX3d(buf, camera.rotx.x, camera.roty.x);
	buf = rotateZ3d(buf, camera.rotx.z, camera.roty.z);
	out.x = buf.x / buf.z * camera.size.x;
	out.y = buf.y / buf.z * camera.size.y;
	out.z = buf.z;
	return out;
}
camera3dr camera3dto3dr(camera3d cam) {
	camera3dr out;
	out.pos = cam.pos;
	out.size = cam.size;
	out.rotx.x = sin(cam.rot.x);
	out.roty.x = cos(cam.rot.x);
	out.rotx.y = sin(cam.rot.y);
	out.roty.y = cos(cam.rot.y);
	out.rotx.z = sin(cam.rot.z);
	out.roty.z = cos(cam.rot.z);
	return out;
}