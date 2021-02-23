#include "pch.h"
#include "CRandomSearch.h"


CRandomSearch::CRandomSearch(CProblem *cProblem) {
	c_problem = cProblem;
}

CRandomSearch::~CRandomSearch() {
}

CSolution CRandomSearch::cFindSolution(double dTime) {
	CRandom cRand;
	CSolution cMaxQualitySol;
	CSolution cSol(((CMscnProblem*)c_problem)->iGetSupplierAmount(), ((CMscnProblem*)c_problem)->iGetFactoryAmount(),
		((CMscnProblem*)c_problem)->iGetWarehouseAmount(), ((CMscnProblem*)c_problem)->iGetRetailerAmount());
	int iActQuality;
	int iErrCounter = 0;
	bool bSatisfErr = false;
	double dMaxQuality = -DBL_MAX;
	int iNotSatisfied = 0;
	CTimer cTime;
	cTime.vSetTimer(dTime);
	while(cTime.bCheckIfTimePassed()==false){
		cSol.vGenerateSolutionTable(cRand);
		if (((CMscnProblem*)c_problem)->bSetSolution(cSol)) {
			iActQuality = c_problem->dGetQuality();
			if (!c_problem->bConstraintsSatisfied()) {
				iNotSatisfied++;
			}
			if (iActQuality > dMaxQuality) {
				dMaxQuality = iActQuality;
				cMaxQualitySol = cSol;
			}
		}
	}
	((CMscnProblem*)c_problem)->bSetSolution(cMaxQualitySol);
	std::cout << "MAX QUALITY: " <<dMaxQuality << endl;
	std::cout << "NOT SATISFIED: " << iNotSatisfied << endl;
	return cMaxQualitySol;
}