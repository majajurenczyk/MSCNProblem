#include "pch.h"
#include "CDiffEvol.h"
#include<iostream>

bool CDiffEvol::bErrSize = false;

CDiffEvol::CDiffEvol(CProblem *cProb, double dDiffWeigh, double dCrossProb, int iPopSize) {
	c_problem = cProb;
	d_diff_weigh = dDiffWeigh;
	d_cross_prob = dCrossProb;
	i_population_size = iPopSize;	
}


CDiffEvol::~CDiffEvol()
{
}

CSolution CDiffEvol::cFindSolution(double dTime) {
	CRandom cRand;
	CPopulation cPop;
	cPop.vInitPopulation(i_population_size, c_problem, cRand, bErrSize);
	CTimer cTime;
	cTime.vSetTimer(dTime);
	while (cTime.bCheckIfTimePassed()==false) {
		for (size_t ii = 0; ii < cPop.vGetPopulation()->size(); ii++) {

			CSolution cBaseInd = cPop.cGetRandomInd(cRand);
			CSolution cAddInd0 = cPop.cGetRandomInd(cRand);
			CSolution cAddInd1 = cPop.cGetRandomInd(cRand);

			if (cPop.vGetPopulation()->at(ii).bAreSolutionsDifferent(cBaseInd, cAddInd0, cAddInd1)) {

				CSolution cNewInd(((CMscnProblem*)c_problem)->iGetSupplierAmount(), ((CMscnProblem*)c_problem)->iGetFactoryAmount(),
					((CMscnProblem*)c_problem)->iGetWarehouseAmount(), ((CMscnProblem*)c_problem)->iGetRetailerAmount());

				cNewInd = cPop.vGetPopulation()->at(ii);

				for (int iGenOff = 0; iGenOff < cPop.vGetPopulation()->at(ii).iGetSolSize(); iGenOff++) {

					if (cRand.iGenerateInt(0, 1) < d_cross_prob) {
						cNewInd.getSolution()[iGenOff] = cBaseInd.getSolution()[iGenOff] +
							d_diff_weigh * (cAddInd0.getSolution()[iGenOff] - cAddInd1.getSolution()[iGenOff]);
					}
					else {
						cNewInd.getSolution()[iGenOff] = cPop.vGetPopulation()->at(ii).getSolution()[iGenOff];
					}
				}
			
				((CMscnProblem*)c_problem)->bSetSolution(cPop.vGetPopulation()->at(ii));
				double iActQual = c_problem->dGetQuality();
				((CMscnProblem*)c_problem)->bSetSolution(cNewInd);
				double iActQualNew = c_problem->dGetQuality();
				if (iActQualNew < iActQual) {
					((CMscnProblem*)c_problem)->bSetSolution(cPop.vGetPopulation()->at(ii));
				}
				else {
					(*cPop.vGetPopulation())[ii] = cNewInd;
				}
			}
		}
	}
	return ((CMscnProblem*)c_problem)->cGetSolution();
}
