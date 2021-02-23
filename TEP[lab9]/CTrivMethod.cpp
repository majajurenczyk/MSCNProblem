#include "pch.h"
#include "CTrivMethod.h"


CTrivMethod::CTrivMethod(CProblem* cProb){
	c_problem = cProb;
}


CTrivMethod::~CTrivMethod(){
}

CSolution CTrivMethod::cFindSolution(double dTime) {
	CSolution cSol(((CMscnProblem*)c_problem)->iGetSupplierAmount(), ((CMscnProblem*)c_problem)->iGetFactoryAmount(),
		((CMscnProblem*)c_problem)->iGetWarehouseAmount(), ((CMscnProblem*)c_problem)->iGetRetailerAmount());
	for (int ii = 0; ii < cSol.iGetSolSize(); ii++) {
		cSol.getSolution()[ii] = ii;
	}
	((CMscnProblem*)c_problem)->bSetSolution(cSol);
	return cSol;
}
