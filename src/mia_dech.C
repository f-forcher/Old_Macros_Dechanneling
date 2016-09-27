/*
 * mia_dech.C
 *
 *  Created on: Sep 26, 2016
 *      Author: fforcher
 */



#include <cstdio> //system()
#include <iostream>

#include "dbg_macro.h"
#include "func_sim.h"
#include "dech.h"

extern TDirectory* PROJDIR;

namespace mions {

Double_t myfunction(Double_t *x, Double_t *par) {
	// TODO Float?
	// Float_t xx = x[0];

	Float_t xx = x[0];
	Double_t f = par[0] * exp(-xx / par[1]);
	return f;


// TODO Provata con STF45, "generalizzare" agli altri
/*
 * Macro to calculate dechanneling lenght for a crystal.
 * @nome_cristallo: Nabe of the folder with the crystal data
 * @output_dech: file in which to append the "Ldech"s, in the format
 *               Crystal | dechanneling L at +-5 microrad [m] | dechanneling L at +-10 microrad [m]
 */
}

/*
class dati_channeling {
		Double_t x_entrata;
		Double_t y_entrata;
		Double_t x_uscita;
		Double_t y_uscita;
};
*/



void mia_dech(std::string nome_cristallo, std::shared_ptr<std::ofstream> output_dech) {

	gStyle->SetPalette(1);
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);
	TGaxis::SetMaxDigits(3);

	std::string cartella_cristallo = "ForFrancesco/" + nome_cristallo
			+ "_exp/";
	gSystem->ChangeDirectory(cartella_cristallo.c_str());
	std::string pathfiledati = //cartella_cristallo +
			  "recoDataSimple_" + nome_cristallo + ".torsion.correction.histo.dat";

	// TODO Generalizzare il nome del file
	std::string filedati = "recoDataSimple_" + nome_cristallo + ".torsion.correction.histo.dat";

	std::ifstream file_dat(pathfiledati);

	dati_channeling dc;

	if (file_dat) {
		DBG(clog << pathfiledati << endl;, ;)
		// TODO il mio codice qua


		while (file_dat >> dc.x_entrata) {
			file_dat >> dc.y_entrata;
			file_dat >> dc.x_uscita;
			file_dat >> dc.y_uscita;
		};



	} else {
		DBG(
			clog << nome_cristallo << ": File .dat not found" << endl;
			clog << "Nome cercato: " << pathfiledati << endl;
			clog << "Current Dir: " << system("pwd") << endl;,
			; )
		clog << "[WARNING] For crystal " << nome_cristallo << ", File .dat not found" << endl <<
				"          Trying to use old dech.C macro and .root data file" << endl << endl;
		PROJDIR->cd();
		dech(nome_cristallo,output_dech);
	}

}


}
