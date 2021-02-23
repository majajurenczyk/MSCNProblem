#include "pch.h"
#include "CSolution.h"
#include <string>
#include<iostream>
#include<fstream>

using namespace std;

CSolution::CSolution() {
}

CSolution::CSolution(int iSuppAmount, int iFactAmount, int iWarhAmount, int iRetAmount) {
	i_supp_amount = iSuppAmount;
	i_fact_amount = iFactAmount;
	i_warh_amount = iWarhAmount;
	i_ret_amount = iRetAmount;

	i_solution_size = i_supp_amount * i_fact_amount + i_fact_amount * i_warh_amount + i_warh_amount * i_ret_amount;
	pd_solution = new double[i_solution_size];
}

CSolution::CSolution(const CSolution &cOtherSol) {
	pd_solution = new double[cOtherSol.i_solution_size];
	i_solution_size = cOtherSol.i_solution_size;
	for (int ii = 0; ii < i_solution_size; ii++) {
		pd_solution[ii] = cOtherSol.pd_solution[ii];
	}
	i_supp_amount = cOtherSol.i_supp_amount;
	i_fact_amount = cOtherSol.i_fact_amount;
	i_warh_amount = cOtherSol.i_warh_amount;
	i_ret_amount = cOtherSol.i_ret_amount;
}

CSolution::~CSolution() {
	delete pd_solution;
}

void CSolution::vReadSolutionFromFile(string sFileName, bool &bError) {
	string sText;
	double dData;
	fstream cFile;
	int iPos = 0;;

	cFile.open(sFileName, ios::in);
	if (!cFile.good()) {
		bError = true;
		return;
	}

	cFile >> sText >> i_supp_amount;
	cFile >> sText >> i_fact_amount;
	cFile >> sText >> i_warh_amount;
	cFile >> sText >> i_ret_amount;

	i_solution_size = i_supp_amount * i_fact_amount + i_fact_amount * i_warh_amount + i_warh_amount * i_ret_amount;
	pd_solution = new double[i_solution_size];

	cFile >> sText;
	vReadDataToSolutionTable(cFile, i_supp_amount * i_fact_amount, iPos);
	cFile >> sText;
	vReadDataToSolutionTable(cFile, i_fact_amount * i_warh_amount, iPos);
	cFile >> sText;
	vReadDataToSolutionTable(cFile, i_warh_amount * i_ret_amount, iPos);

	cFile.close();
}


void CSolution::vReadDataToSolutionTable(fstream &cFile, int iAmount, int &iPos) {
	double dData;
	for (int ii = 0; ii < iAmount; ii++) {
		cFile >> dData;
		pd_solution[iPos] = dData;
		iPos++;
	}
}
void CSolution::vGenerateSolutionTable(CRandom &cRand) {
	int iFromSuppToFactProdOffset = 0;
	int iFromFactToWarhProdOffset = i_supp_amount * i_fact_amount;
	int iFromWarhToRetProdOffset = i_supp_amount * i_fact_amount + i_fact_amount * i_warh_amount;
	int iSolutionSize = i_supp_amount * i_fact_amount + i_fact_amount * i_warh_amount + i_warh_amount * i_ret_amount;

	for (int ii = iFromSuppToFactProdOffset; ii < iFromFactToWarhProdOffset; ii++) {
		pd_solution[ii] = cRand.dGenerateDouble(cd_supp_to_fact_minval, cd_supp_to_fact_maxval);
	}
	for (int ii = iFromFactToWarhProdOffset; ii < iFromWarhToRetProdOffset; ii++) {
		pd_solution[ii] = cRand.dGenerateDouble(cd_fact_to_warh_minval, cd_fact_to_warh_maxval);
	}
	for (int ii = iFromWarhToRetProdOffset; ii < iSolutionSize; ii++) {
		pd_solution[ii] = cRand.dGenerateDouble(cd_warh_to_ret_minval, cd_warh_to_ret_maxval);
	}
}

bool CSolution::bAreSolutionsDifferent(CSolution cSol) {
	return pd_solution != cSol.getSolution();
}

void CSolution::vShowSolTable() {
	std::cout << endl;
	for (int ii = 0; ii < i_solution_size; ii++) {
		std::cout << pd_solution[ii] << " ; ";
	}
	std::cout << endl;
}

bool CSolution::bAreSolutionsDifferent(CSolution cSol1, CSolution cSol2, CSolution cSol3) {
	return bAreSolutionsDifferent(cSol1) && bAreSolutionsDifferent(cSol2) && bAreSolutionsDifferent(cSol3)
		&& cSol1.bAreSolutionsDifferent(*this) && cSol1.bAreSolutionsDifferent(cSol2) && cSol1.bAreSolutionsDifferent(cSol3)
		&& cSol2.bAreSolutionsDifferent(*this) && cSol2.bAreSolutionsDifferent(cSol1) && cSol2.bAreSolutionsDifferent(cSol3)
		&& cSol3.bAreSolutionsDifferent(*this) && cSol3.bAreSolutionsDifferent(cSol2) && cSol3.bAreSolutionsDifferent(cSol1);
}
void CSolution::vFixPartSolution(int iStartOff, int iEndOff, double dMinVal, double dMaxVal) {
	for (int ii = iStartOff; ii < iEndOff; ii++) {
		if (pd_solution[ii] > dMaxVal) { 
			pd_solution[ii]=dMaxVal;
		}
		if (pd_solution[ii] < dMinVal) {
			pd_solution[ii]=dMinVal;
		}
	}
}

void CSolution::vFixSolution() {
	int iFromSuppToFactProdOffset = 0;
	int iFromFactToWarhProdOffset = i_supp_amount * i_fact_amount;
	int iFromWarhToRetProdOffset = i_supp_amount * i_fact_amount + i_fact_amount * i_warh_amount;
	int iSolutionSize = i_supp_amount * i_fact_amount + i_fact_amount * i_warh_amount + i_warh_amount * i_ret_amount;

	vFixPartSolution(iFromSuppToFactProdOffset, iFromFactToWarhProdOffset, cd_supp_to_fact_minval, cd_supp_to_fact_maxval);
	vFixPartSolution(iFromFactToWarhProdOffset, iFromWarhToRetProdOffset, cd_fact_to_warh_minval, cd_fact_to_warh_maxval);
	vFixPartSolution(iFromWarhToRetProdOffset, iSolutionSize, cd_warh_to_ret_minval, cd_warh_to_ret_maxval);
}


void CSolution::operator=(const CSolution &cOtherSol) {
	if (this->getSolution() != NULL) {
		delete this->getSolution();
	}
	pd_solution = new double[cOtherSol.i_solution_size];
	i_solution_size = cOtherSol.i_solution_size;
	for (int ii = 0; ii < i_solution_size; ii++) {
		pd_solution[ii] = cOtherSol.pd_solution[ii];
	}
	i_supp_amount = cOtherSol.i_supp_amount;
	i_fact_amount = cOtherSol.i_fact_amount;
	i_warh_amount = cOtherSol.i_warh_amount;
	i_ret_amount = cOtherSol.i_ret_amount;
}
