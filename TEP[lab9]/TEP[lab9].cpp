// TEP[lab9].cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "CMscnProblem.h"
#include "CSolution.h"
#include "CRandom.h"
#include "CRandomSearch.h"
#include "CDiffEvol.h"
#include "CTrivMethod.h"

using namespace std;


int main()
{
	/*CMscnProblem problem;
	problem.bOpenProblemFile("problem1.txt");
	problem.vPrint();
	bool bSolErr = false;

	cout << "\n~~~~~~~~~~~~~~~~~~~~~\n";

	CSolution sol;
	sol.vReadSolutionFromFile("sol1.txt", bSolErr);
	problem.bSetSolution(sol);

	bool bSatisfError = false;

	double dQual = problem.dGetQuality(bSatisfError);

	if (bSatisfError == true) {
		cout << "\nNOT SATISFIED, QUALITY: "<< dQual;
	}
	else {
		cout << "\nSATISFIED, QUALITY: " << dQual;
	}

	problem.bWriteProblemFile("newprob.txt");*/

	/*CRandom rand(2);
	for (int n = 0; n < 10; n++) {
		cout << rand.dGenerateDouble (1, 5) << ' ';
	}*/

	CMscnProblem problem;
	problem.bSetSuppliers(2);
	problem.bSetFactories(2);
	problem.bSetWarehouses(2);
	problem.bSetRetailers(2);

	problem.vGenerateInstance(1);
	CRandomSearch search(&problem);
	search.cFindSolution(3);
	cout << "\nDiffEvol\n";
	CDiffEvol cDifEv(&problem, 0.3, 0.4, 10);
	cDifEv.cFindSolution(3);

	//cout << "\nOSTATECZNIE: ";
	cout << "\nNEW QUALITY: " << problem.dGetQuality();
	cout << "\nCONSTRAINTS SATISFIED: " << problem.bConstraintsSatisfied();

	cout << "\nMODYFIKACJA\n";

	CTrivMethod cTrMet(&problem);
	cTrMet.cFindSolution(0).vShowSolTable();
	cout << "\nQUALITY: " << problem.dGetQuality();
	cout << "\nSATISFIED: " << problem.bConstraintsSatisfied();


	
	//problem.vPrint();

	//problem.bWriteProblemFile("generated.txt");
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
