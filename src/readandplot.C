/*
 * readandplot.C
 *
 *  Created on: Nov 16, 2016
 *      Author: fforcher
 */

#include <TH1.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>



#include <string>
#include <vector>
#include <fstream>
#include <iostream>

int readandplot(std::string file_in, std::string file_out) {
	using namespace std;

	vector<Double_t> Rc10;
	vector<Double_t> Rc10_err;
	vector<Double_t> Ld10;
	vector<Double_t> Ld10_err;

	ifstream file_10_cuts_dat("./10_cuts_dat");

	Double_t tRc10;
	Double_t tRc10_err;
	Double_t tLd10;
	Double_t tLd10_err;

	while (file_10_cuts_dat >> tRc10) {
		file_10_cuts_dat >> tLd10;
		file_10_cuts_dat >> tRc10_err;
		file_10_cuts_dat >> tLd10_err;

		Rc10.push_back(tRc10);
		Ld10.push_back(tLd10);
		Rc10_err.push_back(tRc10_err);
		Ld10_err.push_back(tLd10_err);
	}


	TCanvas* cRcLd = new TCanvas( "10cuts", "10cuts plot" );

	unsigned long len = Rc10.size();
	TGraphErrors* RcLd_10 = new TGraphErrors( len, &Rc10[0], &Ld10[0], &Rc10_err[0], &Ld10_err[0] );

	TF1 *fRcLd_10 = new TF1( "fRcLd_10", "pol1" );
	//TF1 *fRcLd_10 = new TF1( "fRcLd_5", "[0]*pow(1-[b]/x,2)" );
	TFitResultPtr fit_RcLd_10 = RcLd_10->Fit( fRcLd_10, "S" );
	cout << "Results for the +-10 cuts sharp fit" << endl;
	fit_RcLd_10->Print("V");

	clog << "k: " << fit_RcLd_10->Parameter(1) << endl;
	clog << "kerr: " << fit_RcLd_10->Error(1) << endl;
	cout << "1/k = " << 1.0/fit_RcLd_10->Parameter(1)
		 << " +- "<< fit_RcLd_10->Error(1)/pow(fit_RcLd_10->Parameter(1),2)
		 << endl;

	RcLd_10->SetTitle( "Ln/Le vs (1-Rc/R)^2 (cuts at 10)" );
	RcLd_10->GetXaxis()->SetTitle( "Rc [m]" );
	RcLd_10->GetYaxis()->SetTitle( "Ld [m]" );
	RcLd_10->Draw( "AP*" );

	TFile* ten_cuts_dat_rootfile = new TFile("10_cuts_dat.root", "RECREATE");

	RcLd_10->Write();
}
