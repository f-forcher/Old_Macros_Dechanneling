/*
 * DatiChanneling.h
 *
 *  Created on: Sep 27, 2016
 *      Author: fforcher
 */

#ifndef DATICHANNELING_H_
#define DATICHANNELING_H_

#include <TROOT.h>
#include <iostream>
#include <memory>
#include <vector>

namespace mions {

class DatiChanneling {
public:
	DatiChanneling();

	/* @brief: Read the file file_dati and fill the class
	 *
	 */
	DatiChanneling(std::shared_ptr<std::ifstream> file_dati);
	//Todo builders
	virtual ~DatiChanneling();

	void print() const;

	//TODO getters setters
private:
	std::vector<Double_t> x_entrata;
	std::vector<Double_t> y_entrata;
	std::vector<Double_t> x_uscita;
	std::vector<Double_t> y_uscita;

};

} /* namespace mions */

#endif /* DATICHANNELING_H_ */
