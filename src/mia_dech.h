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
/*
 * Macro to calculate dechanneling lenght for a crystal.
 * @nome_cristallo: Name of the folder with the crystal data
 * @output_dech: file in which to append the "Ldech"s, in the format
 *               Crystal | dechanneling L at +-5 microrad [m] | dechanneling L at +-10 microrad [m]
 * @dati_cristalli_orig: declared data of the crystal
 * @dati_cristalli_calcolati: data calculated using three separated functions
 * dati_cristalli_calcolati_ftot: data calculated using a total fit
 */
void mia_dech(std::string nome_cristallo, std::shared_ptr<std::ofstream> output_analisi_dech,
		std::shared_ptr<TFile> root_output, const mions::CrystalDataTable& dati_cristalli_orig,
		mions::CrystalDataTable510& dati_cristalli_calcolati,
		mions::CrystalDataTable510& dati_cristalli_calcolati_ftot);
}

#else
// Altrimenti includi il file della macro. Nel compilatore naturalmente direbbe multiple definition quando linka l'altro
// file.

#include "mia_dech.C"

#endif

#endif /* MIA_DECH_H_ */
