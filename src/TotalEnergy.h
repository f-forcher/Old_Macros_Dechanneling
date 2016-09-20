/*
 * TotalEnergy.h
 *
 *  Created on: 16 gen 2016
 *      Author: francesco
 */

#ifndef TOTALENERGY_H_
#define TOTALENERGY_H_

#include "Event.h"
#include "util/include/LazyObserver.h"
#include "util/include/Singleton.h"

class TotalEnergy: public Singleton<TotalEnergy>, public LazyObserver<Event> {

	//... friend declaration ...
	friend Singleton<TotalEnergy> ;

public:

	// function to be called when a new event arrives
	virtual void update(const Event& ev);

	// sum of measurements, no background subtraction
	double rawSum();
	// Total energy minus background
	double energy();

private:

	// sum of data, no background subtraction
	double totalenergy;

	TotalEnergy();
	~TotalEnergy();
	TotalEnergy(const TotalEnergy& x);
	TotalEnergy& operator=(const TotalEnergy& x);

};

#endif /* TOTALENERGY_H_ */
