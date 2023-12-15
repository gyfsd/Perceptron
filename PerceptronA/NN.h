#pragma once
#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include <iostream>
#include <stdint.h>
#include "ActivateF.h"
#include "ShortDef.h"
typedef struct NN_Arch {
	ul* lsize;
	ul lcount;
	ul AF;
};
class NN {
private:
	double** neuron_in;
	double* deltas_buf;
	double* deltas_buf2;
	void* memory;
	uint32_t allocsize;
	ul maxlayersize = 0;
	double GetI(ul layer, ul oneuron);
	void ZeroChangeFill();
	void ApplyWeightChanges(double N);
public:
	NN_Arch arch;
	double* ideals;
	double** neuron_vals;
	double*** weight_change;
	double*** neuron_weights;
	double** neuron_errder;
	ActivateF AF;
	void SetAF(ui m);
	void RandomWeights();
	void RandomValues();
	void Init(NN_Arch _arch);
	double ForwardFeed();
	void BackPropogation(double N);
	void SetInput(double* input);
	void SetIdeals(double* ideals_);
	void PrintVals(ul layer);
	void PrintWeights(ul layer);
	void PrintAllInfo();
	void SaveToFile(wchar_t * path);
	void OpenFromFile(wchar_t * path);
};