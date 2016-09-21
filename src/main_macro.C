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

int main_macro(int argc, const char* argv[]) {

	using namespace std;

	// store command line parameters

	int argc2 = 7;
	const char* argv2[] = { "./Debug/Ronch_braggPlot_v05", "input",
			"bragg_events.txt", "hist", "hh", "ranges", "energyRanges" };

	cout << "Test main macro" << endl;

	//mia_dech();

	return 0;

}



