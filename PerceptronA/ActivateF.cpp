#include "ActivateF.h"
double Sigmoid(double a) {
	return 1 / (1 + exp(-a));
	//return 1 / (1 + pow(2.7,-a));
}
double ReLu(double a) {
	if (a > 1) {
		a = 1;
	}
	if (a < -1) {
		a = -1;
	}
	if (a < 0) {
		return 0;
	}
	if (a >= 0) {
		return a;
	}
}
double Thx(double a) {
	return (exp(a)-exp(-a)) / (exp(a)+exp(-a));
}
double Linear(double a) {
	return a;
}
double Softsign(double a) {
	return a / (1 + fabs(a));
}
double SingleStep(double a) {
	if (a < 0) {
		return 0;
	}
	if (a >= 0) {
		return 1;
	}
}
double SigmoidDer(double a) {
	return a * (1 - a);
}
double ReLuDer(double a) {
	if (a > 1) {
		a = 1;
	}
	if (a < -1) {
		a = -1;
	}
	if (a < 0) {
		return 0;
	}
	if (a >= 0) {
		return 1;
	}
}
double ThxDer(double a) {
	return 1 / (exp(a) + exp(-a));
}
double LinearDer(double a) {
	return 1;
}
double SoftsignDer(double a) {
	return ((1 + fabs(a)) - a) / (pow(1 + fabs(a),2));

}
double SingleStepDer(double a) {
	return 0;
}
void ActivateF::set(int n) {
		f = n;
		SettedF = (double (*)(double))FList[n];
		SettedFD = (double (*)(double))FDList[n];
}
void ActivateF::use(double * val,uint32_t n) {
	uint32_t cur = 0;
	while (cur < n) {
		val[cur] = (SettedF)(val[cur]);
		cur++;
	}
}
void ActivateF::useDer(double* val, uint32_t n) {
	uint32_t cur = 0;
	while (cur < n) {
		val[cur] = (SettedFD)(val[cur]);
		cur++;
	}
}

double ActivateF::use(double val) {
	return SettedF(val);
}
double ActivateF::useDer(double val) {
	return SettedFD(val);
}
