#include "NN.h"
#include "ActivateF.h"
#include "Threads.h"
#include "MathH.h"
#include "3DGeometry.h"
#include "2DGeometry.h"
#include "ArgumentsData.h"
#include <Windows.h>
#include <iostream>
#include <SDL.h>
#define cbufs 1000
#define DEFAULT_SRF_SIZEX 500
#define DEFAULT_SRF_SIZEY 500
typedef struct v2d {
	double x, y;
};
ul wcstrs(const wchar_t * s) {
	ul x = 0;
	while (s[x] != 0) {x++;}
	return x;
}
void command(const wchar_t * cstring,wchar_t * obuf,ul obufsize) {
	WriteConsoleW(GetStdHandle(STD_ERROR_HANDLE), cstring, wcstrs(cstring),NULL,NULL);
	HANDLE ih = GetStdHandle(STD_INPUT_HANDLE);
	ul cur = 0;
	wchar_t c = 0;
	DWORD b;
	while (cur < obufsize - 1 & c != L"\n"[0]) {
		ReadConsoleW(ih,&c,1,&b,NULL);
		obuf[cur] = c;
		cur++;
	}
	obuf[cur] = 0;
	return;
}
void cwwc(const wchar_t * str) {
	ul x = wcstrs(str);
	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE),str,x,NULL,NULL);
}
void cewc(const wchar_t* str) {
	ul x = wcstrs(str);
	WriteConsoleW(GetStdHandle(STD_ERROR_HANDLE), str, x, NULL, NULL);
}
double s2c(int a,int s) {
	return (double)(a) / (double)(s) * 2 - 1;
}
int c2s(double a, int s) {
	return (a + 1) / 2 * s;
}
void Resize32(uint32_t* in, int ix, int iy, uint32_t* out, int ox, int oy) {
	double cx = (double)ix / (double)ox;
	double cy = (double)ix / (double)oy;
	int x = 0;
	int y = 0;
	while (y < oy) {
		x = 0;
		while (x < ox) {
			((uint32_t*)(out))[x + (y * ox)] = ((uint32_t*)(in))[uint32_t((double)x * cx) + (uint32_t((double)y * cy) * ix)];
			x++;
		}
		y++;
	}
}
 int main(int argc, char* argv[]) {
	ul MaxIteration = 100;
	double BackPropogationLearningSpeed = 0.1;
	double ErrorMinimum = 0.01;
	int sx = 50;
	int sy = 50;
	double * buf = (double *)malloc(sx*sy*sizeof(double));
	double* circle = (double*)malloc(sx * sy * sizeof(double));
	double* square = (double*)malloc(sx * sy * sizeof(double));
	int x = 0;
	int y = 0;
	while (x < sx) {
		int y = 0;
		while (y < sy) {
			double xc = s2c(x,sx);
			double yc = s2c(y, sy);
			if (sqrt(pow(xc, 2) + pow(yc, 2)) < 0.5) {
				circle[x + (y * sx)] = 1;
			}
			else {
				circle[x + (y * sx)] = 0;
			}
			y++;
		}
		x++;
	}
	x = 0;
	y = 0;
	while (x < sx) {
		y = 0;
		while (y < sy) {
			square[x + (y * sx)] = 0;
			y++;
		}
		x++;
	}
	x = sx / 4;
	y = sy / 4;
	while (x < sx/4*3) {
		y = sy / 4;
		while (y < sy/4*3) {
			square[x + (y * sx)] = 1;
			y++;
		}
		x++;
	}
	NN net;
	NN_Arch net_arch = {NULL,4,0};
	ul net_lsize[10] = {3,100,100,1};
	net_arch.lsize = net_lsize;
	net.Init(net_arch);
	wchar_t cbuf[cbufs];
	while (1) {
		cewc(L"(Other - Exit; 1 - Set Activation Function; 2 - Start Learning NN; 3 - Forward Feed NN; 4 - Print NN info; 5 - Random Weights; 6 - Test NN; 7 - Save NN; 8 - Load NN; 9 - Set Error Minimum; 10 - Set Max iterations; 11 - Set Learning speed)\n");
		command(L">> ",cbuf,cbufs);
		ui val = _wtoi(cbuf);
		if (val == 0) {
			return 0;
		}
		if (val == 1) {
			cewc(L"(Other - Cancel;1 - Sigmoid;2 - ReLu;3 - Thx;4 - Linear;5 - Softsign;6 - SingleStep)\n");
			command(L">> ", cbuf, cbufs);
			ui val = _wtoi(cbuf); 
			if (val != 0 & val < 7) {
				printf("(%i)",val);
				net.SetAF(val-1);
				cwwc(L"(Done)\n");
			}
		}
		if (val == 2) {
			cewc(L"(Enter the count of epochs  )\n");
			command(L">> ", cbuf, cbufs);
			ul epochc = _wtol(cbuf);
			ul epoch = 0;
			while (epoch < epochc) {
				ul trainset = 0;
				while (trainset < 2) {
					int x = 0;
					int y = 0;
					while (x < sx) {
						int y = 0;
						while (y < sy) {
							double input[3] = { (double)x/(double)sx,(double)y / (double)sy,trainset };
							double ideal = 0;
							if (trainset == 0) {
								ideal = circle[x + (y * sx)];
							}
							if (trainset == 1) {
								ideal = square[x + (y * sx)];
							}
							net.SetIdeals(&ideal);
							net.SetInput(input);
							uint32_t iteration = 0;
							while (net.ForwardFeed() > ErrorMinimum & iteration < MaxIteration) {
								net.BackPropogation(BackPropogationLearningSpeed);
								iteration++;
							}
							y++;
						}
						x++;
					}
					trainset++;
				}
				printf("(%i/%i)\n", epoch,epochc);
				epoch++;
			}
			cewc(L"(Learning complete)\n");
		}
		if (val == 3) {
			printf("(Error: %f)\n",net.ForwardFeed());
		}
		if (val == 4) {
			net.PrintAllInfo();
		}
		if (val == 5) {
			net.RandomWeights();
		}
		if (val == 6) {
			SDL_Window* wnd = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,DEFAULT_SRF_SIZEX, DEFAULT_SRF_SIZEY,0);
			SDL_Surface* srf = SDL_GetWindowSurface(wnd);
			SDL_Event ev;
			uint32_t* srf2 = (uint32_t *)malloc(sx*sy*sizeof(uint32_t *));
			while (1) {
				cewc(L"(q - Cancel [0 <= N <= 1] - Test value)\n");
				command(L">> ", cbuf, cbufs);
				double val = _wtof(cbuf);
				if (wcsncmp(cbuf, L"q",1 ) == 0) {	
					break;
				}
				else {
					if (val < 0) {
						val = 0;
					}
					if (val > 1) {
						val = 1;
					}
					int x = 0;
					int y = 0;
					while (x < sx) {
						int y = 0;
						while (y < sy) {
							double input[3] = { (double)x / (double)sx,(double)y / (double)sy,val };
							net.SetInput(input);
							net.ForwardFeed();
							uint32_t* p = &(((uint32_t*)srf2)[x + (y * sx)]);
							*((uint8_t*)(p)+0) = net.neuron_vals[net.arch.lcount - 1][0] * 255;
							*((uint8_t*)(p)+1) = net.neuron_vals[net.arch.lcount - 1][0] * 255;
							*((uint8_t*)(p)+2) = net.neuron_vals[net.arch.lcount - 1][0] * 255;
							*((uint8_t*)(p)+3) = net.neuron_vals[net.arch.lcount - 1][0] * 255;
							y++;
						}
						SDL_PollEvent(&ev);
						x++;
					}
					Resize32(srf2,sx,sy,(uint32_t *)srf->pixels,DEFAULT_SRF_SIZEX,DEFAULT_SRF_SIZEY);
					SDL_UpdateWindowSurface(wnd);
				}
			}
			SDL_DestroyWindow(wnd);
		}
		if (val == 7) {
			cewc(L"(Enter Save Path)\n");
			command(L">> ", cbuf, cbufs);
			net.SaveToFile(cbuf);
		}
		if (val == 8) {
			cewc(L"(Enter Load Path)\n");
			command(L">> ", cbuf, cbufs);
			net.OpenFromFile(cbuf);
		}
		if (val == 9) {
			cewc(L"(Enter a New Error minimum)\n");
			command(L">> ", cbuf, cbufs);
			ErrorMinimum = _wtof(cbuf);
		}
		if (val == 10) {
			cewc(L"(Enter a new Maximum of Iterations)\n");
			command(L">> ", cbuf, cbufs);
			MaxIteration = _wtol(cbuf);
		}
		if (val == 11) {
			cewc(L"(Enter a new Learning speed)\n");
			command(L">> ", cbuf, cbufs);
			BackPropogationLearningSpeed = _wtol(cbuf);
		}
	}
	return 0;
}