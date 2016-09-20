//... include header files ...

#include "../BraggStatistic.h"

#include <cmath>

#include "../Constants.h"
#include "../TotalEnergy.h"
#include "../util/include/Singleton.h"


BraggStatistic::BraggStatistic(int minEnergy, int maxEnergy) :
		MIN_ENERGY_(minEnergy), MAX_ENERGY_(maxEnergy), number_of_events_(0), sumenergies(
				Event::minSize(), 0), sumsquares((int) Event::minSize(), 0), meanenergies(
				Event::minSize(), 0), rmsenergies(Event::minSize(), 0) {

}

BraggStatistic::~BraggStatistic() {
	// vector are automatically freed in their own destructor
}

// Corrected an incongruence: a Event* here and an Event& in
// the header BraggStatistic.h
// I suppose it was intended to be a Event&
// void BraggStatistic::add( const Event* ev ) {
void BraggStatistic::add(const Event& ev) {

	// total energy
	auto totalenergy = TotalEnergy::instance()->energy();

	// check for total energy being in the required range,
	// if not return
	if (!(totalenergy > MIN_ENERGY_ and totalenergy < MAX_ENERGY_)) {
		return;
	}

	// loop over points and update sums for each point
	for (unsigned i = 0; i < Event::minSize(); ++i) {
		sumenergies[i] += ev.energy(i) - Constants::bckgrn_mean;
		sumsquares[i] += pow(ev.energy(i) - Constants::bckgrn_mean, 2);
	}

	// update event counter
	++number_of_events_;

	return;

}

const std::vector<double>& BraggStatistic::eMean() const {
	return meanenergies;
}

const std::vector<double>& BraggStatistic::eRMS() const {
	return rmsenergies;
}

void BraggStatistic::compute() {
	using namespace std;

	// loop over points and compute mean and rms for each point
	// For every point, calculate the mean and rms
	for (unsigned i = 0; i < Event::minSize(); ++i) {
		meanenergies[i] = sumenergies[i] / double(number_of_events_);
		rmsenergies[i] = sqrt(
				sumsquares[i] / double(number_of_events_) - pow(
						sumenergies[i] / double(number_of_events_), 2));
	}
	return;
}

