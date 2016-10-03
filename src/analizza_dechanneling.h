/*
 * analizza_dechanneling.h
 *
 *  Created on: Oct 3, 2016
 *      Author: fforcher
 */

#ifndef ANALIZZA_DECHANNELING_H_
#define ANALIZZA_DECHANNELING_H_


#ifndef __CLING__

#include "my_typedefs.h"

class TFile;

	namespace mions {
	void analizza_dechanneling(std::shared_ptr<std::ofstream> output_dech,
			std::shared_ptr<TFile> root_output,
			const mions::CrystalDataTable510& dati_cristalli);
	}

#else
	// Altrimenti includi il file della macro. Nel compilatore naturalmente direbbe multiple definition quando linka l'altro
	// file.

	#include "analizza_dechanneling.C"


#endif




#endif /* ANALIZZA_DECHANNELING_H_ */
