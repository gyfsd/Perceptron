#pragma once
#include "3DGeometry.h"
#include "2DGeometry.h"
#include <stdint.h>
typedef struct camera3d {
	verticle3d pos;
	verticle3d rot;
	verticle2d size;
};
typedef struct camera3f {
	verticle3f pos;
	verticle3f rot;
	verticle2f size;
};
typedef struct camera3dr {
	verticle3d pos;
	verticle3d rotx;
	verticle3d roty;
	verticle2d size;
};
typedef struct camera3fr {
	verticle3f pos;
	verticle3f rotx;
	verticle3f roty;
	verticle2f size;
};
typedef struct edges3 {
	uint32_t* edges;
	uint32_t edgesC;
	uint8_t state;
};
typedef struct mesh3d {
	verticle3d * verticles;
	uint32_t verticlesC;
	edges3* faces;
	uint32_t facesC;
};