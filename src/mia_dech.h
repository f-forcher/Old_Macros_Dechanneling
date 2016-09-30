/*
 * mia_dech.h
 *
 *  Created on: Sep 27, 2016
 *      Author: fforcher
 */

#ifndef MIA_DECH_H_
#define MIA_DECH_H_

// Per poter usare questa macro sia compilando che eseguendo.
// https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html#moving-between-interpreter-and-compiler
#ifndef __CLING__

#include "my_typedefs.h"

class TFile;

	namespace mions {
	void mia_dech(const std::string nome_cristallo,
			std::shared_ptr<std::ofstream> output_dech,
			std::shared_ptr<TFile> root_output,
			const mions::CrystalDataTable& dati_cristalli);
	}

#else
	// Altrimenti includi il file della macro. Nel compilatore naturalmente direbbe multiple definition quando linka l'altro
	// file.

	#include "mia_dech.C"


#endif



#endif /* MIA_DECH_H_ */
