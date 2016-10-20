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
#include <fstream>
#include <memory>
#include <vector>
#include <array>

namespace mions {

using EventoDechanneling = std::array<Double_t,4>;

class DatiChanneling {
public:
	//DatiChanneling();

	/* @brief: Read the file file_dati and fill the class
	 *
	 */
	//DatiChanneling(std::shared_ptr<std::ifstream> file_dati);
	DatiChanneling(std::string nome_file_dati);
	virtual ~DatiChanneling();

	/*
	 * @brief: print the events with index from "from" to "to"
	 */
	void print(ULong64_t from, ULong64_t to) const;

	/*
	 * @brief: print the events with index from 0 to "to"
	 */
	void print(ULong64_t to) const {
		print( 0, to );
	}
	;

	/*
	 * @brief: get the four data of a single event ("orizontal" slice)
	 * @return: EventoDechanneling is an std::array<Double_t,4>
	 */
	EventoDechanneling getEvent(ULong64_t index) const;
	/*
	 * @brief: get number of events
	 */
	auto getSize() const {
		return x_entrata.size();
	}

	//TODO getters setters
private:
	std::vector<Double_t> x_entrata;
	std::vector<Double_t> y_entrata;
	std::vector<Double_t> x_uscita;
	std::vector<Double_t> y_uscita;

};

} /* namespace mions */

#endif /* DATICHANNELING_H_ */
