#pragma once
#include <string>
#include "CRandom.h"
using namespace std;

class CSolution
{
public:
	CSolution();
	CSolution(int iSuppAmount, int iFactAmount, int iWarhAmount, int iRetAmount);
	CSolution(const CSolution &cOtherSol);
	CSolution(double* pdSol, int iSolSize) {
		pd_solution = pdSol;
		i_solution_size = iSolSize;
	}
	~CSolution();
	void operator=(const CSolution &cOtherSol);
	double* getSolution() { return pd_solution;}
	void vReadSolutionFromFile(string sFileName, bool &bError); 
	void vGenerateSolutionTable(CRandom &cRand);
	bool bAreSolutionsDifferent(CSolution cSol1, CSolution cSol2, CSolution cSol3);
	void vFixSolution();
	void vShowSolTable();

	int iGetSolSize() { return i_solution_size;}
	int iGetSuppAmount() { return i_supp_amount; }
	int iGetFactAmount() { return i_fact_amount; }
	int iGetWarhAmount() { return i_warh_amount; }
	int iGetRetAmount() { return i_supp_amount; }

private:
	double* pd_solution;
	int i_solution_size;
	int i_supp_amount;
	int i_fact_amount;
	int i_warh_amount;
	int i_ret_amount;
	void vReadDataToSolutionTable(fstream &cFile, int iAmount, int &iPos);
	bool bAreSolutionsDifferent(CSolution cSol);
	void vFixPartSolution(int iStartOff, int iEndOff, double dMinVal, double dMaxVal);

	const double cd_supp_to_fact_minval = 1000;
	const double cd_supp_to_fact_maxval = 2000;

	const double cd_fact_to_warh_minval = 100;
	const double cd_fact_to_warh_maxval = 200;

	const double cd_warh_to_ret_minval = 10;
	const double cd_warh_to_ret_maxval = 20;

};

