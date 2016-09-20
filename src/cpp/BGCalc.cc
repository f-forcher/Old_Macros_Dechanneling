//... include header files ...

#include "../BGCalc.h"

#include <cmath>

#include "../AnalysisFactory.h"
#include "../TotalEnergy.h"
#include "../util/include/Singleton.h"

using namespace std;

// define a builder for this analyzer and register it
// builder to create an EnergyDist analyzer
class BGCalcBuilder: public AnalysisFactory::Builder {
public:
	// assign "bgcalc" as name for this analyzer and builder
	BGCalcBuilder() :
			AnalysisFactory::Builder("bgcalc") {
	}
	// create an EnergyDist when builder is run
	virtual AnalysisSteering* build(const AnalysisInfo* info) {
		return new BGCalc(info);
	}
};
// create a global BGCalcBuilder, so that it is created and registered
// before main execution start:
// when the AnalysisFactory::create function is run,
// an EnergyDistBuilder will be available with name "bgcalc".
static BGCalcBuilder bgcalcbuilder;

BGCalc::BGCalc(const AnalysisInfo* info) :
		AnalysisSteering(info) {
}

BGCalc::~BGCalc() {
}

void BGCalc::beginJob() {

	// min and max energy, min and max point, number of selected events
	MIN_ENERGY_ = 3000;
	MAX_ENERGY_ = 5000;
	min_points = 100;
	max_points = 120;
	number_of_events_ = 0;

	// sum of energies and sum of square energies
	sumenergies = 0;
	sumsquares = 0;
	return;

}

void BGCalc::update(const Event& ev) {

	// total energy
	auto totalenergy = TotalEnergy::instance()->rawSum();

	// check for total energy being in the required range,
	// if not return
	if (!(totalenergy > MIN_ENERGY_ and totalenergy < MAX_ENERGY_)) {
		return;
	}

	// loop over points and update sums
	for (unsigned i = min_points; i < max_points; ++i) {
		sumenergies += ev.energy(i);
		sumsquares += (ev.energy(i) * ev.energy(i));
	}

	// update event counter
	++number_of_events_;

	return;
}

void BGCalc::endJob() {

	// compute background

	// number of points
	auto numpoints = max_points - min_points;
	// total number of measurements
	auto tot_measurements = numpoints * number_of_events_;
	// compute mean and rms
	double meanenergies = sumenergies / double(tot_measurements);
	double rmsenergies = sqrt(
			sumsquares / double(tot_measurements) - pow(
					sumenergies / double(tot_measurements), 2));

	// printout result
	cout << "background " << meanenergies << " +- " << rmsenergies << endl;

	return;

}

