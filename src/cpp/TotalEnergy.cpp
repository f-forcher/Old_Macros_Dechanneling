/*
 * TotalEnergy.cpp
 *
 *  Created on: 16 gen 2016
 *      Author: francesco
 */

#include "../TotalEnergy.h"

#include "../Constants.h"
#include "../util/include/LazyObserver.h"

TotalEnergy::TotalEnergy() {
	// TODO Auto-generated constructor stub

}

TotalEnergy::~TotalEnergy() {
	// TODO Auto-generated destructor stub
}

// function to be called when a new event arrives
void TotalEnergy::update(const Event& ev) {
	// set sum at 0
	totalenergy = 0;

	// loop over points
	for (unsigned i = 0; i < Event::minSize(); ++i) {
		totalenergy += ev.energy(i);
	}

	return;
}

// sum of measurements, no background subtraction
double TotalEnergy::rawSum() {
	// check for new event and return result
	check();
	return totalenergy;
}

double TotalEnergy::energy() {
	// check for new event and return result
	check();
	return totalenergy - Constants::bckgrn_mean*Event::minSize() ;
}
