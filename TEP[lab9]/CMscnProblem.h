#pragma once
#include <vector>;
#include "CSolution.h";
#include "CRandom.h";
#include "CProblem.h";

using namespace std;

class CMscnProblem : public CProblem
{
public:
	CMscnProblem();
	~CMscnProblem();

	bool bSetSuppliers(int iSuppAmount); //	SETTERS FOR SUPPLIERS, FACTORIES, WAREHOUSES, RETAILERS AMOUNT													
	bool bSetFactories(int iFactAmount);
	bool bSetWarehouses(int iWarhAmount);
	bool bSetRetailers(int iRetAmount);

	bool bSetSuppMaxProd(int iIndex, double dValue); // SETTERS FOR MAX VALUES OF PRODUCTS FROM SUPPLIERS, FROM FACTORIES, WAREHOUSE
	bool bSetFactMaxProd(int iIndex, double dValue); // CAPACITY AND RETAILERS DEMAND
	bool bSetWarhCap(int iIndex, double dValue);
	bool bSetRetDem(int iIndex, double dValue);

	bool bSetSuppToFactCost(int iIndexX, int iIndexY, double dValue); //SETTERS FOR COSTS OF TRANSPORT FROM PLACE TO PLACE 
	bool bSetFactToWarhCost(int iIndexX, int iIndexY, double dValue);
	bool bSetWarhToRetCost(int iIndexX, int iIndexY, double dValue);

	bool bSetSuppOnePay(int iIndex, double dValue); //SETTERS FOR ONE PAYMENT FOR SERVICE
	bool bSetFactOnePay(int iIndex, double dValue);
	bool bSetWarhOnePay(int iIndex, double dValue);

	bool bSetRetInc(int iIndex, double dValue);

	bool bSetSuppRestr(int iIndex, int iIndexY, double dValue); //SETTERS FOR AMOUNT OF PRODUCT BOUNDS FROM PLACE TO PLACE
	bool bSetFactRestr(int iIndex, int iIndexY, double dValue);
	bool bSetWarhRestr(int iIndex, int iIndexY, double dValue);

	bool bSetSolution(CSolution sol);
	void vDeleteSolution();
	CSolution cGetSolution() {
		return solution;
	}

	double dGetQuality(); //GET TOTAL INCOME
	bool bConstraintsSatisfied();//CHECK IF CONSTRAINTS SATISFIED

	double dMinValueOfProduct(int iIndex, bool&bError); //GET MIN AMOUNT OF PRODUCT FOR SOLUTION TABLE ELEMENT
	double dMaxValueOfProduct(int iIndex, bool&bError); //GET MAX
	bool bCheckSolutionConstraints(); //CHECK SOLUTION CONSTRAINTS FROM MINMAX MATRIX

	bool bWriteProblemFile(string sFileName); //SAVE PROBLEM INFO TO FILE
	bool bOpenProblemFile(string sFileName);  //GET PROBLEM INFO FROM FILE

	void vPrint(); //PRINT INFO

	//LIST 10

	void vGenerateInstance(int iInstanceSeed);

	int iGetSupplierAmount(){ return i_supplier_amount; }
	int iGetFactoryAmount() { return i_factory_amount; }
	int iGetWarehouseAmount() { return i_warehouse_amount; }
	int iGetRetailerAmount() { return i_retailer_amount; }

	vector<double>* vGetSuppMaxProduct() { return &v_supp_max_product; }
	vector<double>* vGetFactMaxProduct() { return &v_fact_max_product; }
	vector<double>* vGetWarhCapacity() { return &v_warh_capacity; }
	vector<double>* vGetRetDemand() { return &v_ret_demand; }
	
private:
	bool bCheckSolution(CSolution CSol); //GETQUALITY AND GETCONSTRAINTS, CHECK IF SOLUTION FILE IS OK
	
	double dOnePaymentCosts(int iSourceAmount, int iDestAmount, vector<double> vSourcePayCosts, int iOffset); //TO COUNT TOTAL INCOME, QUALITY
	double dProdTransportCosts(int iSourceAmount, int iDestAmount, vector<vector<double>>vvSourceToDestCosts, int iOffset);
	double dShopsIncome(int iOffset);

	bool dCheckAmountContraints(int iSourceAmount, int iDestAmount, vector<double> vProdConstraints, int iProdOffset); //TO CHECK ALL CONSTRAINTS
	bool dCheckSumConstraints(int iSourceAmount, int iHalfStopAmount, int iDestAmount, int iFstProdOffset, int iSndProdOffset);
	bool dCheckAmountRetContraints(int iWrhAmount, int iRetAmount, vector<double> vDemConstraints, int iProdOffset);

	double* dpMinMaxValueOfProduct(int iIndex); //TO GET MIN AND MAX VALUE OF ELEMENT IN SOLUTION TABLE

	string sVectorToString(vector<double> vVect); //CONVERT VECTOR TO STRING TO SAVE IT TO FILE
	string sMatrixToString(vector<vector<double>> vvMatrix); //SAME WITH MATRIX

	void vReadVectorFromFile(fstream &cFile, int iSize, vector<double> &vVect); //READ VECTOR FROM FILE TO GET IT IN THE PROGRAMME
	void vReadMatrixFromFile(fstream &cFile, int iSizeX, int iSizeY, vector<vector<double>> &vvMatrix); //SAME WITH MATRIX

	bool setValueInVector(vector<double> &vVector, int iIndex, double dValue); //TO SET VALUE IN VECTOR AND MATRIX
	bool setValueInMatrix(vector<vector<double>> &vvMatrix, int iIndexX, int iIndexY, double dValue);
	void vResizeMatrixCol(vector<vector<double>> &vvMatrix, int iNewSize);

	static void vPrintTable(vector<double> vTable); //PRIVATE FOR PRINT
	static void vPrintMatrix(vector<vector<double>> vvMatrix);

	//LIST 10
	void vFillMatrixRandom(vector<vector<double>>& vvMatrix, double dMinVal, double dMaxVal, CRandom& cRand);
	void vFillVectorRandom(vector<double>& vVector, double dMinVal, double dMaxVal, CRandom& cRand);
	void vFillRestrMatrixRandom(vector<vector<double>>& vvMatrix, double dMinMinVal, double dMinMaxVal, double dMaxMaxVal, CRandom& cRand);

	void vFixAmountConstraints(int iSourceAmount, int iDestAmount, vector<double> vProdConstraints, int iProdOffset);
	void vFixAmountRetConstraints(int iWrhAmount, int iRetAmount, vector<double> vDemConstraints, int iProdOffset);
	void vFixSumConstraints(int iSourceAmount, int iHalfStopAmount, int iDestAmount, int iFstProdOffset, int iSndProdOffset);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    CSolution solution;

	int i_supplier_amount; 
	int i_factory_amount;
	int i_warehouse_amount;
	int i_retailer_amount;

	vector<double> v_supp_max_product;
	vector<double> v_fact_max_product;
	vector<double> v_warh_capacity;
	vector<double> v_ret_demand;


	const double cdMinRestr = 200000; 
	const double cdMaxRestr = 500000;

	vector<vector<double>> vv_supp_to_fact_cost; 
	vector<vector<double>> vv_fact_to_warh_cost;
	vector<vector<double>> vv_warh_to_ret_cost;

	const double cdMinCost = 1;
	const double cdMaxCost = 100;


	vector<double> v_supp_one_payment; 
	vector<double> v_fact_one_payment;
	vector<double> v_warh_one_payment;

	const double cdMinOnePayCost = 1;
	const double cdMaxOnePayCost = 100;

	vector<double> v_ret_income; 

	const double cdMinRetIncome = 5000;
	const double cdMaxRetIncome = 6000;

	vector <vector<double>> vv_supp_restr; 
	vector <vector<double>> vv_fact_restr;
	vector <vector<double>> vv_warh_restr;

	const double cdMinMinRestr = 0;
	const double cdMinMaxRestr = 0;
	const double cdMaxMaxRestr = 200000;
};

