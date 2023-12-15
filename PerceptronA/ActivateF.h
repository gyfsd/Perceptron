#pragma once
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
double Sigmoid(double);
double ReLu(double);
double Thx(double);
double SigmoidDer(double);
double ReLuDer(double);
double ThxDer(double);
double Linear(double a);
double Softsign(double a);
double SingleStep(double a);
double LinearDer(double a);
double SoftsignDer(double a);
double SingleStepDer(double a);
class ActivateF
{
	int f = 0;
	void * FList[6] = {&Sigmoid,&ReLu,&Thx,&Linear,&Softsign,&SingleStep};
	void* FDList[6] = { &SigmoidDer ,& ReLuDer,& ThxDer,& LinearDer,& SoftsignDer,& SingleStepDer };
	double (*SettedF)(double);
	double (*SettedFD)(double);

public:
	void set(int n);
	void use(double * val,uint32_t n);
	void useDer(double * val, uint32_t n);
	double use(double val);
	double useDer(double val);
};

