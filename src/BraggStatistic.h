#ifndef BraggStatistic_h
#define BraggStatistic_h

#include <vector>

#include "Event.h"

class BraggStatistic {

public:

	// create a set of counters for events with
	// total energy between minEnergy and maxEnergy
	BraggStatistic(int minEnergy, int maxEnergy);
	~BraggStatistic();

	// add an event, after checking for total energy
	void add(const Event& ev);
	// compute mean and rms
	void compute();

	// return results:
	// number of events with total energy in the range
	// array of mean energies for each point
	// array of rms  energies for each point
	int nEvents() const {
		return number_of_events_;
	}
	const std::vector<double>& eMean() const;
	const std::vector<double>& eRMS() const;

	// C++11 standard syntax to prevent the compiler to automatically generate them;
	BraggStatistic(const BraggStatistic& x) = delete;
	BraggStatistic& operator=(const BraggStatistic& x) = delete;

private:

	// dummy copy constructor and assignment to prevent inadvertent copy
	// In C++11 better done through explicitly deleted functions, see above
	// BraggStatistic           ( const BraggStatistic& x );
	// BraggStatistic& operator=( const BraggStatistic& x );

	// min and max energy, number of selected events
	const int MIN_ENERGY_;
	const int MAX_ENERGY_;
	unsigned number_of_events_;

	// arrays with sum of energies and sum of square energies
	std::vector<int> sumenergies;
	std::vector<unsigned> sumsquares;

	// arrays with mean and rms energies
	std::vector<double> meanenergies;
	std::vector<double> rmsenergies;

};

#endif

