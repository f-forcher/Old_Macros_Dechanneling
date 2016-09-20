#include "../EventDump.h"

#include <iostream>

#include "../AnalysisFactory.h"
#include "../Event.h"

using namespace std;

// builder to create an EventDump analyzer
class EventDumpBuilder: public AnalysisFactory::Builder {
public:
	// assign "dump" as name for this analyzer and builder
	EventDumpBuilder() :
			AnalysisFactory::Builder("dump") {
	}
	// create an EventDump when builder is run
	virtual AnalysisSteering* build(const AnalysisInfo* info) {
		return new EventDump(info);
	}
};
// create a global EventDumpBuilder, so that it is created and registered
// before main execution start:
// when the AnalysisFactory::create function is run,
// an EventDumpBuilder will be available with name "dump".
static EventDumpBuilder ed;

EventDump::EventDump(const AnalysisInfo* info) :
		AnalysisSteering(info) {
}

EventDump::~EventDump() {
}

// function to be called at execution start
void EventDump::beginJob() {
	return;
}

// function to be called at execution end
void EventDump::endJob() {
	return;
}

// function to be called for each event
void EventDump::update(const Event& ev) {

	using namespace std;

	cout << "Event " << ev.eventNumber() << ": " << ev.dataSize() << " points"
			<< endl;
	for (unsigned i = 0; i < ev.dataSize(); ++i) {
		cout << "    " << ev.energy(i);

		// Digression: It should be noted that on modern processor complicated conditions
		// inside repeated blocks (like a for loop) are not cpu-intensive if
		// they are almost always true or false, thanks to branch prediction.
		// Anyway, since logic operators like && (=and) uses short-circuit evalutation,
		// it's better to put them in order of probability to have different values during
		// the cycle.
		if (i % 12 == 0 and i != 0 and i != ev.dataSize() - 1) {
			cout << endl;
		}
	}
	cout << endl << endl;
	return;

}

