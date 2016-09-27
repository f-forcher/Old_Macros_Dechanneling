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
#include "DatiChanneling.h"

extern TDirectory* PROJDIR;
extern 	std::vector<TH1*> vHistograms;
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

	//TODO Trovare un modo di tornare su di cartella con ROOT
	PROJDIR->cd();

	std::string cartella_cristallo = "ForFrancesco/" + nome_cristallo
			+ "_exp/";
	gSystem->ChangeDirectory(cartella_cristallo.c_str());
	std::string pathfiledati = //cartella_cristallo +
			  "recoDataSimple_" + nome_cristallo + ".torsion.correction.histo.dat";

	// TODO Generalizzare il nome del file
	std::string nomefiledati = "recoDataSimple_" + nome_cristallo + ".torsion.correction.histo.dat";

	std::ifstream file_dat(pathfiledati);

	//600 bin ,da -200 a 400
	// Scelti guardando i grafici fatti da dech.C

	vHistograms.push_back( new TH1D(
	/* name */"hdati",
	/* title */"Istogramma Dati",
	/* X-dimension */600, -200, 400));


	if (file_dat) {
		gStyle->SetPalette(1);
		gStyle->SetOptStat(1);
		gStyle->SetOptTitle(1);
		TGaxis::SetMaxDigits(3);


		DBG(
			clog << "[LOG]: " << "Crystal " << nome_cristallo << endl;
			clog << "[LOG]: File "<< pathfiledati << endl << endl;
			, ; )
		// TODO il mio codice qua

		DatiChanneling dati(nomefiledati);

		EventoDechanneling ev;
		auto datisize = dati.getSize();

		//dati.print(datisize);

		//Read events and select those with entry angle between -+5 microrad
		for (ULong64_t i = 0; i < datisize; ++i) {

			ev = dati.getEvent(i);
			Double_t x_entrata = ev[0];
			Double_t x_uscita = ev[2];


			if (x_entrata > -5 and x_entrata < 5) {
				vHistograms.back()->Fill(x_uscita-x_entrata);
			}
		}

		std::vector<TCanvas* > vCanvases;

		//TCanvas* mio_c1 = new TCanvas();
		vCanvases.push_back(new TCanvas());
		vHistograms.back()->Draw();

	} else {
		DBG(
			clog << nome_cristallo << ": File .dat not found" << endl;
			clog << "Nome cercato: " << pathfiledati << endl;
			clog << "Current Dir: " << system("pwd") << endl;
			, ; )
		clog << "[WARNING] For crystal " << nome_cristallo << ", File .dat not found" << endl <<
				"          Trying to use old dech.C macro and .root data file" << endl << endl;
		PROJDIR->cd();
		//Todo rimetterla;
		//dech(nome_cristallo,output_dech);
	}

}


}
