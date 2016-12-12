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
#include <utility> //pair
#include <algorithm> //sort
#include <map> //map
#include <assert.h> //assert

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
	auto start_analysis = 140; // At what thetaX angle [murad] we should start the slicing
	auto analysis_width = 56; // For how many thetaX we analyze, from start_analysis
	vector<TH1D*> vhTRANS(60);
	// Assume we start in the VR region (one peak around -Rc*bending)
	int regVR = start_analysis; // When the VR region starts. We assume we are in it when we start. [murad]
	int regVRAM = 0;   // When the pure VR region ends and the first AM peak appear. [murad]
	int regAM = 0;     // When the last VR peak has disappeared, we are fully in the AM region. [murad]
	int last_nfound = 0;   // The number of peaks found in the previous cycle, to detect transitions.
	// NOTE: using "zero" as the false value assumes we never cross it in analysis,
	// for example start_analysis = -20 and analysis_width = 40 could not work as intended
	// TODO: fare grafico di VR e AM
	struct GaussParams {
		Double_t constant;
		Double_t mean;
		Double_t sigma;

		GaussParams(Double_t constant_,Double_t mean_,Double_t sigma_) :
			constant(constant_),
			mean(mean_),
			sigma(sigma_) {};
	};

	// Here, vector<GaussParams> has either one or two elements, depending if there are two peaks.
	map<Double_t, vector<GaussParams> > mPeaks; // All peaks. Map from thetax to that slice's peak(s) params
	map<Double_t,GaussParams> mVR; // Save the VR peaks
	map<Double_t,GaussParams> mAM; // Save the AM peaks
	map<Double_t,Double_t> mNorm; // Save the normalization (and incidentally, also all possible keys/slices)
	//vector<Double_t> vTwoPeaks; vTwoPeaks.reserve(15); // Save the positions which have two peaks
	// If you change smth, eg the i's increment, remember to change it also on the vector-filling loop (under this one)
	for (auto i = 0; i < analysis_width; i=i+deltaslice)
	{
		cout << "ii: " << i <<endl;
		auto& hTRANS = vhTRANS[i];
		//TODO fit totale

		//slices( 160 + i + 11 - 1, 160 + i + 11 + 1, hTRANS );
		slices( start_analysis + i /*- 1*/, start_analysis + i + deltaslice - 1, hTRANS );


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
		auto thresoldpeaks = 0.09; // Minimum peak height relative to max bin
		TSpectrum *s = new TSpectrum( 2 * npeaks );
		Int_t nfound = s->Search( hTRANS, expectedsigma, "", thresoldpeaks );
		printf( "Found %d candidate peaks in hTRANS to fit\n", nfound );
		s->Print();



		//Peak search smooth
		TSpectrum *s_sm = new TSpectrum( 2 * npeaks );
		Int_t nfound_sm = s_sm->Search( hSmooth, expectedsigma, "", thresoldpeaks );
		printf( "Found %d candidate peaks in hSmooth to fit\n", nfound_sm );
//		s_sm->Print();
		// Double_t *xpeaks_sm = s_sm->GetPositionX();
		// Double_t *ypeaks_sm = s_sm->GetPositionY();



		if (nfound != 1 and nfound != 2) {

			cerr << "[ERROR]: Found " << nfound << " peaks "
					"instead of 1 or 2" << endl;
			cerr << "[ERROR]: Found " << nfound_sm << " smoothed peaks "
					"instead of 1 or 2" << endl;

			return;
		} else {
			clog << "[LOG] Found " << nfound << "peak(s)" << endl;

			// Check the transitions from the various regions, in a robust way.
			// The second peak can appear and disappear before reappearing again,
			// so we define the transition VRAM as the maximum interval, between
			// the first appearance of two peaks and their very last appearance.
			if (nfound == 1 and regVRAM == 0) {
				clog << "[LOG] VR region" << endl;
			} else if (nfound == 2 and regVRAM == 0) {
				clog << "[LOG] Start of the VRAM region" << endl;
				regVRAM = start_analysis + i;
			} else if (nfound == 1 and last_nfound == 2 and regVRAM != 0) {
				// We cannot know for sure if there is gonna be another peak
				// until all the slices have been analyzed.
				clog << "[LOG] Could it be " << start_analysis + i << " the end of VRAM and start of AM?" << endl;
				regAM = start_analysis + i;
			}

//					cerr << "[LOG] Found " << nfound_sm << " smoothed peak(s)" << endl;
			//	return;
		}
		// Now select if we fit with one or two gaussians
		auto& fVRAM =  nfound == 1 ? fVRAMsing : fVRAMdoub;

		// update last_nfound
		last_nfound = nfound;

		//Save the peaks found by TSPectrum
		vector<pair<Double_t, Double_t> > xypeaks; xypeaks.reserve(2);
		for (int i = 0; i < nfound; ++i) {
			auto x = s->GetPositionX()[i];
			auto y = s->GetPositionY()[i];

			auto xy = make_pair(x,y);

			xypeaks.push_back(xy);
		}

		//Sort the peaks
		for ( int i = 0; i < nfound; ++i ) {
			clog << "[LOG] X" << i << ": " << get<0>(xypeaks[i]) << "  ";
			clog << "Y" << i << ": " << get<1>(xypeaks[i]) << endl;
		}

		std::sort(xypeaks.begin(), xypeaks.end());

		clog << "[LOG] sorting peaks..." << endl;
		for ( int i = 0; i < nfound; ++i ) {
			clog << "[LOG] X" << i << ": " << get<0>(xypeaks[i]) << "  ";
			clog << "Y" << i << ": " << get<1>(xypeaks[i]) << endl;
		}






//		//Set parameters, pars from 0 to 2 are from VR, pars from 3 to 5 are from AM
//		auto startconst = hTRANS->Integral();
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

		//Setting initial parameters for the total fit, using peaks info
		{
			constexpr int X = 0;
			constexpr int Y = 1;
			// This is the VR or AM one, starting
			fVRAM->SetParameter( 0, get<Y>(xypeaks[0]) );
			fVRAM->SetParameter( 1, get<X>(xypeaks[0])  );
			fVRAM->SetParameter( 2, expectedsigma + 4 );

			if (nfound == 2) {
				// Since I order the peaks, when there are two peaks this is the AM one,
				// on the right (bigger X coordinate, near zero)
				fVRAM->SetParameter( 3, get<Y>(xypeaks[1]) );
				fVRAM->SetParameter( 4, get<X>(xypeaks[1]) );
				fVRAM->SetParameter( 5, expectedsigma + 2 );
			}
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
		TFitResultPtr fit_hTRANS = hTRANS->Fit( fVRAM, "IREM+" );
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


		{
			// Now, save parameters of the peaks in an array
			//
			Double_t parVRAM[6] = { 0 };
			fitResultVRAM->GetParameters( parVRAM );

			//Assuming the fit didn't flip the peaks...
			auto trans_const1 = fitResultVRAM->GetParameter( CONSTANT );
			auto trans_mean1 = fitResultVRAM->GetParameter( MEAN );
			auto trans_sigma1 = fitResultVRAM->GetParameter( SIGMA );
			GaussParams tmp(trans_const1,trans_mean1,trans_sigma1);
			vector<GaussParams> vtmp;
			vtmp.push_back(tmp);
//			tmp.constant = trans_const1;
//			tmp.mean = trans_mean1;
//			tmp.sigma = trans_sigma1;


			if (nfound == 2) {
				auto trans_const2 = fitResultVRAM->GetParameter( CONSTANT + 3 );
				auto trans_mean2 = fitResultVRAM->GetParameter( MEAN + 3 );
				auto trans_sigma2 = fitResultVRAM->GetParameter( SIGMA + 3 );
				GaussParams tmp2(trans_const2,trans_mean2,trans_sigma2);
				vtmp.push_back(tmp2);
			}

			mPeaks.emplace( make_pair(double(start_analysis + i),vtmp) );
		}

		mNorm.emplace( double( start_analysis + i ), fitResultVRAM->Integral(-40,30) );


/*
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
/*
		cout << "int_fVR2: " << int_fVR2 << endl;
		cout << "int_fAM2: " << int_fAM2 << endl;

		cout << "eff: " << int_fVR2 / int_fAM2  << endl;
		cout << "if: " << i <<endl;
*/

	}



	/*
	 * Place the peaks in the right array.
	 * We use a simple test:
	 * if there is two, the left one (lesser mean) is VR, the other is
	 * if there is only one, then the closest to 0 is the AM
	 * The last condition is somewhat suspect, even if it is theoretically sound.
	 * Still, we prefer a simple test not involving the complexities of a multi-slice comparison,
	 * and on STF45 it seems to suffice.
	 */
	// Idea, use the first slice as the mean VR and the last as the AM.
	// NOTE regAM contains the last 2->1 peak number transition found, so if analysis_width
	// is too small, we can't be sure that we are fully in the AM region, but this can be checked by looking
	// at the 2D graph. More important, even then we cannot know in the big slices loop where exactly we are unti
	// it ends.
	cout << "\n\nTEST PICCHI" << endl;
	for (const auto& t : mPeaks) {
		if (t.second.size() == 1) {
			cout << "Slice " << t.first << ":\n\t " << t.second[0].mean << endl;
		} else {
			cout << "Slice" << t.first << ":\n\t " << t.second[0].mean << endl;
			cout << "\t " << t.second[1].mean << endl;
		}
	}

	DBG( std::clog << "qua ci sono: [WARNING] " << std::endl
	; , ; )
	auto media_VR = mPeaks.at( start_analysis )[0].mean;
	auto media_AM = mPeaks.at( start_analysis + analysis_width - deltaslice )[0].mean; //Remember these are maps not vectors

	for (auto i = start_analysis; i < start_analysis + analysis_width; i = i + deltaslice) {
		//Double_t i = start_analysis + j;
		DBG( std::clog << "[LOG] i: " << i << std::endl
		; , ; )
		auto& peaks = mPeaks.at( i ); // Get a reference to the vector with one or two peaks of this slice

		if (i < regVRAM) {
			// VR
			// Should be only one peak here
			assert( peaks.size() == 1 );
			mVR.emplace( i, peaks[0] ); // Get the only peak at position (slice) thetax = i

		} else if (i >= regVRAM and i < regAM and peaks.size() == 2) {
			// VRAM
			// We are in VRAM e we found two peaks, all ok
			// TODO sort them and assign accordingly

			//The first has
			std::sort( peaks.begin(), peaks.end(), [](const GaussParams& a,const GaussParams& b) {
				return a.mean < b.mean;
			} );

			mVR.emplace( i, peaks[0] );
			mAM.emplace( i, peaks[1] );

		} else if (i >= regVRAM and i < regAM and peaks.size() == 1) {
			//VRAM

			// We are in VRAM but we found only one peaks, we have to identify which one
			// we use the simple test descripted above, the less distance from a "sure" AM or VR peak
			if (abs( peaks[0].mean - media_VR ) < abs( peaks[0].mean - media_AM )) {
				mVR.emplace( i, peaks[0] );
			} else {
				mAM.emplace( i, peaks[0] );
			}
		} else if (i >= regAM) {
			// AM
			// Should be only one peak here
			assert( peaks.size() == 1 );

			mAM.emplace( i, peaks[0] );
		}
	}


	//Output some peak information
	cout << "\n\nPICCHI VR" << endl;
	for (const auto& t : mVR) {
		cout << "" << t.first << " " << t.second.constant << endl;
	}
	cout << "\n\nPICCHI AM" << endl;
	for (const auto& t : mAM) {
		cout << "" << t.first << " " << t.second.constant << endl;
	}

	cout << "\n\nPICCHI VR, normalized" << endl;
	for (const auto& t : mVR) {
		//cout << "" << t.first << " " << mVR.at(t.first).constant / mNevents.at(t.first)  << endl;
		cout << "" << t.first << " " <<
				t.second.constant * sqrt(2 * M_PI) * t.second.sigma / mNorm.at(t.first) << endl;
	}
	cout << "\n\nPICCHI AM, normalized" << endl;
	for (const auto& t : mAM) {
		cout << "" << t.first << " " <<
				t.second.constant * sqrt(2 * M_PI) * t.second.sigma / mNorm.at(t.first) << endl;
	}

	//For


	cout << "[LOG]: Start of VRAM transition: " << regVRAM << endl;
	cout << "[LOG]: Start of AM region: " << regAM << endl;





	return;
}

}

