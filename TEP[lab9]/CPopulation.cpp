#include "pch.h"
#include "CPopulation.h"


CPopulation::CPopulation(){
}


CPopulation::~CPopulation(){
}

void CPopulation::vInitPopulation(int iPopSize, CProblem *cProblem, CRandom &cRand, bool &bErrSize) {
	v_population.clear();
	int iSatisfSol = 0;
	if (iPopSize < ci_min_population_size) {
		bErrSize = true;
		return;
	}
	while (iSatisfSol != iPopSize) {
		CSolution cSol(((CMscnProblem*)cProblem)->iGetSupplierAmount(), ((CMscnProblem*)cProblem)->iGetFactoryAmount(),
			((CMscnProblem*)cProblem)->iGetWarehouseAmount(), ((CMscnProblem*)cProblem)->iGetRetailerAmount());
		cSol.vGenerateSolutionTable(cRand);
		if (((CMscnProblem*)cProblem)->bSetSolution(cSol) && cProblem->bConstraintsSatisfied()) {
			v_population.push_back(cSol);
			//cSol.vShowSolTable();
			iSatisfSol++;
		}
	}
}
CSolution CPopulation::cGetRandomInd(CRandom &cRand) {
	int iRandIndex = cRand.iGenerateInt(0, v_population.size()-1);
	return v_population[iRandIndex];
}
 