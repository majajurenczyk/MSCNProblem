#pragma once
#include "CMscnProblem.h"
#include <iostream>
#include <float.h>
#include "CSolution.h"
#include "COptimizer.h"
#include "CTimer.h"
class CRandomSearch : public COptimizer
{
public:
	CRandomSearch(CProblem *cProblem);
	~CRandomSearch();
	CSolution cFindSolution(double dTime);
};

