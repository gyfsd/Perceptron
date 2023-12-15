#include "MeshLoader.h"
namespace text_op {
	uint64_t skipIdenticalCharacters(uint64_t offset, uint8_t* buf, uint64_t buflimit) {
		uint64_t cur = offset;
		uint8_t sym = buf[cur];
		cur++;
		while (buf[cur] != sym & cur < buflimit) {

			cur++;
		}
		if (cur == buflimit) {
			return -1;
		}
		return cur;
	}
	uint64_t skipCharactersNotFromTheList(uint64_t offset, uint8_t* buf, uint64_t buflimit, uint8_t* chars, uint8_t charscount) {
		uint64_t cur = offset;
		uint8_t cur2;
		while (cur < buflimit) {
			cur2 = 0;
			while (cur2 < charscount) {
				if (chars[cur2] == buf[cur]) {
					return cur;
				}
				cur2++;
			}
			cur++;
		}
		return -1;
	}
	uint64_t skipCharactersFromTheList(uint64_t offset, uint8_t* buf, uint64_t buflimit, uint8_t* chars, uint8_t charscount) {
		uint64_t cur = offset;
		uint8_t cur2;
		while (cur < buflimit) {
			cur2 = 0;
			while (cur2 < charscount) {
				if (chars[cur2] == buf[cur]) {
					goto skip2;
				}
				cur2++;
			}
			return cur;
		skip2:
			cur++;
		}
		return -1;
	}
};
mesh3d LoadObjMesh(wchar_t* path) {
	const long skip = 4096;
	HANDLE file = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == NULL) {
		std::cerr << "Failed to open mesh file\n";
		exit(1);
	}
	mesh3d m = {};
	long size = GetFileSize(file, NULL);
	uint8_t* filebuf = (uint8_t*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);

	uint64_t maincur = 0;
	DWORD readen;
	while (maincur < size) {
		SetFilePointer(file, maincur, NULL, FILE_BEGIN);
		ReadFile(file, filebuf + maincur, skip, &readen, NULL);
		maincur += skip;
	}
	while (maincur < size) {
		SetFilePointer(file, maincur, NULL, FILE_BEGIN);
		ReadFile(file, filebuf + maincur, 1, &readen, NULL);
		maincur += skip;
	}
	maincur = 0;
	uint32_t vcount = 0;
	uint32_t fcount = 0;
	uint32_t fsum = 0;
	uint32_t subcur;
	while (maincur < size) {
		if (strncmp((char*)filebuf + maincur, "v ", 2) == 0) {
			vcount++;
		}
		if (strncmp((char*)filebuf + maincur, "f ", 2) == 0) {
			subcur = maincur + 2;
			fcount++;
			while (filebuf[subcur] != 0x0a & subcur < size) {
				subcur = text_op::skipCharactersNotFromTheList(subcur, filebuf, size, (uint8_t*)"-0123456789infNa.\n", 18);
				subcur = text_op::skipCharactersFromTheList(subcur, filebuf, size, (uint8_t*)"-0123456789infNa./", 18);
				fsum++;
			}
		}
		//putchar(filebuf[maincur]);
		maincur++;
	}
	std::cerr << "verticles:" << vcount << " faces:" << fcount << " fsum:" << fsum << std::endl;
	//m.mesh.pointsC = vcount;
	//m.mesh.points = (struct Point*)VirtualAlloc(NULL, sizeof(struct Point) * vcount, MEM_COMMIT, PAGE_READWRITE);
	//m.mesh.faces = (struct Face*)VirtualAlloc(NULL, sizeof(struct Face) * fcount, MEM_COMMIT, PAGE_READWRITE);
	//m.mesh.facesC = fcount;
	m.verticlesC = vcount;
	m.facesC = fcount;
	m.faces = (edges3 *)VirtualAlloc(NULL, sizeof(edges3) * fcount, MEM_COMMIT, PAGE_READWRITE);
	m.verticles = (verticle3d*)VirtualAlloc(NULL, sizeof(verticle3d) * vcount, MEM_COMMIT, PAGE_READWRITE);
	float* verticles_positions = (float*)VirtualAlloc(NULL, sizeof(float) * 3 * vcount, MEM_COMMIT, PAGE_READWRITE);
	uint32_t* faces_points = (uint32_t*)VirtualAlloc(NULL, sizeof(uint32_t) * fsum, MEM_COMMIT, PAGE_READWRITE);
	maincur = 0;
	uint32_t counter;
	float posbuf;
	uint32_t vcounter = 0;
	uint32_t pcounter = 0;
	while (maincur < size) {
		if (strncmp((char*)filebuf + maincur, "v ", 2) == 0) {
			subcur = maincur;
			counter = 0;
			while (counter < 3 & subcur < size & filebuf[subcur] != 0x0a) {
				subcur = text_op::skipCharactersNotFromTheList(subcur, filebuf, size, (uint8_t*)"-0123456789infNa.\n", 18);
				posbuf = -atof((char*)filebuf + subcur);
				verticles_positions[vcounter] = posbuf;
				vcounter++;
				subcur = text_op::skipCharactersFromTheList(subcur, filebuf, size, (uint8_t*)"-0123456789infNa./", 18);
			}
			m.verticles[pcounter].x = *(verticles_positions + (pcounter * 3));
			m.verticles[pcounter].y = *(verticles_positions + (pcounter * 3)+1);
			m.verticles[pcounter].z = *(verticles_positions + (pcounter * 3)+2);
			pcounter++;
		}
		maincur++;
	}
	maincur = 0;
	vcounter = 0;
	uint32_t mcounter = 0;
	while (maincur < size) {
		if (strncmp((char*)filebuf + maincur, "f ", 2) == 0) {
			subcur = maincur + 2;
			pcounter = 0;
			m.faces[vcounter].edges = faces_points + mcounter;
			while (filebuf[subcur] != 0x0a & subcur < size) {
				subcur = text_op::skipCharactersNotFromTheList(subcur, filebuf, size, (uint8_t*)"-0123456789iN.\n", 18);
				counter = atoi((char*)filebuf + subcur);
				m.faces[vcounter].edgesC++;
				m.faces[vcounter].edges[pcounter] = counter - 1;
				pcounter++;
				mcounter++;
				subcur = text_op::skipCharactersFromTheList(subcur, filebuf, size, (uint8_t*)"-0123456789infNa./", 18);
			}
			vcounter++;
		}
		maincur++;
	}
	//end
	VirtualFree((LPVOID)filebuf, 0, MEM_RELEASE);
	return m;
}
mesh3d LoadObjMeshA(char* path) {
	const long skip = 4096;
	HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == NULL) {
		std::cerr << "Failed to open mesh file\n";
		exit(1);
	}
	mesh3d m = {};
	long size = GetFileSize(file, NULL);
	uint8_t* filebuf = (uint8_t*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);

	uint64_t maincur = 0;
	DWORD readen;
	while (maincur < size) {
		SetFilePointer(file, maincur, NULL, FILE_BEGIN);
		ReadFile(file, filebuf + maincur, skip, &readen, NULL);
		maincur += skip;
	}
	while (maincur < size) {
		SetFilePointer(file, maincur, NULL, FILE_BEGIN);
		ReadFile(file, filebuf + maincur, 1, &readen, NULL);
		maincur += skip;
	}
	maincur = 0;
	uint32_t vcount = 0;
	uint32_t fcount = 0;
	uint32_t fsum = 0;
	uint32_t subcur;
	while (maincur < size) {
		if (strncmp((char*)filebuf + maincur, "v ", 2) == 0) {
			vcount++;
		}
		if (strncmp((char*)filebuf + maincur, "f ", 2) == 0) {
			subcur = maincur + 2;
			fcount++;
			while (filebuf[subcur] != 0x0a & subcur < size) {
				subcur = text_op::skipCharactersNotFromTheList(subcur, filebuf, size, (uint8_t*)"-0123456789infNa.\n", 18);
				subcur = text_op::skipCharactersFromTheList(subcur, filebuf, size, (uint8_t*)"-0123456789infNa./", 18);
				fsum++;
			}
		}
		//putchar(filebuf[maincur]);
		maincur++;
	}
	std::cerr << "verticles:" << vcount << " faces:" << fcount << " fsum:" << fsum << std::endl;
	//m.mesh.pointsC = vcount;
	//m.mesh.points = (struct Point*)VirtualAlloc(NULL, sizeof(struct Point) * vcount, MEM_COMMIT, PAGE_READWRITE);
	//m.mesh.faces = (struct Face*)VirtualAlloc(NULL, sizeof(struct Face) * fcount, MEM_COMMIT, PAGE_READWRITE);
	//m.mesh.facesC = fcount;
	m.verticlesC = vcount;
	m.facesC = fcount;
	m.faces = (edges3*)VirtualAlloc(NULL, sizeof(edges3) * fcount, MEM_COMMIT, PAGE_READWRITE);
	m.verticles = (verticle3d*)VirtualAlloc(NULL, sizeof(verticle3d) * vcount, MEM_COMMIT, PAGE_READWRITE);
	float* verticles_positions = (float*)VirtualAlloc(NULL, sizeof(float) * 3 * vcount, MEM_COMMIT, PAGE_READWRITE);
	uint32_t* faces_points = (uint32_t*)VirtualAlloc(NULL, sizeof(uint32_t) * fsum, MEM_COMMIT, PAGE_READWRITE);
	maincur = 0;
	uint32_t counter;
	float posbuf;
	uint32_t vcounter = 0;
	uint32_t pcounter = 0;
	while (maincur < size) {
		if (strncmp((char*)filebuf + maincur, "v ", 2) == 0) {
			subcur = maincur;
			counter = 0;
			while (counter < 3 & subcur < size & filebuf[subcur] != 0x0a) {
				subcur = text_op::skipCharactersNotFromTheList(subcur, filebuf, size, (uint8_t*)"-0123456789infNa.\n", 18);
				posbuf = -atof((char*)filebuf + subcur);
				verticles_positions[vcounter] = posbuf;
				vcounter++;
				subcur = text_op::skipCharactersFromTheList(subcur, filebuf, size, (uint8_t*)"-0123456789infNa./", 18);
			}
			m.verticles[pcounter].x = *(verticles_positions + (pcounter * 3));
			m.verticles[pcounter].y = *(verticles_positions + (pcounter * 3) + 1);
			m.verticles[pcounter].z = *(verticles_positions + (pcounter * 3) + 2);
			pcounter++;
		}
		maincur++;
	}
		maincur = 0;
	vcounter = 0;
	uint32_t mcounter = 0;
	while (maincur < size) { 
		if (strncmp((char*)filebuf + maincur, "f ", 2) == 0) {
			subcur = maincur + 2;
			pcounter = 0;
			m.faces[vcounter].edges = faces_points + mcounter;  
			while (filebuf[subcur] != 0x0a & subcur < size) {
				subcur = text_op::skipCharactersNotFromTheList(subcur, filebuf, size, (uint8_t*)"-0123456789iN.\n", 18);
				counter = atoi((char*)filebuf + subcur);
				m.faces[vcounter].edgesC++;
				m.faces[vcounter].edges[pcounter] = counter - 1;
				pcounter++;
				mcounter++;
				subcur = text_op::skipCharactersFromTheList(subcur, filebuf, size, (uint8_t*)"-0123456789infNa./", 18);
			}
			vcounter++;
		}
		maincur++;
	}
	//end
	VirtualFree((LPVOID)filebuf, 0, MEM_RELEASE);
	return m;
}