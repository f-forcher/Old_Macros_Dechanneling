#ifndef BGCalc_h
#define BGCalc_h

//... include header files ...

#include <vector>

#include "AnalysisSteering.h"
#include "Event.h"
#include "util/include/ActiveObserver.h"

class BGCalc: public AnalysisSteering, ActiveObserver<Event> {

public:

	BGCalc(const AnalysisInfo* info);
	virtual ~BGCalc();

	// function to be called at execution start
	virtual void beginJob();
	// function to be called at execution end
	virtual void endJob();
	// function to be called for each event
	virtual void update(const Event& ev);

private:

	// min and max energy, min and max point, number of selected events
	int MIN_ENERGY_;
	int MAX_ENERGY_;
	int min_points;
	int max_points;
	int number_of_events_;

	// sum of energies and sum of square energies
	long sumenergies;
	unsigned long sumsquares;

	BGCalc(const BGCalc& x);
	BGCalc& operator=(const BGCalc& x);

};

#endif

