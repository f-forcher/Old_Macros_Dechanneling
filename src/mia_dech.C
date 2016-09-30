/*
 * mia_dech.C
 *
 *  Created on: Sep 26, 2016
 *      Author: fforcher
 */

//#include <cstdio> //system()
#include <iostream>
#include <unistd.h> // chdir
#include <sstream>
//#include <map>

#include "dbg_macro.h"
#include "func_sim.h"
#include "dech.h"
#include "DatiChanneling.h"
#include "my_typedefs.h"

//extern TDirectory* ROOT_PROJDIR;
//extern 	std::vector<TH1*> vHistograms;
//extern 	std::vector<TH1*> vHistograms10;
//extern std::vector<TCanvas*> vCanvases;
extern char PROJECT_DIR[FILENAME_MAX];

namespace mions {

Double_t myfunction(Double_t *x, Double_t *par) {
	// TODO Float?
	// Float_t xx = x[0];

	Double_t xx = x[0];
	Double_t f = par[0] * exp(-xx / par[1]);
	return f;

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

void mia_dech(std::string nome_cristallo,
		std::shared_ptr<std::ofstream> output_dech,
		std::shared_ptr<TFile> root_output,
		const mions::CrystalDataTable& dati_cristalli) {

	//PROJDIR->cd();
	// Go to the project's "home" folder
	chdir(PROJECT_DIR);

	using std::string;
	using std::ifstream;
	using std::stringstream;

	//gStyle->SetPalette(1);
	//gStyle->SetOptStat(0);
	//gStyle->SetOptTitle(0);
	//Gaxis::SetMaxDigits(3);

	string cartella_cristallo = "ForFrancesco/" + nome_cristallo + "_exp/";
	gSystem->ChangeDirectory(cartella_cristallo.c_str());
	string pathfiledati = //cartella_cristallo +
			"recoDataSimple_" + nome_cristallo
					+ ".torsion.correction.histo.dat";

	string nomefiledati = "recoDataSimple_" + nome_cristallo
			+ ".torsion.correction.histo.dat";

	ifstream file_dat(pathfiledati);

	//600 bin ,da -200 a 400
	// Scelti guardando i grafici fatti da dech.C

	// select +- 5 microrad in nomehisto5, +-10 in nomehisto10
	string nomehisto5 = "hdati5_" + nome_cristallo;
	string nomehisto10 = "hdati10_" + nome_cristallo;
	string titlehisto5 = nome_cristallo + ", cuts at +- 5 microrad";
	string titlehisto10 = nome_cristallo + ", cuts at +- 10 microrad";
	clog << nomehisto5 << endl;

	// vHistograms.push_back( new TH1D(
	// /* name */nomehisto.c_str(),
	// /* title */nome_cristallo.c_str(),
	// /* X-dimension */600, -200, 400));

	auto histogram5 = std::make_unique < TH1D > (
	/* name */nomehisto5.c_str(),
	/* title */titlehisto5.c_str(),
	/* X-dimension */600, -200, 400);

	auto histogram10 = std::make_unique < TH1D > (
	/* name */nomehisto10.c_str(),
	/* title */titlehisto10.c_str(),
	/* X-dimension */600, -200, 400);

	//vHistograms.front()->SetNameTitle(nomehisto5.c_str(),nome_cristallo.c_str());

	if (file_dat) {
		// Il codice per la mia analisi qua

		//gStyle->SetPalette(1);
		//gStyle->SetOptStat(1);
		//gStyle->SetOptTitle(1);
		//TGaxis::SetMaxDigits(3);

		DBG(
				clog << "[LOG]: " << "Crystal " << nome_cristallo << endl; clog << "[LOG]: File "<< pathfiledati << endl << endl;,
				;)

		//Riempi gli istogrammi
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
				histogram5->Fill(x_uscita - x_entrata);
			}

			if (x_entrata > -10 and x_entrata < 10) {
				//vHistograms.front()->Fill(x_uscita-x_entrata);
				histogram10->Fill(x_uscita - x_entrata);
			}
		}

		//TODO FITTING
		//Alla fine dovro' scrivere sul file dechanneling_table.txt

		/*
		 ****************
		 *				*
		 *      FIT!    *
		 *				*
		 ****************
		 */

		// Peak around zero fAmor, peak around thetab fChan, nuclear expo fDech
		// xmin = -30 and xmax=10 are for now empirical values estimeted looking at the graphs.
		// gaus = [0]*exp( ((x-[1])/[2])^2 )
		//const Double_t mean_fchan_estim = dati_cristalli[nome_cristallo][(int)FieldCrystalDataTable::bending_angle];
		const Double_t mean_fchan_estim =
				dati_cristalli.at(nome_cristallo)[(int) FieldCrystalDataTable::bending_angle];

		DBG(clog << "mean_dech_estim:" << mean_fchan_estim << endl
		; , ;)

		// First peak
		TF1* fAmor = new TF1("fAmor", "gaus", -30, 10);

		// Second peak
		TF1* fChan = new TF1("fChan", "gaus", mean_fchan_estim - 20,
				mean_fchan_estim + 20);

		// Clearer syntax than f->SetParameters(&param[0]); ...
		fAmor->SetParNames("ConstAm", "MeanAm", "SigmaAm");

		fChan->SetParNames("ConstCh", "MeanCh", "SigmaCh");

		// Starting numbers guessed by looking at the graphs
		fAmor->SetParameter("ConstAm", histogram5->GetEntries() / 2.0);
		fAmor->SetParameter("MeanAm", -5);
		fAmor->SetParameter("SigmaAm", 8);

		// Starting numbers guessed by looking at the graphs
		fChan->SetParameter("ConstCh", histogram5->GetEntries() / 2.0);
		fChan->SetParameter("MeanCh", mean_fchan_estim);
		fChan->SetParameter("SigmaCh", 6.5);

		//TH1::Fit(const char* fname, Option_t* option, Option_t* graphicoption, Axis_t xmin, Axis_t xmax ) a;
		/* Option_T* option =
		 "W" Set all errors to 1
		 "I" Use integral of function in bin instead of value at bin center
		 "L" Use Loglikelihood m ethod (default is chisquare method)
		 "LL" Use Loglikelihood method and bin contents are not integers)
		 "U" Use a User specified fitting algorithm (via SetFCN)
		 "Q" Quiet mode (minimum printing)
		 "V" Verbose mode (default is between Q and V)
		 "E" Perform better Errors estimation using Minos technique
		 "B" Use this option when you want to fix one or more parameters
		 and the fitting function is like "gaus","expo","poln","landau".
		 “M" More. Improve fit results
		 "R" Use the Range specified in the function range
		 "N" Do not store the graphics function, do not draw
		 "0" Do not plot the result of the fit.
		 "+" Add this new fitted function to the list of fitted functions.
		 *
		 */
		histogram5->Fit(fAmor, "IREM+");
		histogram10->Fit(fAmor, "IREM+");

		histogram5->Fit(fChan, "IREM+");
		histogram10->Fit(fChan, "IREM+");

		TF1 *fitResultAm5 = histogram5->GetFunction("fAmor");
		TF1 *fitResultCh5 = histogram5->GetFunction("fChan");

		TF1 *fitResultAm10 = histogram10->GetFunction("fAmor");
		TF1 *fitResultCh10 = histogram10->GetFunction("fChan");



		/*
		 *  Dechanneling exponential
		 */


		// Lets fit the exponential of the dechanneling starting from +3 sigma right of amorphous peak to -3 sigma left of channeling peak


		// GetFunction does not copy the parameters' name?
		constexpr int MeanAm = 1;
		constexpr int MeanCh = 1;

		constexpr int SigmaAm = 2;
		constexpr int SigmaCh = 2;


		auto meanAm5  = fitResultAm5->GetParameter(MeanAm);
		auto sigmaAm5 = fitResultAm5->GetParameter(SigmaAm);

		DBG(clog << " meanAm5: " << meanAm5 << endl
				 << " sigmaAm5: " << sigmaAm5 << endl;, ; )

		auto meanCh5  = fitResultCh5->GetParameter(MeanCh);
		auto sigmaCh5 = fitResultCh5->GetParameter(SigmaCh);

		DBG(clog << " meanCh5: " << meanCh5 << endl
				 << " sigmaCh5: " << sigmaCh5 << endl;, ; )


		auto meanAm10  = fitResultAm10->GetParameter(MeanAm);
		auto sigmaAm10 = fitResultAm10->GetParameter(SigmaAm);

		auto meanCh10  = fitResultCh10->GetParameter(MeanCh);
		auto sigmaCh10 = fitResultCh10->GetParameter(SigmaCh);

		TF1* fDech5  = new TF1("fDech", "expo", meanAm5 + 3 * sigmaAm5, meanCh5 - 3 * sigmaCh5);
		TF1* fDech10 = new TF1("fDech", "expo", meanAm10 + 3 * sigmaAm10, meanCh10 - 3 * sigmaCh10);

		DBG(clog << "xmindech: " << meanAm5 + 3 * sigmaAm5
				<< " xmaxdech: " << meanCh5 - 3 * sigmaCh5 << endl;, ;)

		// expo:  exp([0]+[1]*x)
		fDech5->SetParNames("ConstDc", "SlopeDc");
		fDech10->SetParNames("ConstDc", "SlopeDc");

		//fDech->SetParameter("MeanAm",-5);

		histogram5 ->Fit(fDech5, "IREM+");
		histogram10->Fit(fDech10, "IREM+");


		// Ld = [1]*10e6*Rc




		//TCanvas* mio_c1 = new TCanvas();
		//vCanvases.push_back(new TCanvas(nome_cristallo.c_str()));
		//vHistograms.front()->Draw();
		//histogram5->Draw();
		root_output->Write();

	} else {
		DBG(
				clog << nome_cristallo << ": File .dat not found" << endl; clog << "Nome cercato: " << pathfiledati << endl; clog << "Current Dir: " << system("pwd") << endl;,
				;)
		clog << "[WARNING] For crystal " << nome_cristallo
				<< ", File .dat not found" << endl
				<< "          Trying to use old dech.C macro and .root data file"
				<< endl << endl;

		//ROOT_PROJDIR->cd();
		//Todo rimetterla;
		//dech(nome_cristallo,output_dech);
	}

}

}
