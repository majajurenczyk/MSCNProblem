#pragma once
#include"CMscnProblem.h";
#include"CPopulation.h";
#include"CSolution.h";
#include "COptimizer.h";
#include"CTimer.h";

class CDiffEvol : public COptimizer
{
public:
	CDiffEvol(CProblem *cProb, double dDiffWeigh, double dCrossProb, int iPopSize);
	~CDiffEvol();
	CSolution cFindSolution(double dTime);

private:
	static bool bErrSize;
	double d_diff_weigh;
	double d_cross_prob;
	int i_population_size;
};

