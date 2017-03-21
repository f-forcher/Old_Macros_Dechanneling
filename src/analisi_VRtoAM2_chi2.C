 /*
 * analisi_VRtoAM_chi2.C
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


void analisi_VRtoAM_chi2(std::string nome_cristallo, std::string exp_or_sim, int start_analysis,
		int analysis_width,
		std::map<std::string, double> other_params) {
	using namespace std;
	using mions::slices;




	auto deltaslice = 1; //[murad]
	auto chi2_threshold = other_params.at("chi2_threshold");

	{
		system( "rm -f Varie/Video/*.png" );
		system( "rm -f Varie/Video/GIF_slices.gif" );
		system( "rm -f Varie/Video/GIF_slices.gif" );

		string txt_files_remove = "rm -f ForFrancesco/" + nome_cristallo+"_"+exp_or_sim + "/txt_data/*.txt";
		system(txt_files_remove.c_str());
	}

	TH1D* hVR;
	TH1D* hAM;

	slices( nome_cristallo, exp_or_sim, start_analysis, start_analysis + deltaslice, hVR );
	slices( nome_cristallo, exp_or_sim, start_analysis + analysis_width, start_analysis
			+ analysis_width + deltaslice, hAM );


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




	//auto i = 5;

	//ST101
	//auto start_analysis = -210; // At what thetaX angle [murad] we should start the slicing
	//auto analysis_width = 70; // For how many thetaX we analyze, from start_analysis

	//STF45
	//auto start_analysis = 160; // At what thetaX angle [murad] we should start the slicing
	//auto analysis_width = 190 (o 30??); // For how many thetaX we analyze, from start_analysis

	vector<TH1D*> vhTRANS(200);
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
		slices( nome_cristallo, exp_or_sim, start_analysis + i /*- 1*/, start_analysis + i + deltaslice, hTRANS );


		//hTRANS->Scale(1.0/hTRANS->Integral()); c1
		//TF1* fVRAM = new TF1( "fVRAM", "[0]*exp(-0.5 * std::pow( ( x-[1]) / [2], 2) ) + (1-[0])*exp(-0.5 * std::pow( ( x-[4]) / [5], 2) )", -40, 40 );
		TF1* fVRAMdoub = new TF1( "fVRAM", "gaus(0) + gaus(3)", -40, 30 );
		TF1* fVRAMsing = new TF1( "fVRAM", "gaus(0)", -40, 30 );

		TF1* fGaussTest = new TF1( "fGaussTest", "gaus(0)", -40, 30 );
		TF1* fDoubleGaussTest = new TF1( "fDoubleGaussTest",  "gaus(0) + gaus(3)", -40, 30 );



		//hTRANS->Smooth();
		// Markov Smoothing
		TSpectrum *smoother = new TSpectrum( 4 ); // Random 4, I don't think is needed here.
		auto nbinsTRANS = hTRANS->GetXaxis()->GetLast();
		DBG( std::clog << "nbinsTRANS: " << nbinsTRANS << std::endl; , ; )
		Double_t* dataTRANS = new Double_t[nbinsTRANS];
		Double_t* dataHighRes = new Double_t[nbinsTRANS];
		for (auto i = 0; i < nbinsTRANS; i++) dataTRANS[i] = hTRANS->GetBinContent(i + 1);



		TH1D *hSmooth = (TH1D*)(hTRANS->Clone());
//		hSmooth->SetLineColor(kGreen);
//		smoother->SmoothMarkov(dataTRANS,nbinsTRANS,3); //3, 7, 10


		// Fitting the two test functions:
		// ** Setting the parameters for the double gaussian one
		{
			auto expectedsigma = sigmaVR;
			// This is the VR or AM one, starting
			fDoubleGaussTest->SetParameter( 0, hTRANS->GetMaximum() / 2.0 );
			fDoubleGaussTest->SetParameter( 1, meanVR  );
			fDoubleGaussTest->SetParameter( 2, expectedsigma );
			fDoubleGaussTest->SetParameter( 3, hTRANS->GetMaximum() / 2.0 );
			fDoubleGaussTest->SetParameter( 4, meanAM  );
			fDoubleGaussTest->SetParameter( 5, expectedsigma );
		}

		//  ** Test fits
		TFitResultPtr FRptr_fGaussTest = hTRANS->Fit( fGaussTest, "ISREM0+" );
		TFitResultPtr FRptr_fDoubleGaussTest = hTRANS->Fit( fDoubleGaussTest, "ISREM0+" );




		//TF1* fitResultTest = hTRANS->GetFunction("fGaussTest");
		auto test_chi2_1 = FRptr_fGaussTest->Chi2();
		auto test_chi2_2 = FRptr_fDoubleGaussTest->Chi2();

		auto test_ndf_1 = FRptr_fGaussTest->Ndf();
		auto test_ndf_2 = FRptr_fDoubleGaussTest->Ndf();
		auto test_numdata = hTRANS->GetEntries();
		auto test_param1 = 3; // Number of parameters of a gaussian
		auto test_param2 = 6; // Number of parameters of two gaussians
		cout << "test_chi2_1: " << test_chi2_1 << endl;
		cout << "test_chi2_2: " << test_chi2_2 << endl;
		cout << "test_ndf_1: " << test_ndf_1 << endl;
		cout << "test_ndf_2: " << test_ndf_2 << endl;



		assert(test_ndf_1 + 3 == test_ndf_2 +6 ); //TODO fails... check what to use



//		auto test_f = ( double(test_chi2_1 - test_chi2_2) / double(test_param2 - test_param1) ) /
//				      (test_chi2_2 / double(test_ndf_2 - test_param2) );
		auto test_f = ( double(test_chi2_1 - test_chi2_2) / double(test_ndf_1 - test_ndf_2) ) /
				      (test_chi2_2 / double(test_ndf_2) );

		DBG( std::cout << "[LOG] test_f: " << test_f << std::endl; , ; )
		//TODO F test!


		// Now select if we fit with one or two gaussians
		//auto& fVRAM =  test_chi2_norm < chi2_threshold ? fVRAMsing : fVRAMdoub;
		//auto nfound = test_chi2_norm < chi2_threshold ? 1 : 2;

		auto& fVRAM =  test_f > chi2_threshold ? fVRAMsing : fVRAMdoub;
		auto nfound = test_f > chi2_threshold ? 1 : 2;
		if (nfound != 1 and nfound != 2) {

			cerr << "[ERROR]: Found " << nfound << " peaks "
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
		}

			// update last_nfound
			last_nfound = nfound;


		//Setting initial parameters for the first peak
		{
			auto expectedsigma = sigmaVR;
			// This is the VR or AM one, starting
			fVRAM->SetParameter( 0, hTRANS->GetMaximum() / 2.0 );
			fVRAM->SetParameter( 1, meanVR  );
//			fVRAM->SetParLimits( 1, meanVR - 4, meanVR + 4  );
			fVRAM->SetParameter( 2, expectedsigma);

			if (nfound == 2) {
				// Since I order the peaks, when there are two peaks this is the AM one,
				// on the right (bigger X coordinate, near zero)
				fVRAM->SetParameter( 3, hTRANS->GetMaximum() / 2.0 );
				fVRAM->SetParameter( 4, meanAM );
//				fVRAM->SetParLimits( 4, meanAM - 4, meanAM + 4  );
				fVRAM->SetParameter( 5, expectedsigma);
			}
		}



		TCanvas* c_fitVRAM = new TCanvas("c_fitVRAM","c_fitVRAM");
		c_fitVRAM->cd();

		//hTRANS->Smooth();



		//Total fit
		TFitResultPtr fit_hTRANS = hTRANS->Fit( fVRAM, "IREM+" );
		hSmooth->Draw("SAME");
		hTRANS->Draw("SAME");

		string nomehisto = hTRANS->GetName();
		auto nomefilepng = "Varie/Video_" +nome_cristallo+ "_" +exp_or_sim+ "/" + nomehisto + ".png";
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
//	auto media_VR = mPeaks.at( start_analysis )[0].mean;
	auto media_VR = meanVR;
	DBG( std::clog << "qua ci sono: [WARNING] " << std::endl
	; , ; )
//	auto media_AM = mPeaks.at( start_analysis + analysis_width - deltaslice )[0].mean; //Remember these are maps not vectors
	auto media_AM = meanAM;
	DBG( std::clog << "qua ci sono: [WARNING] " << std::endl
	; , ; )
	for (auto i = start_analysis; i < start_analysis + analysis_width; i = i + deltaslice) {
		//Double_t i = start_analysis + j;
		DBG( std::clog << "[LOG] i: " << i << std::endl
		; , ; )
		auto& peaks = mPeaks.at( i ); // Get a reference to the vector with one or two peaks of this slice

		if (i < regVRAM) {
			// VR
			// Should be only one peak here
			//assert( peaks.size() == 1 );
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
			// assert( peaks.size() == 1 );

			mAM.emplace( i, peaks[0] );
		}
	}
	DBG( std::clog << "qua ci sono: [WARNING] " << std::endl
	; , ; )

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

