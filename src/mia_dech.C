/*
 * mia_dech.C
 *
 *  Created on: Sep 26, 2016
 *      Author: fforcher
 */



//#include <cstdio> //system()
#include <iostream>
#include <unistd.h> // chdir


#include "dbg_macro.h"
#include "func_sim.h"
#include "dech.h"
#include "DatiChanneling.h"

extern TDirectory* ROOT_PROJDIR;
//extern 	std::vector<TH1*> vHistograms;
//extern 	std::vector<TH1*> vHistograms10;
extern std::vector<TCanvas* > vCanvases;
extern char PROJECT_DIR[FILENAME_MAX];

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




void mia_dech(std::string nome_cristallo, std::shared_ptr<std::ofstream> output_dech, std::shared_ptr<TFile> root_output) {

	//TODO Trovare un modo di tornare su di cartella
	//PROJDIR->cd();
	chdir(PROJECT_DIR);

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
	std::string nomefiledati = "recoDataSimple_" + nome_cristallo + ".torsion.correction.histo.dat";

	std::ifstream file_dat(pathfiledati);

	//600 bin ,da -200 a 400
	// Scelti guardando i grafici fatti da dech.C

	// select +- 5 microrad in nomehisto5, +-10 in nomehisto10
	std::string nomehisto5 = "hdati5_" + nome_cristallo;
	std::string nomehisto10 = "hdati10_" + nome_cristallo;
	std::string titlehisto5 = nome_cristallo + ", cuts at +- 5 microrad";
	std::string titlehisto10 = nome_cristallo + ", cuts at +- 10 microrad";
	clog << nomehisto5 << endl;


	// vHistograms.push_back( new TH1D(
	// /* name */nomehisto.c_str(),
	// /* title */nome_cristallo.c_str(),
	// /* X-dimension */600, -200, 400));

	auto histogram5 = std::make_unique<TH1D>(
			 /* name */nomehisto5.c_str(),
			 /* title */titlehisto5.c_str(),
			 /* X-dimension */600, -200, 400);

	auto histogram10 = std::make_unique<TH1D>(
			 /* name */nomehisto10.c_str(),
			 /* title */titlehisto10.c_str(),
			 /* X-dimension */600, -200, 400);

	//TODO ??? gli hist han tutti lo stesso nome
	//vHistograms.front()->SetNameTitle(nomehisto.c_str(),nome_cristallo.c_str());


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
				//vHistograms.front()->Fill(x_uscita-x_entrata);
				histogram5->Fill(x_uscita-x_entrata);
			}

			if (x_entrata > -10 and x_entrata < 10) {
				//vHistograms.front()->Fill(x_uscita-x_entrata);
				histogram10->Fill(x_uscita-x_entrata);
			}
		}

		//std::vector<TCanvas* > vCanvases;

		//TCanvas* mio_c1 = new TCanvas();
		//Canvases.push_back(new TCanvas(nome_cristallo.c_str()));
		//vHistograms.front()->Draw();
		//histogram5->Draw();

		root_output->Write();

	} else {
		DBG(
			clog << nome_cristallo << ": File .dat not found" << endl;
			clog << "Nome cercato: " << pathfiledati << endl;
			clog << "Current Dir: " << system("pwd") << endl;
			, ; )
		clog << "[WARNING] For crystal " << nome_cristallo << ", File .dat not found" << endl <<
				"          Trying to use old dech.C macro and .root data file" << endl << endl;


		//ROOT_PROJDIR->cd();
		//Todo rimetterla;
		//dech(nome_cristallo,output_dech);
	}

}


}
