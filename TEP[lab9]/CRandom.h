#pragma once
#include<random>
using namespace std;
class CRandom
{
public:
	CRandom();
	CRandom(int iSeed);
	~CRandom();
	int iGenerateInt(int iMin, int iMax);
	double dGenerateDouble(double iMin, double iMax);
private:
	mt19937 cGenerator;
};

