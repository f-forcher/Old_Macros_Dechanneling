/*
 * DatiChanneling.cpp
 *
 *  Created on: Sep 27, 2016
 *      Author: fforcher
 */

#include "DatiChanneling.h"

namespace mions {

/*
 DatiChanneling::DatiChanneling() {
 // TODO Auto-generated constructor stub

 }
 */

DatiChanneling::DatiChanneling(std::string nome_file_dati) {

	//  131072 = 2^17, tipical number of events in the range 100 000 - 500 000

	std::ifstream file_dati( nome_file_dati );

	if (file_dati) {

		x_entrata.reserve( 131072 );
		y_entrata.reserve( 131072 );
		x_uscita.reserve( 131072 );
		y_uscita.reserve( 131072 );

		Double_t tx_entrata;
		Double_t ty_entrata;
		Double_t tx_uscita;
		Double_t ty_uscita;

		while (file_dati >> tx_entrata) {
			file_dati >> ty_entrata;
			file_dati >> tx_uscita;
			file_dati >> ty_uscita;

			x_entrata.emplace_back( tx_entrata );
			y_entrata.emplace_back( ty_entrata );
			x_uscita.emplace_back( tx_uscita );
			y_uscita.emplace_back( ty_uscita );
		};
	} else {
		std::cerr << "[ERROR]: Tried to open " << nome_file_dati;
		throw "[ERROR]: File .dat not opened";
	}

}

DatiChanneling::~DatiChanneling() {
	// TODO Auto-generated destructor stub
}

void DatiChanneling::print(ULong64_t from, ULong64_t to) const {
	using namespace std;
	EventoDechanneling ev;

	for (ULong64_t i = from; i < to; ++i) {
		ev = getEvent( i );

		// Stampa i valori dell'evento
		for (const auto& dato : ev) {
			cout << dato << " ";
		}
		cout << endl;
	}
}

EventoDechanneling DatiChanneling::getEvent(ULong64_t index) const {
	EventoDechanneling ev;

	ev[0] = x_entrata[index];
	ev[1] = y_entrata[index];
	ev[2] = x_uscita[index];
	ev[3] = y_uscita[index];

	return ev;
}

} /* namespace mions */
