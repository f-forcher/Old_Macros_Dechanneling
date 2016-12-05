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
#include <string>
#include <cmath> //ceil floor
#include <chrono>//milliseconds
#include <thread>//sleep

#include <TH1.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TSpectrum.h>

#include "slices.h"
#include "dbg_macro.h"
#include "my_typedefs.h"


namespace mions {


void analisi_VRtoAM() {
	using namespace std;
	using mions::slices;

	auto deltaslice = 2; //[murad]
	system( "rm -f Varie/Video/*.png" );
	system( "rm -f Varie/Video/GIF_slices.gif" );
	vector<TH1D*> vhist;
	vhist.reserve( 32 );
	for (int i = 161; i < 189; i = i + 1) {
		TH1D* thistogram;
		slices( i-1, i + 1, thistogram, false );
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

	slices( 160, 160 + deltaslice, hVR );
	slices( 190, 190 + deltaslice, hAM );


	// First gaussian
	TF1* fVR = new TF1( "fVR", "gaus", -40, 30 );
	// Second gaussian
	TF1* fAM = new TF1( "fAM", "gaus", -40, 30 );

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
	c_fitVR->cd();
	hVR->Draw();
	TCanvas* c_fitAM = new TCanvas("c_fitAM","c_fitAM");
	c_fitAM->cd();
	hAM->Draw();


	auto howmanys = 2; //how many sigmas we integrate
	auto VRl = hVR -> FindBin(meanVR - howmanys*sigmaVR); // VR low
	auto VRh = hVR -> FindBin(meanVR + howmanys*sigmaVR); // VR high
	auto AMl = hAM -> FindBin(meanAM - howmanys*sigmaAM); // AM low
	auto AMh = hAM -> FindBin(meanAM + howmanys*sigmaAM); // AM high
	auto intervallo_low = hVR->FindBin(-40); //  Should not matter what histo we use to get the number since they have the same range
	auto intervallo_high = hVR->FindBin(40);

	auto int_hVR = hVR -> Integral(VRl, VRh);
	auto int_VRtot = hVR -> Integral(intervallo_low, intervallo_high);

	auto int_hAM = hAM -> Integral(AMl, AMh);
	auto int_AMtot = hAM -> Integral(intervallo_low, intervallo_high);

	cout << "int_hVR2: " << int_hVR << endl;
	cout << "int_AM2: " << int_hAM << endl;
	cout << "eff VR: " << int_hVR / double(int_VRtot)  << endl;
	cout << "eff AM: " << int_hAM / double(int_AMtot)  << endl;




	//auto i = 5;
	vector<TH1D*> vhTRANS(24);
	for (auto i = 0; i < 24; ++i)
	{
		cout << "ii: " << i <<endl;
		auto& hTRANS = vhTRANS[i];
		//TODO fit totale

		//slices( 160 + i + 11 - 1, 160 + i + 11 + 1, hTRANS );
		slices( 160 + i + 1 /*- 1*/, 160 + i + 1 + 1, hTRANS );


		//hTRANS->Scale(1.0/hTRANS->Integral()); c1
		//TF1* fVRAM = new TF1( "fVRAM", "[0]*exp(-0.5 * std::pow( ( x-[1]) / [2], 2) ) + (1-[0])*exp(-0.5 * std::pow( ( x-[4]) / [5], 2) )", -40, 40 );
		TF1* fVRAMdoub = new TF1( "fVRAM", "gaus(0) + gaus(3)", -40, 30 );
		TF1* fVRAMsing = new TF1( "fVRAM", "gaus(0)", -40, 30 );


		//hTRANS->Smooth();
		// Markov Smoothing
		TSpectrum *smoother = new TSpectrum( 4 ); // Random 4, I don't think is needed here.
		auto nbinsTRANS = hTRANS->GetXaxis()->GetLast();
		DBG( std::clog << "nbinsTRANS: " << nbinsTRANS << std::endl; , ; )
		Double_t* dataTRANS = new Double_t[nbinsTRANS];
		for (auto i = 0; i < nbinsTRANS; i++) dataTRANS[i] = hTRANS->GetBinContent(i + 1);

		TH1D *hSmooth = (TH1D*)(hTRANS->Clone());
		hSmooth->SetLineColor(kGreen);
		smoother->SmoothMarkov(dataTRANS,nbinsTRANS,3); //3, 7, 10
		for (auto i = 0; i < nbinsTRANS; i++) hSmooth->SetBinContent(i + 1,dataTRANS[i]);





		//Peak search
		auto npeaks = 2;
		auto expectedsigma = 4; // A little smaller through
		auto thresoldpeaks = 0.10; // Minimum peak height relative to max bin
		TSpectrum *s = new TSpectrum( 2 * npeaks );
		Int_t nfound = s->Search( hTRANS, expectedsigma, "", thresoldpeaks );
		printf( "Found %d candidate peaks in hTRANS to fit\n", nfound );
		s->Print();
		Double_t *xpeaks = s->GetPositionX();
		Double_t *ypeaks = s->GetPositionY();


		//Peak search smooth
		TSpectrum *s_sm = new TSpectrum( 2 * npeaks );
		Int_t nfound_sm = s_sm->Search( hSmooth, expectedsigma, "", thresoldpeaks );
		printf( "Found %d candidate peaks in hSmooth to fit\n", nfound_sm );
		s_sm->Print();
		Double_t *xpeaks_sm = s_sm->GetPositionX();
		Double_t *ypeaks_sm = s_sm->GetPositionY();


		auto& fVRAM = nfound == 1 ? fVRAMsing : fVRAMdoub;
		if (nfound != 1 and nfound != 2 ) {
			cerr << "[ERROR]: Found " << nfound << " peaks "
					"instead of 1 or 2"<< endl;
			cerr << "[ERROR]: Found " << nfound_sm << " smoothed peaks "
								"instead of 1 or 2"<< endl;

			return;
		} else {
					cerr << "[LOG] Found " << nfound << "peak(s)" << endl;
					cerr << "[LOG] Found " << nfound_sm << " smoothed peak(s)" << endl;
				//	return;
		}











//		//Set parameters, pars from 0 to 2 are from VR, pars from 3 to 5 are from AM
		auto startconst = hTRANS->Integral();
//		fVRAM->SetParameter(0,startconst / 2.0);
//		fVRAM->SetParameter(3,startconst / 2.0);
//		//fVRAM->FixParameter(4,startconst);
//		//fVRAM->SetParLimits(3,startconst-5, startconst + 5);
//		for (int i = 1; i < 3; ++i) {
//			fVRAM->SetParameter(i,parVR[i]);
//			fVRAM->SetParameter(i+3,parAM[i]);
//		}

/*
		fVRAM->SetParameter(0,ypeaks[0]);
		fVRAM->SetParameter(1,xpeaks[0]);
		fVRAM->SetParameter(2,expectedsigma+4);

		if (nfound == 2) {
			fVRAM->SetParameter(3,ypeaks[1]);
			fVRAM->SetParameter(4,ypeaks[1]);
			fVRAM->SetParameter(5,expectedsigma+4);
		}
*/

		fVRAM->SetParameter(0,ypeaks[0]);
		fVRAM->SetParameter(1,xpeaks[0]);
		fVRAM->SetParameter(2,expectedsigma+4);

		if (nfound == 2) {
			fVRAM->SetParameter(3,ypeaks[1]);
			fVRAM->SetParameter(4,ypeaks[1]);
			fVRAM->SetParameter(5,expectedsigma+2);
		}






		TCanvas* c_fitVRAM = new TCanvas("c_fitVRAM","c_fitVRAM");
		c_fitVRAM->cd();

		//hTRANS->Smooth();



		//hSmooth->Fit( fVRAM, "IREM+" );
		//hSmooth->Draw("SAME");

		//Partial fit
		TF1* fVRpre = new TF1( "fVRAM", "gaus(0)", -40, 30 );
		TF1* fAMpre = new TF1( "fVRAM", "gaus(0)", -40, 30 );
		// TODO usare sort o set per ordinare i picchi, quello piu' a destra e' VR



		//Total fit
		hTRANS->Fit( fVRAM, "REM+" );
		hTRANS->Draw("SAME");

		string nomehisto = hTRANS->GetName();
		auto nomefilepng = "Varie/Video/" + nomehisto + ".png";
		c_fitVRAM->SaveAs(nomefilepng.c_str());

		//std::this_thread::sleep_for(std::chrono::milliseconds(1500));

		TF1 *fitResultVRAM = hTRANS->GetFunction( "fVRAM" );
		//TF1 *fitResultVRAM = hSmooth->GetFunction( "fVRAM" );
		if (!fitResultVRAM) {
			cerr << "[ERROR]: Did not fit" << endl;
			cerr << "[ERROR]: i: " << i << endl;
			hSmooth->Draw();
			hTRANS->Draw("SAME");

			return;
		}

		//TFitResultPtr fit_hTRANS = hTRANS->Fit( fVRAM, "IREM+" );

/*
		// Parameters of the two peaks during transition
		// In an array and also as named variables
		Double_t parVRAM[6] = { 0 };
		fitResultVRAM->GetParameters(parVRAM);

		//auto trans_constVR = fitResultVRAM->GetParameter( CONSTANT );
		auto trans_meanVR = fitResultVRAM->GetParameter (  MEAN );
		auto trans_sigmaVR = fitResultVRAM->GetParameter( SIGMA );

		//auto trans_constAM = fitResultVRAM->GetParameter( CONSTANT + 3 );
		auto trans_meanAM = fitResultVRAM->GetParameter (  MEAN + 3 );
		auto trans_sigmaAM = fitResultVRAM->GetParameter( SIGMA + 3 );


		// First gaussian PEAK
		TF1* fVR2 = new TF1( "fVR2", "gaus", -40, 40 );
		// Second gaussian PEAK. The sum of the two is fVRAM
		TF1* fAM2 = new TF1( "fAM2", "gaus", -40, 40 );

		//Get the parameters for the peaks from fVRAM
		for (auto j = 0; j < 3; ++j) {
			fVR2->SetParameter(j,parVRAM[j]);
			fAM2->SetParameter(j,parVRAM[j+3]);
		}



		auto int_fVR2 = fVR2 -> Integral(trans_meanVR - howmanys*trans_sigmaVR,
										trans_meanVR + howmanys*trans_sigmaVR);
		auto int_fAM2 = fAM2 -> Integral(trans_meanAM - howmanys*trans_sigmaAM,
											trans_meanAM + howmanys*trans_sigmaAM);

		cout << "int_fVR2: " << int_fVR2 << endl;
		cout << "int_fAM2: " << int_fAM2 << endl;

		cout << "eff: " << int_fVR2 / int_fAM2  << endl;
		cout << "if: " << i <<endl;
*/

	}



	return;
}

}

