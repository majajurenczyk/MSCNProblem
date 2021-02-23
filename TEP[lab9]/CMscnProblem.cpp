#include "pch.h"
#include "CMscnProblem.h"
#include <iostream>
#include<string>
#include<vector>
#include<fstream>
using namespace std;

CMscnProblem::CMscnProblem() {
}

CMscnProblem::~CMscnProblem() {
}

//PUBLIC SETTERS

bool CMscnProblem::bSetSuppliers(int iSuppAmount) {
	if (iSuppAmount <= 0) return false;
	i_supplier_amount = iSuppAmount;
	v_supp_max_product.resize(iSuppAmount);
	v_supp_one_payment.resize(iSuppAmount);
	vv_supp_to_fact_cost.resize(iSuppAmount);
	vv_supp_restr.resize(2 * iSuppAmount);
	return true;
}

bool CMscnProblem::bSetFactories(int iFactAmount) {
	if (iFactAmount <= 0) return false;

	i_factory_amount = iFactAmount;
	v_fact_max_product.resize(iFactAmount);
	v_fact_one_payment.resize(iFactAmount);

	vResizeMatrixCol(vv_supp_to_fact_cost, iFactAmount);
	vResizeMatrixCol(vv_supp_restr, iFactAmount);

	vv_fact_to_warh_cost.resize(iFactAmount);
	vv_fact_restr.resize(2 * iFactAmount);

	return true;
}

bool CMscnProblem::bSetWarehouses(int iWarhAmount) {
	if (iWarhAmount <= 0) return false;

	i_warehouse_amount = iWarhAmount;
	v_warh_capacity.resize(iWarhAmount);
	v_warh_one_payment.resize(iWarhAmount);

	vResizeMatrixCol(vv_fact_to_warh_cost, iWarhAmount);
	vResizeMatrixCol(vv_fact_restr, iWarhAmount);

	vv_warh_to_ret_cost.resize(iWarhAmount);
	vv_warh_restr.resize(2 * iWarhAmount);

	return true;
}

bool CMscnProblem::bSetRetailers(int iRetAmount) {
	if (iRetAmount <= 0) return false;

	i_retailer_amount = iRetAmount;
	v_ret_demand.resize(iRetAmount);
	v_ret_income.resize(iRetAmount);

	vResizeMatrixCol(vv_warh_to_ret_cost, iRetAmount);
	vResizeMatrixCol(vv_warh_restr, iRetAmount);

	return true;
}

bool CMscnProblem::bSetSuppMaxProd(int iIndex, double dValue) {
	return setValueInVector(v_supp_max_product, iIndex, dValue);
}

bool CMscnProblem::bSetFactMaxProd(int iIndex, double dValue) {
	return setValueInVector(v_fact_max_product, iIndex, dValue);
}

bool CMscnProblem::bSetWarhCap(int iIndex, double dValue) {
	return setValueInVector(v_warh_capacity, iIndex, dValue);
}
bool CMscnProblem::bSetRetDem(int iIndex, double dValue) {
	return setValueInVector(v_ret_demand, iIndex, dValue);
}


bool CMscnProblem::bSetSuppToFactCost(int iIndexX, int iIndexY, double dValue) {
	return setValueInMatrix(vv_supp_to_fact_cost, iIndexX, iIndexY, dValue);
}
bool CMscnProblem::bSetFactToWarhCost(int iIndexX, int iIndexY, double dValue) {
	return setValueInMatrix(vv_fact_to_warh_cost, iIndexX, iIndexY, dValue);
}
bool CMscnProblem::bSetWarhToRetCost(int iIndexX, int iIndexY, double dValue) {
	return setValueInMatrix(vv_warh_to_ret_cost, iIndexX, iIndexY, dValue);
}


bool CMscnProblem::bSetSuppOnePay(int iIndex, double dValue) {
	return setValueInVector(v_supp_one_payment, iIndex, dValue);
}
bool CMscnProblem::bSetFactOnePay(int iIndex, double dValue) {
	return setValueInVector(v_fact_one_payment, iIndex, dValue);
}
bool CMscnProblem::bSetWarhOnePay(int iIndex, double dValue) {
	return setValueInVector(v_warh_one_payment, iIndex, dValue);
}


bool CMscnProblem::bSetRetInc(int iIndex, double dValue) {
	return setValueInVector(v_ret_income, iIndex, dValue);
}

bool CMscnProblem::bSetSuppRestr(int iIndexX, int iIndexY, double dValue) {
	return setValueInMatrix(vv_supp_restr, iIndexX, iIndexY, dValue);
}
bool CMscnProblem::bSetFactRestr(int iIndexX, int iIndexY, double dValue) {
	return setValueInMatrix(vv_fact_restr, iIndexX, iIndexY, dValue);
}
bool CMscnProblem::bSetWarhRestr(int iIndexX, int iIndexY, double dValue) {
	return setValueInMatrix(vv_warh_restr, iIndexX, iIndexY, dValue);
}

//PRIVATE FOR SETTERS

void CMscnProblem::vResizeMatrixCol(vector<vector<double>> &vvMatrix, int iNewSize) {
	for (size_t ii = 0; ii < vvMatrix.size(); ii++) {
		vvMatrix.at(ii).resize(iNewSize);
	}
}

bool CMscnProblem::setValueInVector(vector<double>&vVector, int iIndex, double dValue) {
	if (iIndex < 0 || iIndex >= vVector.size() || dValue <= 0) return false;
	vVector[iIndex] = dValue;
	return true;
}

bool CMscnProblem::setValueInMatrix(vector<vector<double>>&vvMatrix, int iIndexX, int iIndexY, double dValue) {
	if (iIndexX < 0 || iIndexX >= vvMatrix.size() || iIndexY<0 || iIndexY>=vvMatrix.at(0).size() || dValue <= 0) return false;
	vvMatrix[iIndexX][iIndexY] = dValue;
	return true;

}

//SET SOLUTION

bool CMscnProblem::bSetSolution(CSolution sol) {
	if (bCheckSolution(sol)) {
		solution = sol;
		return true;
	}
	return false;
}

void CMscnProblem::vDeleteSolution() {
	CSolution sol;
	solution = sol;
}

//GET QUALITY

double CMscnProblem::dGetQuality() {
	int iShopsIncome = 0;
	int iDealCosts = 0;
	int iTransportCosts = 0;

	int iSuppProdStartOffset = 0;
	int iFactProdStartOffset = i_supplier_amount * i_factory_amount;
	int iWarhProdStartOffset = i_supplier_amount * i_factory_amount + i_factory_amount * i_warehouse_amount;

	if (!bConstraintsSatisfied()) {
		vFixAmountConstraints(i_supplier_amount, i_factory_amount, v_supp_max_product, iSuppProdStartOffset);
		vFixAmountConstraints(i_factory_amount, i_warehouse_amount, v_fact_max_product, iFactProdStartOffset);
		vFixAmountConstraints(i_warehouse_amount, i_retailer_amount, v_warh_capacity, iWarhProdStartOffset);
		vFixAmountRetConstraints(i_warehouse_amount, i_retailer_amount, v_ret_demand, iWarhProdStartOffset);
		vFixSumConstraints(i_supplier_amount, i_factory_amount, i_warehouse_amount, iSuppProdStartOffset, iFactProdStartOffset);
		vFixSumConstraints(i_factory_amount, i_warehouse_amount, i_retailer_amount, iFactProdStartOffset, iWarhProdStartOffset);
	}

	iShopsIncome = dShopsIncome(iWarhProdStartOffset);

	iDealCosts = dOnePaymentCosts(i_supplier_amount, i_factory_amount, v_supp_one_payment, iSuppProdStartOffset) +
		dOnePaymentCosts(i_factory_amount, i_warehouse_amount, v_fact_one_payment, iFactProdStartOffset) +
		dOnePaymentCosts(i_warehouse_amount, i_retailer_amount, v_warh_one_payment, iWarhProdStartOffset);

	iTransportCosts = dProdTransportCosts(i_supplier_amount, i_factory_amount, vv_supp_to_fact_cost, iSuppProdStartOffset) +
		dProdTransportCosts(i_factory_amount, i_warehouse_amount, vv_fact_to_warh_cost, iFactProdStartOffset) +
		dProdTransportCosts(i_warehouse_amount, i_retailer_amount, vv_warh_to_ret_cost, iWarhProdStartOffset);

	return iShopsIncome - iDealCosts - iTransportCosts;

}

//PRIVATE FOR GET QUALITY

double CMscnProblem::dShopsIncome(int iOffset) {
	double iShopsIncome = 0;
	int iWarhProdStartOffset = iOffset;
	for (int ii = 0; ii < i_warehouse_amount; ii++) {
		for (int ij = 0; ij < i_retailer_amount; ij++) {
			iShopsIncome = iShopsIncome + v_ret_income.at(ij)*solution.getSolution()[iWarhProdStartOffset++];
		}
	}
	return iShopsIncome;
}

double CMscnProblem::dProdTransportCosts(int iSourceAmount, int iDestAmount, vector<vector<double>>vvSourceToDestCosts, int iOffset) {
	int iProdOffset = iOffset;
	double dTransCosts = 0;
	for (int ii = 0; ii < iSourceAmount; ii++) {
		for (int ij = 0; ij < iDestAmount; ij++) {
			dTransCosts = dTransCosts + solution.getSolution()[iProdOffset++] * vvSourceToDestCosts.at(ii).at(ij);
		}
	}
	return dTransCosts;
}

double CMscnProblem::dOnePaymentCosts(int iSourceAmount, int iDestAmount, vector<double> vSourcePayCosts, int iStartIndex) {
	int iDealCosts = 0;
	int iSuppProdStartIndex = iStartIndex;

	for (int ii = 0; ii < iSourceAmount; ii++) {
		int iSumProd = 0;
		for (int ij = 0; ij < iDestAmount; ij++) {
			iSumProd = iSumProd + solution.getSolution()[iSuppProdStartIndex++];
		}
		if (iSumProd > 0) {
			iDealCosts = iDealCosts + vSourcePayCosts[ii];
		}
	}
	return iDealCosts;
}


bool CMscnProblem::bCheckSolution(CSolution CSol) {
	if (CSol.getSolution() == NULL|| i_supplier_amount!= CSol.iGetSuppAmount() || i_factory_amount != CSol.iGetFactAmount()
		|| i_warehouse_amount != CSol.iGetWarhAmount() || i_retailer_amount != CSol.iGetRetAmount()) {
		return false;
	}
	for (int ii = 0; ii < CSol.iGetSolSize(); ii++) {
		if (CSol.getSolution()[ii] < 0) {
			return false;
		}
	}
	return true;
}

//CONSTRAINTS SATISFIED

bool CMscnProblem::bConstraintsSatisfied() {
	
	if (!bCheckSolutionConstraints()) {
		return false;
	}

	int iSuppProdStartOffset = 0;
	int iFactProdStartOffset = i_supplier_amount * i_factory_amount;
	int iWarhProdStartOffset = i_supplier_amount * i_factory_amount + i_factory_amount * i_warehouse_amount;

	bool bCon1 = dCheckAmountContraints(i_supplier_amount, i_factory_amount, v_supp_max_product, iSuppProdStartOffset);
	bool bCon2 = dCheckAmountContraints(i_factory_amount, i_warehouse_amount, v_fact_max_product, iFactProdStartOffset);
	bool bCon3 = dCheckAmountContraints(i_warehouse_amount, i_retailer_amount, v_warh_capacity, iWarhProdStartOffset);
	bool bCon4 = dCheckAmountRetContraints(i_warehouse_amount, i_retailer_amount, v_ret_demand, iWarhProdStartOffset);
	bool bCon5 = dCheckSumConstraints(i_supplier_amount, i_factory_amount, i_warehouse_amount, iSuppProdStartOffset, iFactProdStartOffset);
	bool bCon6 = dCheckSumConstraints(i_factory_amount, i_warehouse_amount, i_retailer_amount, iFactProdStartOffset, iWarhProdStartOffset);

	return (bCon1 && bCon2 && bCon3 && bCon4 && bCon5 && bCon6);
}

//PRIVATE FOR CONSTRAINTS SATISFIED

bool CMscnProblem::dCheckAmountRetContraints(int iWrhAmount, int iRetAmount, vector<double> vDemConstraints, int iProdOffset) {
	for (int ii = 0; ii < iRetAmount; ii++) {
		int iSumProducts = 0;
		for (int ij = 0; ij < iWrhAmount; ij++) {
			iSumProducts = iSumProducts + solution.getSolution()[iProdOffset + ij * iRetAmount + ii];
		}
		if (iSumProducts >= vDemConstraints[ii]) return false;
	}
	return true;
}

bool CMscnProblem::dCheckAmountContraints(int iSourceAmount, int iDestAmount, vector<double> vProdConstraints, int iProdOffset) {
	int iOffset = iProdOffset;
	for (int ii = 0; ii < iSourceAmount; ii++) {
		int iSumProducts = 0;
		for (int ij = 0; ij < iDestAmount; ij++) {
			iSumProducts = iSumProducts + solution.getSolution()[iProdOffset++];
		}
		if (iSumProducts >= vProdConstraints[ii]) return false;
	}
	return true;
}

void CMscnProblem::vFixAmountConstraints(int iSourceAmount, int iDestAmount, vector<double> vProdConstraints, int iProdOffset) {
	for (int ii = 0; ii < iSourceAmount; ii++) {
		int iSumProducts = 0;
		for (int ij = 0; ij < iDestAmount; ij++) {
			if (iSumProducts + solution.getSolution()[iProdOffset + ii * iDestAmount + ij] > vProdConstraints[ii]) {
				solution.getSolution()[iProdOffset + ii * iDestAmount + ij] = vProdConstraints[ii] - iSumProducts;
			}
			iSumProducts = iSumProducts + solution.getSolution()[iProdOffset + ii * iDestAmount + ij];
		}
	}
}

void CMscnProblem::vFixAmountRetConstraints(int iWrhAmount, int iRetAmount, vector<double> vDemConstraints, int iProdOffset) {
	for (int ii = 0; ii < iRetAmount; ii++) {
		int iSumProducts = 0;
		for (int ij = 0; ij < iWrhAmount; ij++) {
			if (iSumProducts + solution.getSolution()[iProdOffset + ij * iRetAmount + ii] > vDemConstraints[ii]) {
				solution.getSolution()[iProdOffset + ij * iRetAmount + ii] = vDemConstraints[ii] - iSumProducts;
			}
			iSumProducts = iSumProducts + solution.getSolution()[iProdOffset + ij * iRetAmount + ii];
		}
		
	}
}

bool CMscnProblem::dCheckSumConstraints(int iSourceAmount, int iHalfStopAmount, int iDestAmount, int iFstProdOffset, int iSndProdOffset) {
	for (int ii = 0; ii < iHalfStopAmount; ii++) {
		double iFstSum = 0;
		for (int ij = 0; ij < iSourceAmount; ij++) {
			iFstSum = iFstSum + solution.getSolution()[iFstProdOffset + ij * iHalfStopAmount + ii];
		}
		double iSndSum = 0;
		for (int ij = 0; ij < iDestAmount; ij++) {
			iSndSum = iSndSum + solution.getSolution()[iSndProdOffset + ii * iDestAmount + ij];
		}
		if (iSndSum > iFstSum) {
			return false;
		}
	}
	return true;
}

void CMscnProblem::vFixSumConstraints(int iSourceAmount, int iHalfStopAmount, int iDestAmount, int iFstProdOffset, int iSndProdOffset) {
	for (int ii = 0; ii < iHalfStopAmount; ii++) {
		double iFstSum = 0;
		for (int ij = 0; ij < iSourceAmount; ij++) {
			iFstSum = iFstSum + solution.getSolution()[iFstProdOffset + ij * iHalfStopAmount + ii];
		}
		double iSndSum = 0;
		for (int ij = 0; ij < iDestAmount; ij++) {
			if (iSndSum + solution.getSolution()[iSndProdOffset + ii * iDestAmount + ij] > iFstSum) {
				solution.getSolution()[iSndProdOffset + ii * iDestAmount + ij] = iFstSum - iSndSum;
			}
			iSndSum = iSndSum + solution.getSolution()[iSndProdOffset + ii * iDestAmount + ij];
		}
	}
}


//MIN MAX VALUE FOR ELEMENTS IN SOLUTION TABLE

double CMscnProblem::dMaxValueOfProduct(int iIndex, bool&bError) {
	if (iIndex < 0 || iIndex >= solution.iGetSolSize()) {
		bError = true;
		return 0;
	}
	return dpMinMaxValueOfProduct(iIndex)[1];
}

double CMscnProblem::dMinValueOfProduct(int iIndex, bool&bError) {
	if (iIndex < 0 || iIndex >= solution.iGetSolSize()) {
		bError = true;
		return 0;
	}
	return dpMinMaxValueOfProduct(iIndex)[0];
}

//PRIVATE FOR MINMAX

double* CMscnProblem::dpMinMaxValueOfProduct(int iIndex) {
	double dResultArray[2];
	int iIndY;
	int iIndX;
	if (iIndex < i_supplier_amount*i_factory_amount) {
		iIndY = iIndex % i_factory_amount;
		iIndX = (iIndex - iIndY) / i_factory_amount;
		dResultArray[0] = vv_supp_restr.at(2 * iIndX).at(iIndY);
		dResultArray[1] = vv_supp_restr.at(2 * iIndX + 1).at(iIndY);
	}
	else if (iIndex < (i_supplier_amount*i_factory_amount + i_factory_amount * i_warehouse_amount)) {
		iIndY = (iIndex - i_supplier_amount * i_factory_amount)%i_warehouse_amount;
		iIndX = iIndX = (iIndex - i_supplier_amount * i_factory_amount - iIndY) / i_warehouse_amount;
		dResultArray[0] = vv_fact_restr.at(2 * iIndX).at(iIndY);
		dResultArray[1] = vv_fact_restr.at(2 * iIndX + 1).at(iIndY);
	}
	else {
		iIndY = (iIndex - (i_supplier_amount * i_factory_amount + i_factory_amount * i_warehouse_amount))%i_retailer_amount;
		iIndX = (iIndex - (i_supplier_amount * i_factory_amount + i_factory_amount * i_warehouse_amount) - iIndY) / i_retailer_amount;
		dResultArray[0] = vv_warh_restr.at(2 * iIndX).at(iIndY);
		dResultArray[1] = vv_warh_restr.at(2 * iIndX + 1).at(iIndY);
	}
	return dResultArray;
}

//CHECK IF SOLUTION ELEMENT IS IN MINMAX BOUNDS

bool CMscnProblem::bCheckSolutionConstraints() {
	bool bErr = false;
	for (int ii = 0; ii < solution.iGetSolSize(); ii++) {
		if (solution.getSolution()[ii] > dMaxValueOfProduct(ii, bErr) ||
			solution.getSolution()[ii] < dMinValueOfProduct(ii, bErr)) {
			return false;
		}
	}
	return true;
}

//WRITE PROBLEM DATA TO FILE

bool CMscnProblem::bWriteProblemFile(string sFileName) {
	FILE * pf_file = new FILE;
	pf_file = fopen(sFileName.c_str(), "w+");
	if (pf_file == NULL) return false;

	string sData = "";
	sData += "D " + to_string(i_supplier_amount) + "\n";
	sData += "F " + to_string(i_factory_amount) + "\n";
	sData += "M " + to_string(i_warehouse_amount) + "\n";
	sData += "S " + to_string(i_retailer_amount) + "\n";

	sData += "sd\n";
	sData += sVectorToString(v_supp_max_product);
	sData += "sf\n";
	sData += sVectorToString(v_fact_max_product);
	sData += "sm\n";
	sData += sVectorToString(v_warh_capacity);
	sData += "ss\n";
	sData += sVectorToString(v_ret_demand);

	sData += "cd\n";
	sData += sMatrixToString(vv_supp_to_fact_cost);
	sData += "cf\n";
	sData += sMatrixToString(vv_fact_to_warh_cost);
	sData += "cm\n";
	sData += sMatrixToString(vv_warh_to_ret_cost);

	sData += "ud\n";
	sData += sVectorToString(v_supp_one_payment);
	sData += "uf\n";
	sData += sVectorToString(v_fact_one_payment);
	sData += "um\n";
	sData += sVectorToString(v_warh_one_payment);
	sData += "p\n";
	sData += sVectorToString(v_ret_income);

	sData += "xdminmax\n";
	sData += sMatrixToString(vv_supp_restr);
	sData += "xfminmax\n";
	sData += sMatrixToString(vv_fact_restr);
	sData += "xmminmax\n";
	sData += sMatrixToString(vv_warh_restr);

	fprintf(pf_file, sData.c_str());
	
	if (fclose(pf_file) == EOF) return false;
	return true;
}

//PRIVATE TO WRITING DATA TO FILE

string CMscnProblem::sVectorToString(vector<double> vVect) {
	string sData = "";
	for (size_t ii = 0; ii < vVect.size(); ii++) {
		sData = sData + to_string(vVect.at(ii)) + " ";
	}
	sData += "\n";
	return sData;
}

string CMscnProblem::sMatrixToString(vector<vector<double>> vvMatrix) {
	string sData = "";
	for (size_t ii = 0; ii < vvMatrix.size(); ii++) {
		for (size_t ij = 0; ij < vvMatrix.at(0).size(); ij++) {
			sData = sData + to_string(vvMatrix.at(ii).at(ij)) + " ";
		}
	}
	sData += "\n";
	return sData;
}

//OPEN PROBLEM FILE AND GET DATA

bool CMscnProblem::bOpenProblemFile(string sFileName) {
	string sText;
	int iData;

	fstream cFile;
	cFile.open(sFileName, ios::in);
	if (!cFile.good()) return false;

	cFile >> sText >> iData;
	bSetSuppliers(iData);
	cFile >> sText >> iData;
	bSetFactories(iData);
	cFile >> sText >> iData;
	bSetWarehouses(iData);
	cFile >> sText >> iData;
	bSetRetailers(iData);

	cFile >> sText;;
	vReadVectorFromFile(cFile, i_supplier_amount, v_supp_max_product);
	cFile >> sText;;
	vReadVectorFromFile(cFile, i_factory_amount, v_fact_max_product);
	cFile >> sText;;
	vReadVectorFromFile(cFile, i_warehouse_amount, v_warh_capacity);
	cFile >> sText;;
	vReadVectorFromFile(cFile, i_retailer_amount, v_ret_demand);

	cFile >> sText;;
	vReadMatrixFromFile(cFile, i_supplier_amount, i_factory_amount, vv_supp_to_fact_cost);
	cFile >> sText;;
	vReadMatrixFromFile(cFile, i_factory_amount, i_warehouse_amount, vv_fact_to_warh_cost);
	cFile >> sText;;
	vReadMatrixFromFile(cFile, i_warehouse_amount, i_retailer_amount, vv_warh_to_ret_cost);

	cFile >> sText;;
	vReadVectorFromFile(cFile,i_supplier_amount, v_supp_one_payment);
	cFile >> sText;;
	vReadVectorFromFile(cFile, i_factory_amount, v_fact_one_payment);
	cFile >> sText;;
	vReadVectorFromFile(cFile, i_warehouse_amount, v_warh_one_payment);
	cFile >> sText;;
	vReadVectorFromFile(cFile, i_retailer_amount, v_ret_income);

	cFile >> sText;;
	vReadMatrixFromFile(cFile, 2*i_supplier_amount, i_factory_amount, vv_supp_restr);
	cFile >> sText;;
	vReadMatrixFromFile(cFile, 2*i_factory_amount, i_warehouse_amount, vv_fact_restr);
	cFile >> sText;;
	vReadMatrixFromFile(cFile, 2*i_warehouse_amount, i_retailer_amount, vv_warh_restr);

	cFile.close();
	return true;
}

//PRIVATE FOR GETTING DATA FROM FILE

void CMscnProblem::vReadVectorFromFile(fstream &cFile, int iSize, vector<double> &vVect) {
	for (size_t ii = 0; ii < iSize; ii++) {
		cFile >> vVect[ii];
	}
}

void CMscnProblem::vReadMatrixFromFile(fstream &cFile, int iSizeX, int iSizeY, vector<vector<double>> &vvMatrix) {
	for (size_t ii = 0; ii < iSizeX; ii++) {
		for (size_t ij = 0; ij < iSizeY; ij++) {
			cFile >> vvMatrix[ii][ij];
		}
	}
}


//PRINT INFO

void CMscnProblem::vPrint() {
	cout <<"DANE\n";
	cout << "suppliers: " << i_supplier_amount<<endl;
	cout << "factories: " << i_factory_amount << endl;
	cout << "warehouses: " << i_warehouse_amount << endl;
	cout << "retailers: " << i_retailer_amount << endl;
	cout << "\nSUPPLIERS MAX PRODUCT: \n";
	vPrintTable(v_supp_max_product);
	cout << "\nFACTORIES MAX PRODUCT: \n";
	vPrintTable(v_fact_max_product);
	cout << "\nWAREHOUSES CAPACITY: \n";
	vPrintTable(v_warh_capacity);
	cout << "\nRETAILERS DEMAND: \n";
	vPrintTable(v_ret_demand);
	cout << "\n=======================\n";
	cout << "\nONE PAYMENT FOR SUPPLIER: \n";
	vPrintTable(v_supp_one_payment);
	cout << "\nONE PAYMENT FOR WAREHOUSE: \n";
	vPrintTable(v_warh_one_payment);
	cout << "\nONE PAYMENT FOR FACTORY: \n";
	vPrintTable(v_fact_one_payment);
	cout << "\n=======================\n";
	cout << "\nRETAILERS INCOME: \n";
	vPrintTable(v_ret_income);
	cout << "\n=======================\n";
	cout << "\nSUPPLIER TO FACTORY COSTS: \n";
	vPrintMatrix(vv_supp_to_fact_cost);
	cout << "\nFACTORY TO WAREHOUSE COSTS: \n";
	vPrintMatrix(vv_fact_to_warh_cost);
	cout << "\nWARHOUSE TO RETAILER COSTS: \n";
	vPrintMatrix(vv_warh_to_ret_cost);
	cout << "\n=======================\n";
	cout << "\nSUPPLIERS TO FACTORIES RESTRICTION: \n";
	vPrintMatrix(vv_supp_restr);
	cout << "\nFACTORIES TO WAREHOUSES RESTRICTION: \n";
	vPrintMatrix(vv_fact_restr);
	cout << "\nWAREHOUSES TO RETAILERS RESTRICTION: \n";
	vPrintMatrix(vv_warh_restr);
}

//PRIVATE FOR PRINT

void CMscnProblem::vPrintTable(vector<double> vTable) {
	for (size_t ii = 0; ii < vTable.size(); ii++) {
		cout << vTable.at(ii) << " ";
	}
}

void CMscnProblem::vPrintMatrix(vector<vector<double>> vvMatrix) {
	for (size_t ii = 0; ii < vvMatrix.size(); ii++) {
		for (size_t ij = 0; ij < vvMatrix.at(ii).size(); ij++) {
			cout << vvMatrix.at(ii).at(ij) << " ";
		}
		cout << endl;
	}
}

//LIST 10
void CMscnProblem::vFillVectorRandom(vector<double>& vVector, double dMinVal, double dMaxVal, CRandom& cRand) {
	for (size_t ii=0; ii < vVector.size(); ii++) {
		vVector[ii] = cRand.dGenerateDouble(dMinVal, dMaxVal);
	}
}

void CMscnProblem::vFillMatrixRandom(vector<vector<double>>& vvMatrix, double dMinVal, double dMaxVal, CRandom& cRand) {
	for (size_t ii = 0; ii < vvMatrix.size(); ii++) {
		for (size_t ij = 0; ij < vvMatrix.at(0).size(); ij++) {
			vvMatrix[ii][ij] = cRand.dGenerateDouble(dMinVal, dMaxVal);
		}
	}
}

void CMscnProblem::vFillRestrMatrixRandom(vector<vector<double>>& vvMatrix, double dMinMinVal, double dMinMaxVal,double dMaxMaxVal, CRandom& cRand) {
	for (size_t ij = 0; ij < vvMatrix.at(0).size(); ij++) {
		for (size_t ii = 0; ii < vvMatrix.size(); ii = ii + 2) {
			vvMatrix[ii][ij] = cRand.dGenerateDouble(dMinMinVal, dMinMaxVal);
			vvMatrix[ii + 1][ij] = cRand.dGenerateDouble(dMinMaxVal, dMaxMaxVal);
		}
	}
}

void CMscnProblem::vGenerateInstance(int iInstanceSeed) {
	CRandom cRandGen(iInstanceSeed);

	vFillMatrixRandom(vv_supp_to_fact_cost, cdMinCost, cdMaxCost, cRandGen); 
	vFillMatrixRandom(vv_fact_to_warh_cost, cdMinCost, cdMaxCost, cRandGen);
	vFillMatrixRandom(vv_warh_to_ret_cost, cdMinCost, cdMaxCost, cRandGen);

	vFillVectorRandom(v_supp_one_payment, cdMinOnePayCost, cdMaxOnePayCost, cRandGen); 
	vFillVectorRandom(v_fact_one_payment, cdMinOnePayCost, cdMaxOnePayCost, cRandGen);
	vFillVectorRandom(v_warh_one_payment, cdMinOnePayCost, cdMaxOnePayCost, cRandGen);

	vFillVectorRandom(v_ret_income, cdMinRetIncome, cdMaxRetIncome, cRandGen); 

	vFillVectorRandom(v_supp_max_product, cdMinRestr, cdMaxRestr, cRandGen); 
	vFillVectorRandom(v_fact_max_product, cdMinRestr, cdMaxRestr, cRandGen);
	vFillVectorRandom(v_warh_capacity, cdMinRestr, cdMaxRestr, cRandGen);
	vFillVectorRandom(v_ret_demand, cdMinRestr, cdMaxRestr, cRandGen);

	vFillRestrMatrixRandom(vv_supp_restr, cdMinMinRestr, cdMinMaxRestr, cdMaxMaxRestr, cRandGen); 
	vFillRestrMatrixRandom(vv_fact_restr, cdMinMinRestr, cdMinMaxRestr, cdMaxMaxRestr, cRandGen);
	vFillRestrMatrixRandom(vv_warh_restr, cdMinMinRestr, cdMinMaxRestr, cdMaxMaxRestr, cRandGen);
};


