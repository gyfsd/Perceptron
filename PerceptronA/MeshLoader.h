#pragma once
#include <Windows.h>
#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include "3DObjects.h"	
mesh3d LoadObjMesh(wchar_t* path);
mesh3d LoadObjMeshA(char* path);