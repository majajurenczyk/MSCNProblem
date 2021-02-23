#pragma once
#include "COptimizer.h"
#include "CMscnProblem.h"
class CTrivMethod :
	public COptimizer
{
public:
	CTrivMethod(CProblem* cProb);
	~CTrivMethod();
	CSolution cFindSolution(double dTime);
};

