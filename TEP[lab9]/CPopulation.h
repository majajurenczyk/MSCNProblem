#pragma once
#include<vector>
#include"CSolution.h"
#include"CMscnProblem.h"

class CPopulation
{
public:
	CPopulation();
	~CPopulation();
	void vInitPopulation(int iPopSize, CProblem *cProblem, CRandom &cRand, bool& bErrSize);
	CSolution cGetRandomInd(CRandom &cRand);
	vector<CSolution>* vGetPopulation() {
		return &v_population;
	}
private:
	vector<CSolution> v_population;
	const int ci_min_population_size = 4;
};

