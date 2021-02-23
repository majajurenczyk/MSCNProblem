#pragma once
#include"CSolution.h";
#include"CProblem.h";
class COptimizer
{
public:
	virtual ~COptimizer();
	virtual CSolution cFindSolution(double dTime) = 0;
protected:
	CProblem* c_problem;
};

