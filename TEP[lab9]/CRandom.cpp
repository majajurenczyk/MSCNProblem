#include "pch.h"
#include "CRandom.h"
#include<random>


CRandom::CRandom() {
	random_device rd;
	cGenerator.seed(rd());
}

CRandom::CRandom(int iSeed) {
	cGenerator.seed(iSeed);
}

CRandom::~CRandom() {
}

int CRandom::iGenerateInt(int iMin, int iMax) {
	uniform_int_distribution<> cDistr(iMin, iMax);
	return cDistr(cGenerator);
}

double CRandom::dGenerateDouble(double iMin, double iMax) {
	uniform_real_distribution<> cDistr(iMin, iMax);
	return cDistr(cGenerator);
}