/*
 * analisi_VRtoAM.C
 *
 *  Created on: Nov 24, 2016
 *      Author: fforcher
 */
#include <iostream>
#include <vector>
//#include <fstream>
//#include <memory>
//#include <cstdlib>
#include <unistd.h>
//#include <map>
//#include <sstream>
//#include <array>
//#include <cmath>

#include <TH1.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>


#include "slices.h"
#include "dbg_macro.h"
#include "my_typedefs.h"


namespace mions {


void analisi_VRtoAM() {
	using namespace std;
	using mions::slices;

	auto deltaslice = 1; //[murad]
	system( "rm -f Varie/Video/*.png" );
	system( "rm -f Varie/Video/GIF_slices.gif" );
	vector<TH1D*> vhist;
	vhist.reserve( 32 );
	for (int i = 161; i < 189; i = i + 1) {
		TH1D* thistogram;
		slices( i-1, i + 1, thistogram, true );
		vhist.push_back( thistogram );
	}

	// Requires imagemagick
	// sudo apt-get install imagemagick
	auto gifreturnval = system( "convert -delay 20 -loop 0 Varie/Video/*.png Varie/Video/GIF_slices.gif" );
	if (bool( gifreturnval )) {
		clog << "[WARNING]: Failed making the .gif" << endl;
		clog << "[WARNING]: Maybe you have to install imagemagick?" << endl;
		clog << "[WARNING]: sudo apt-get install imagemagick" << endl;
	}

	TH1D* hVR;
	TH1D* hAM;
	TH1D* hTRANS;
	slices( 160, 160 + deltaslice, hVR );
	slices( 190, 190 + deltaslice, hAM );
	slices( 160 + 11 - 1, 160 + 11 + 1, hTRANS );

	// First gaussian
	TF1* fVR = new TF1( "fVR", "gaus", -40, 40 );
	// Second gaussian
	TF1* fAM = new TF1( "fAM", "gaus", -40, 40 );

	hVR->Fit( fVR, "IREM+" );
	hAM->Fit( fAM, "IREM+" );

	TF1 *fitResultVR = hVR->GetFunction( "fVR" );
	TF1 *fitResultAM = hAM->GetFunction( "fAM" );

	Double_t parVR[3] = { 0 };
	Double_t parAM[3] = { 0 };

	fitResultVR->GetParameters(parVR);
	fitResultAM->GetParameters(parAM);

	// Explicit variables for parameters
	// gaus = [0]*exp( ((x-[1])/[2])^2 )
	constexpr int CONSTANT = 0;
	constexpr int MEAN = 1;
	constexpr int SIGMA = 2;

	auto constVR = fitResultVR->GetParameter( CONSTANT );
	auto meanVR = fitResultVR->GetParameter( MEAN );
	auto sigmaVR = fitResultVR->GetParameter( SIGMA );

	//auto constAM = fitResultAM->GetParameter( CONSTANT );
	auto meanAM = fitResultAM->GetParameter( MEAN );
	auto sigmaAM = fitResultAM->GetParameter( SIGMA );

	clog << " meanVR: " << meanVR << endl << " sigmaVR: " << sigmaVR << endl;
	clog << " meanAM: " << meanAM << endl << " sigmaAM: " << sigmaAM << endl;


	TCanvas* c_fitVR = new TCanvas("c_fitVR","c_fitVR");
	hVR->Draw();
	TCanvas* c_fitAM = new TCanvas("c_fitAM","c_fitAM");
	hAM->Draw();





	//TODO fit totale
	TF1* fVRAM = new TF1( "fVRAM", "gaus(0) + gaus(3)", -40, 40 );
	//Set parameters, pars from 0 to 2 are from VR, pars from 3 to 5 are from AM
	fVRAM->SetParameter(0,constVR / 2);
	fVRAM->SetParameter(3,constVR / 2);
	for (int i = 1; i < 3; ++i) {
		fVRAM->SetParameter(i,parVR[i]);
		fVRAM->SetParameter(i+3,parAM[i]);
	}

	TCanvas* c_fitVRAM = new TCanvas("c_fitVRAM","c_fitVRAM");
	c_fitVRAM->cd();

	hTRANS->Fit( fVRAM, "IREM+" );
	hTRANS->Draw();


	TF1 *fitResultVRAM = hTRANS->GetFunction( "fVRAM" );

	//TFitResultPtr fit_hTRANS = hTRANS->Fit( fVRAM, "IREM+" );

	// Parameters of the two peaks during transition
	// In an array and also as named variables
	Double_t parVRAM[6] = { 0 };
	fitResultVRAM->GetParameters(parVRAM);

	auto trans_constVR = fitResultVRAM->GetParameter( CONSTANT );
	auto trans_meanVR = fitResultVRAM->GetParameter (  MEAN );
	auto trans_sigmaVR = fitResultVRAM->GetParameter( SIGMA );

	auto trans_constAM = fitResultVRAM->GetParameter( CONSTANT + 3 );
	auto trans_meanAM = fitResultVRAM->GetParameter (  MEAN + 3 );
	auto trans_sigmaAM = fitResultVRAM->GetParameter( SIGMA + 3 );


	// First gaussian PEAK
	TF1* fVR2 = new TF1( "fVR", "gaus", -40, 40 );
	// Second gaussian PEAK. The sum of the two is fVRAM
	TF1* fAM2 = new TF1( "fAM", "gaus", -40, 40 );

	//Get the parameters for the peaks from fVRAM
	for (int i = 0; i < 3; ++i) {
		fVR2->SetParameter(i,parVRAM[i]);
		fAM2->SetParameter(i,parVRAM[i+3]);
	}



	auto int_fVR2 = fVR2 -> Integral(trans_meanVR - 3*trans_sigmaVR,
									trans_meanVR + 3*trans_sigmaVR);
	auto int_fAM2 = fAM2 -> Integral(trans_meanAM - 3*trans_sigmaAM,
										trans_meanAM + 3*trans_sigmaAM);

	cout << "int_fVR2: " << int_fVR2 << endl;
	cout << "int_fAM2: " << int_fAM2 << endl;

	cout << "eff: " << int_fVR2 / int_fAM2  << endl;


	return;
}

}

