//============================================================================
// Name        : bP_05.cpp
// Author      : Francesco Forcher
// Version     : 1.1
// Description : Modify Plot_v04 and use dispatcher&observers to handle begin/end job
//============================================================================

#include "AnalysisFactory.h"
#include "AnalysisInfo.h"
#include "EventSource.h"
#include "SourceFactory.h"
#include "util/include/Dispatcher.h"

using namespace std;

int main( int argc, char* argv[] ) {

  // store command line parameters

	int argc2 = 7;
	char* argv2[] =
	{
			"./Debug/Ronch_braggPlot_v05", "input", "bragg_events.txt", "hist", "hh", "ranges", "energyRanges"
	};
  AnalysisInfo* info = new AnalysisInfo( argc2, argv2 );

  // create data source
  EventSource* es = SourceFactory::create( info );

  // create a list of analyzers
  AnalysisFactory::create( info );

  // initialize all analyzers
  Dispatcher<AnalysisInfo::AnalysisStatus>::notify( AnalysisInfo::begin );

  // loop over events
  es->run();

  // finalize all analyzers
  Dispatcher<AnalysisInfo::AnalysisStatus>::notify( AnalysisInfo::end );

  return 0;

}

