/*
 * read_histograms.C
 *
 *  Created on: Oct 28, 2016
 *      Author: fforcher
 */

#include <iostream>
#include <sstream>
#include <cmath>
//#include <map>
#include <string>
#include <memory>
#include <fstream>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"

#include "dbg_macro.h"
#include "DatiChanneling.h"

extern bool PREFER_DAT_FILES;

namespace mions {
using std::string;
using std::ifstream;
using std::stringstream;
using std::pow;
using std::endl;
using std::cout;
using std::clog;
using std::cerr;



//TODO Finire! Aggiungere tutte le variabili nei parametri
void read_histograms(string nome_cristallo,
					 string nomefiledati_dat,
		 	 	 	 string nomefiledati_root,
					 TH1D*& histogram5,
					 TH1D*& histogram10
					 ) {


	//We should already be inside the right folder

	auto pathfiledati_dat = nomefiledati_dat;
	auto pathfiledati_root = nomefiledati_root;


	// select +- 5 microrad in nomehisto5, +-10 in nomehisto10
	string nomehisto5 = "hdati5_" + nome_cristallo;
	string nomehisto10 = "hdati10_" + nome_cristallo;
	string titlehisto5 = nome_cristallo + ", cuts at +- 5 microrad";
	string titlehisto10 = nome_cristallo + ", cuts at +- 10 microrad";
	clog << nomehisto5 << endl;


	ifstream file_dat( pathfiledati_dat );
	auto in_file_root = new TFile( pathfiledati_root.c_str() );
	if (bool( file_dat ) and (PREFER_DAT_FILES or not in_file_root->IsOpen())) {
		// Il codice per la mia analisi qua

		//gStyle->SetPalette(1);
		//gStyle->SetOptStat(1);
		//gStyle->SetOptTitle(1);
		//TGaxis::SetMaxDigits(3);

		/*
		DBG(
				clog << "[LOG]: " << "Crystal " << nome_cristallo << endl; clog << "[LOG]: File "<< nomefiledati_dat << endl << endl;,
				; )
		clog << "[LOG]: Using .dat file";
		 */

		//ifstream file_dat(pathfiledati);

		//Riempi gli istogrammi
		DatiChanneling dati( pathfiledati_dat );
		auto in_file_root = new TFile( nomefiledati_root.c_str() );

		EventoDechanneling ev;
		auto datisize = dati.getSize();

		auto histogram5_dat = new TH1D(
		/* name */nomehisto5.c_str(),
		/* title */titlehisto5.c_str(),
		/* X-dimension */600 / 4, -200, 400 );

		auto histogram10_dat = new TH1D(
		/* name */nomehisto10.c_str(),
		/* title */titlehisto10.c_str(),
		/* X-dimension */600 / 4, -200, 400 );

		//dati.print(datisize);

		//Read events and select those with entry angle between -+5 and +-10 microrad
		for (ULong64_t i = 0; i < datisize; ++i) {

			ev = dati.getEvent( i );
			Double_t x_entrata = ev[0];
			Double_t x_uscita = ev[2];

			if (x_entrata > -5 and x_entrata < 5) {
				//vHistograms.front()->Fill(x_uscita-x_entrata);
				histogram5_dat->Fill( x_uscita - x_entrata );
			}

			if (x_entrata > -10 and x_entrata < 10) {
				//vHistograms.front()->Fill(x_uscita-x_entrata);
				histogram10_dat->Fill( x_uscita - x_entrata );
			}
		}
		histogram5 = histogram5_dat;
		histogram10 = histogram10_dat;
		// Technically not necessary now, but maybe I'll add more conditions
	} else if (not bool( file_dat ) or (not PREFER_DAT_FILES or in_file_root->IsOpen())) {

		// Check if .root exist
		if (!(in_file_root->IsOpen())) {
			cerr << "[ERROR] Crystal " << nome_cristallo << endl;
			cerr << "[ERROR]: File .root not found!" << endl;
			return;
		}

		auto h2D = (TH2D*) in_file_root->Get( "dTheta_x_vs_Impact_x_cor" );
		//auto h1D = h2D->ProjectionY();

		auto axis = h2D->GetXaxis();

		auto min5 = axis->FindBin( -5. );
		auto max5 = axis->FindBin( 5. );

		auto min10 = axis->FindBin( -10. );
		auto max10 = axis->FindBin( 10. );

		cout << "bin cut" << endl;
		cout << "  -5 -> " << min5 << "   5 -> " << max5 << endl;
		cout << " -10 -> " << min10 << "  10 -> " << max10 << endl;

		auto h5 = (TH1D*) (h2D->ProjectionY( titlehisto5.c_str(), min5, max5 ));
		h5->GetXaxis()->SetTitle( "#Delta#theta_{x} [#murad]" );
		h5->Rebin( 4 );
		auto h10 = (TH1D*) (h2D->ProjectionY( titlehisto10.c_str(), min10, max10 ));
		h10->GetXaxis()->SetTitle( "#Delta#theta_{x} [#murad]" );
		h10->Rebin( 4 );

		//Crystal qmp36 has been mounted reversed, so we must flip the graph along x axis
		if (nome_cristallo == string( "QMP36" )) {
			clog << "QMP36, flipping the histogram orizontally" << endl;
			auto h5_qmp36_new = new TH1D(
			/* name */nomehisto5.c_str(),
			/* title */titlehisto5.c_str(),
			/* X-dimension */800 / 4, -400, 400 );
			auto h10_qmp36_new = new TH1D(
			/* name */nomehisto10.c_str(),
			/* title */titlehisto10.c_str(),
			/* X-dimension */800 / 4, -400, 400 );
			auto nbin = h5->GetNbinsX();

			for (auto i = 0; i <= nbin; ++i) {
				h5_qmp36_new->SetBinContent( nbin - i, h5->GetBinContent( i ) );
				h10_qmp36_new->SetBinContent( nbin - i, h10->GetBinContent( i ) );
			}

			histogram5 = h5_qmp36_new;
			histogram10 = h10_qmp36_new;

			histogram5->SetTitle( titlehisto5.c_str() );
			histogram10->SetTitle( titlehisto10.c_str() );

			//h5_qmp36

		} else {
			histogram5 = h5;
			histogram10 = h10;

			histogram5->SetTitle( titlehisto5.c_str() );
			histogram10->SetTitle( titlehisto10.c_str() );
		}
	} else {
		cerr << "[ERROR]: Logic error, at line: " << __LINE__ << endl;
		return;
	}

}

}
