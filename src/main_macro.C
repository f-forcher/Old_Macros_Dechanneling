/*
 * main_macro.C
 *
 *  Created on: Sep 21, 2016
 *      Author: fforcher
 */


//============================================================================
// Name        : bP_05.cpp
// Author      : Francesco Forcher
// Version     : 1.1
// Description : Modify Plot_v04 and use dispatcher&observers to handle begin/end job
//============================================================================

//#include "AnalysisFactory.h"
//#include "AnalysisInfo.h"
//#include "EventSource.h"
//#include "SourceFactory.h"
//#include "util/include/Dispatcher.h"

#include <iostream>
#include <vector>
#include <fstream>

void mia_dech(std::string nome_cristallo);

int main_macro(int argc, char* argv[]) {

	using namespace std;

	// store command line parameters

	int argc2 = 7;
	const char* argv2[] = { "./Debug/Ronch_braggPlot_v05", "input",
			"bragg_events.txt", "hist", "hh", "ranges", "energyRanges" };

	// TODO da leggere da un file eventualmente
	std::vector<const char*> elenco_cristalli = {"STF45", "STF38"};

	cout << "Test main macro" << endl;

	// Corso root lunardon/garfagnini
	  // carica la macro generica che legge il file di testo
	  //gROOT->LoadMacro("ReadHistoFromTextFile.C");
	  //gSystem->Load("ReadHistoFromTextFile_C.so"); // altra opzione per la precompilata

	/* TODO Chiamare mia_dech su tutti i cristalli
	 * 1. Ottenere i file recoDataSimple_546_31-59.torsion.correction.histo.root
	 *
	 * 2. Scoprire dove sono le variabili
	 * 	//xtal length and curvature TODO Controllare!
	    Double_t z = 2e-3;
	    Double_t R;
	 * per gli altri cristalli (tabella in tesi di roberto?)
	 *
	 * 3. ???
	 * 4. Profit!
	 */
	// TODO Okkio al c++11! ROOT 5 compatibile? Probabilmente si comunque
	for (const auto& ch : elenco_cristalli){
		cout << endl << endl;
		mia_dech(ch);
	}



	//mia_dech();

	//char t = 'a';
	//while (cin >> t)
	//	return 0;

	return 0;
}



