/*
 * slices.C
 *
 *  Created on: Nov 23, 2016
 *      Author: fforcher
 */

#include <iostream>
#include <sstream>
#include <cmath>
#include <map>
#include <string>
#include <memory>
#include <fstream>


#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
#include "TRandom.h"
#include "TRandom3.h"
#include <TCanvas.h>


#include "./dbg_macro.h"
//#include "./my_typedefs.h"


extern char PROJECT_DIR[];


namespace mions {
using std::string;
using std::ifstream;
using std::stringstream;
using std::pow;
using std::endl;
using std::cout;
using std::clog;
using std::cerr;



//void slices(string nome_cristallo = "STF45",
//		 string nomefiledati_dat = "",
// 	 	 string nomefiledati_root,
//		 Double cut1,
//		 Double cut2
//		 ) {

//void slices(string nome_cristallo, string pathfiledati_root, Double_t cut1, Double_t cut2, TH1D*& hist, bool makepng) {
void slices(std::string nome_cristallo, std::string exp_or_sim, Double_t cut1, Double_t cut2, TH1D*& hist, std::map<std::string, double> otherparams, bool makepng) {
	//auto nome_cristallo = string("ST101");

	auto pathfiledati_root = string("ForFrancesco/") + nome_cristallo + "_" + exp_or_sim
			+ string("/recoDataSimple_renamed.torsion.correction.histo.root");


	//auto nome_cristallo = string("STF45");
	//auto pathfiledati_root = string("ForFrancesco/STF45_sim/recoDataSimple_renamed.torsion.correction.histo.root");



//	auto histogram = new TH1D(
//	/* name */nomehisto.c_str(),
//	/* title */titlehisto.c_str(),
//	/* X-dimension */2, -40, 40 );

	auto in_file_root = new TFile( pathfiledati_root.c_str() );
	// Check if .root exist
	if (!(in_file_root->IsOpen())) {
		cerr << "[ERROR] Crystal " << nome_cristallo << endl;
		cerr << "[ERROR]: File .root not found!" << endl;
		return;
	}

	auto h2D = (TH2D*) in_file_root->Get( "dTheta_x_vs_Impact_x_cor" );

	//TODO
	//h2D->Smooth();


	//auto h1D = h2D->ProjectionY();

	auto axis = h2D->GetXaxis();

	auto bincut1 = axis->FindBin( cut1 );
	auto bincut2 = axis->FindBin( cut2 );

	//Name the image
	stringstream ss;
	ss << "Slices_" << cut1 << "_" << cut2 << "_" << nome_cristallo;
	//ss << "Slices_" << bincut1 << "_" << bincut2 << "_" << nome_cristallo;
	string nomehisto;
	string titlehisto;
	ss >> nomehisto;
	titlehisto = nomehisto;


	//cout << "bin cut" << endl;
	//cout << "  -5 -> " << min5 << "   5 -> " << max5 << endl;
	//cout << " -10 -> " << min10 << "  10 -> " << max10 << endl;

	auto histogram = (TH1D*) (h2D->ProjectionY( titlehisto.c_str(), bincut1, bincut2 ));
	//h5->GetXaxis()->SetTitle( "#Delta#theta_{x} [#murad]" );
	histogram->Rebin( 1 );
	histogram->GetXaxis()->SetRangeUser(-40,30);
	//histogram->GetYaxis()->SetRangeUser(1,1000);

	hist = histogram;
	if (makepng) {

		auto c_slices = new TCanvas("c_slices", "c_slices");
		histogram->Draw();

		auto nomefilepng = "Varie/Video_" + nome_cristallo+"_"+exp_or_sim + "/"+ nomehisto + ".png";
		c_slices->SaveAs(nomefilepng.c_str());
	}

	//TODO Save to txt file the slice data FindBin
	int lowYrange;
	int highYrange;
	if (otherparams.empty()) {
		lowYrange = -40;
		highYrange = 30;
	} else {
		lowYrange  = otherparams.at("lowYrange_slices");
		highYrange = otherparams.at("highYrange_slices");
	}
	auto minbinofrange = h2D->GetYaxis()->FindBin(lowYrange);
	auto maxbinofrange = h2D->GetYaxis()->FindBin(highYrange);
	auto nbinshist = h2D->GetYaxis()->GetLast();
	DBG( std::clog << "minbinofrange: " << minbinofrange << std::endl; , ; )
	DBG( std::clog << "maxbinofrange: " << maxbinofrange << std::endl; , ; )
	DBG( std::clog << "nbinshist: " << nbinshist << std::endl; , ; )


	Double_t* data_hist = new Double_t[nbinshist];
	//for (auto i = 0; i < nbinshist; i++) data_hist[i] = hist->GetBinContent(i + 1);
	for (auto i = minbinofrange; i < maxbinofrange; i++) data_hist[i] = hist->GetBinContent(i + 1);

	std::string nomefile_datitxt = "ForFrancesco/"+nome_cristallo
									+"_"+exp_or_sim+"/"+
									"txt_data/"+nomehisto+".txt";
	std::ofstream txt_file(nomefile_datitxt);
	for (auto i = minbinofrange; i < maxbinofrange; i++) {
		txt_file << hist->GetXaxis()->GetBinCenter(i+1) << "    " << data_hist[i] << endl;
	}

}

}


