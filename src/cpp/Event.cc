#include "../Event.h"

// set the min (120) and max (128) number of points
const unsigned Event::MIN_POINTS = 120;
const unsigned Event::MAX_POINTS = 128;

Event::Event::Event(int event_ID) :
		event_ID_(event_ID) {
	energies_.reserve(MAX_POINTS);
}

Event::~Event() {

}

void Event::add(int energyLoss) {
	// check for max array size:
	// if max number of points not yet reached
	//   increase number of points and store new point energy
	// if max number of points already reached
	//   do nothing
	if (energies_.size() < MAX_POINTS) {
		energies_.push_back(energyLoss);
	} else {
		return;
	}

	return;
}

int Event::eventNumber() const {
	return event_ID_;
}

unsigned int Event::dataSize() const {
	return energies_.size();
}

int Event::energy(unsigned int i) const {
	// check for required point being inside the array:
	// - if required point inside the array return its energy
	// - if not return -1

	// i unsigned, no need to check i>=0
	if (i < energies_.size()) {
		return energies_[i];
	} else {
		return -1;
	}

}

unsigned int Event::maxSize() {
	return MAX_POINTS;
}

unsigned int Event::minSize() {
	return MIN_POINTS;
}

