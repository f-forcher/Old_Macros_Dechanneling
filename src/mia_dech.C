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
#include <cmath>
#include <map>
#include <string>
#include <memory>

#include "dbg_macro.h"
#include "func_sim.h"
#include "dech.h"
#include "DatiChanneling.h"
#include "my_typedefs.h"
#include "read_histograms.h"


//extern TDirectory* ROOT_PROJDIR;
//extern 	std::vector<TH1*> vHistograms;
//extern 	std::vector<TH1*> vHistograms10;
//extern std::vector<TCanvas*> vCanvases;
extern char PROJECT_DIR[FILENAME_MAX];
// extern bool PREFER_DAT_FILES; //Transferred in read_histograms
extern bool SMOOTHED_EXPO;
//extern std::vector<std::string> lista_crist_sia_root_sia_dat;

namespace mions {

//TODO serve? Non penso
Double_t myfunction(Double_t *x, Double_t *par) {
	// Float_t xx = x[0];

	Double_t xx = x[0];
	Double_t f = par[0] * exp( -xx / par[1] );
	return f;

}

/*
 * @brief: calculate the crystal parameters (like the dechanneling_lenght) from the fitted data
 * @thickness: z [mm], thickness of the crystal
 * @bending_angle: [microrad]
 */
std::unique_ptr<std::map<std::string, Double_t> > calculate_cystal_params_from_fit(Double_t thickness,
		Double_t bending_angle, Double_t bending_angle_err, Double_t slopeDc, Double_t slopeDc_err) {

	using std::map;
	using std::pow;
	using std::string;
	using std::unique_ptr;

	unique_ptr<map<string, Double_t> > mapc = std::make_unique<map<string, Double_t> >();
	mapc->emplace( "thickness", thickness );
	mapc->emplace( "bending_angle", bending_angle );
	mapc->emplace( "bending_angle_err", bending_angle_err );

	//Errors calculated with error propagation
	// Rc [m] = z / thetaBend
	auto Rc = (thickness * MILLI_) / (bending_angle * MICRO_);
	// Error propagation: sigmaRc = (thickness / bending_angle^2) * sigmaBendingAngle
	auto Rc_err = (thickness * MILLI_) / pow( bending_angle * MICRO_, 2 ) * bending_angle_err * MICRO_;
	// e^(s*theta) = e^(-theta/thetad) --> thetad = -1/s
	// Angular dechanneling characteristic angle ThetaD [microrad]
	auto thetad = -1.0 / slopeDc;
	// Error prop on theta = 1/s
	auto thetad_err = slopeDc_err / pow( slopeDc, 2 );
	// Ld [m] = thetad * Rc
	auto dechanneling_lenght = (thetad * MICRO_) * Rc;
	// Error prop: sqrt( (Rc*sigmaThetaD)^2 + (ThetaD*sigmaRc)^2 )
	auto dechanneling_lenght_err = sqrt( pow( Rc * thetad_err * MICRO_, 2 ) + pow( thetad * MICRO_ * Rc_err, 2 ) );

	mapc->emplace( "Rc", Rc );
	mapc->emplace( "Rc_err", Rc_err );
	mapc->emplace( "ThetaD", thetad );
	mapc->emplace( "ThetaD_err", thetad_err );
	mapc->emplace( "slopeDc", slopeDc );
	mapc->emplace( "slopeDc_err", slopeDc_err );

	mapc->emplace( "dechanneling_lenght", dechanneling_lenght );
	mapc->emplace( "dechanneling_lenght_err", dechanneling_lenght_err );

	return mapc;
}

/*
 * Macro to calculate dechanneling lenght for a crystal.
 * @nome_cristallo: Name of the folder with the crystal data
 * @output_dech: file in which to append the "Ldech"s, in the format
 *               Crystal | dechanneling L at +-5 microrad [m] | dechanneling L at +-10 microrad [m]
 */
void mia_dech(std::string nome_cristallo, std::shared_ptr<std::ofstream> output_analisi_dech,
		std::shared_ptr<TFile> root_output, const mions::CrystalDataTable& dati_cristalli_orig,
		mions::CrystalDataTable510& dati_cristalli_calcolati,
		mions::CrystalDataTable510& dati_cristalli_calcolati_ftot) {

	//PROJDIR->cd();
	// Go to the project's "home" folder
	chdir( PROJECT_DIR );

	using std::string;
	using std::ifstream;
	using std::stringstream;
	using std::pow;

	using mions::read_histograms;

	//gStyle->SetPalette(1);
	//gStyle->SetOptStat(0);
	//gStyle->SetOptTitle(0);
	//Gaxis::SetMaxDigits(3);

	string cartella_cristallo = "ForFrancesco/" + nome_cristallo + "_exp/";
	gSystem->ChangeDirectory( cartella_cristallo.c_str() );
//	string pathfiledati_dat = //cartella_cristallo +
//			"recoDataSimple_" + nome_cristallo + ".torsion.correction.histo.dat";

	string nomefiledati_dat = "recoDataSimple_" + nome_cristallo + ".torsion.correction.histo.dat";

//	std::string pathfiledati_root = cartella_cristallo + "recoDataSimple_renamed.torsion.correction.histo.root";

	std::string nomefiledati_root = "recoDataSimple_renamed.torsion.correction.histo.root";

	//ifstream file_dat( pathfiledati_dat );

	//600 bin ,da -200 a 400
	// Scelti guardando i grafici fatti da dech.C

	// select +- 5 microrad in nomehisto5, +-10 in nomehisto10
//	string nomehisto5 = "hdati5_" + nome_cristallo;
//	string nomehisto10 = "hdati10_" + nome_cristallo;
//	string titlehisto5 = nome_cristallo + ", cuts at +- 5 microrad";
//	string titlehisto10 = nome_cristallo + ", cuts at +- 10 microrad";
//	clog << nomehisto5 << endl;

	// vHistograms.push_back( new TH1D(
	// /* name */nomehisto.c_str(),
	// /* title */nome_cristallo.c_str(),
	// /* X-dimension */600, -200, 400));

	//	auto histogram5_dat = std::make_unique < TH1D > (
	//	/* name */nomehisto5.c_str(),
	//	/* title */titlehisto5.c_str(),
	//	/* X-dimension */600/4, -200, 400);

	TH1D* histogram5 = nullptr;
	TH1D* histogram10 = nullptr;

	//vHistograms.front()->SetNameTitle(nomehisto5.c_str(),nome_cristallo.c_str());

	/*
	 * If .dat exist and is preferred, use it
	 * if .dat does not exist and is preferred, use .root and warn about it
	 * if .dat exist but is not preferred, use .root
	 * if .dat does not exist and is not prefferred... all's right with the world, use .root
	 */

	read_histograms(nome_cristallo, nomefiledati_dat, nomefiledati_root, histogram5, histogram10);
	//TODO Sostituire con una funzione il caricamento degli istogrammi, per poi spostarlo nell'altro programma
/*
	auto in_file_root = new TFile( nomefiledati_root.c_str() );
	if (bool( file_dat ) and (PREFER_DAT_FILES or not in_file_root->IsOpen())) {
		// Il codice per la mia analisi qua

		//gStyle->SetPalette(1);
		//gStyle->SetOptStat(1);
		//gStyle->SetOptTitle(1);
		//TGaxis::SetMaxDigits(3);

		DBG(
				clog << "[LOG]: " << "Crystal " << nome_cristallo << endl; clog << "[LOG]: File "<< nomefiledati_dat << endl << endl;,
				; )
		clog << "[LOG]: Using .dat file";

		//ifstream file_dat(pathfiledati);

		//Riempi gli istogrammi
		DatiChanneling dati( nomefiledati_dat );

		EventoDechanneling ev;
		auto datisize = dati.getSize();

		auto histogram5_dat = new TH1D(
		 name nomehisto5.c_str(),
		 title titlehisto5.c_str(),
		 X-dimension 600 / 4, -200, 400 );

		auto histogram10_dat = new TH1D(
		 name nomehisto10.c_str(),
		 title titlehisto10.c_str(),
		 X-dimension 600 / 4, -200, 400 );

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
			 name nomehisto5.c_str(),
			 title titlehisto5.c_str(),
			 X-dimension 800 / 4, -400, 400 );
			auto h10_qmp36_new = new TH1D(
			 name nomehisto10.c_str(),
			 title titlehisto10.c_str(),
			 X-dimension 800 / 4, -400, 400 );
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
*/

	// FITTING
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
	// expo:  exp([0]+[1]*x)
	//const Double_t mean_fchan_estim = dati_cristalli[nome_cristallo][(int)FieldCrystalDataTable::bending_angle];
	const Double_t mean_fchan_estim =
			dati_cristalli_orig.at( nome_cristallo )[(int) FieldCrystalDataTable::bending_angle];

	DBG( clog << "mean_dech_estim:" << mean_fchan_estim << endl
	; , ; )

	// First peak
	TF1* fAmor = new TF1( "fAmor", "gaus", -30, 10 );

	// Second peak
	TF1* fChan = new TF1( "fChan", "gaus", mean_fchan_estim - 20, mean_fchan_estim + 20 );

	// Clearer syntax than f->SetParameters(&param[0]); ...
	fAmor->SetParNames( "ConstAm", "MeanAm", "SigmaAm" );

	fChan->SetParNames( "ConstCh", "MeanCh", "SigmaCh" );

	// Starting numbers guessed by looking at the graphs
	fAmor->SetParameter( "ConstAm", histogram5->GetEntries() / 2.0 );
	fAmor->SetParameter( "MeanAm", -5 );
	fAmor->SetParameter( "SigmaAm", 8 );

	// Starting numbers guessed by looking at the graphs
	fChan->SetParameter( "ConstCh", histogram5->GetEntries() / 2.0 );
	fChan->SetParameter( "MeanCh", mean_fchan_estim );
	fChan->SetParameter( "SigmaCh", 6.5 );

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
	histogram5->Fit( fAmor, "I0REM+" );
	histogram10->Fit( fAmor, "I0REM+" );

	histogram5->Fit( fChan, "I0REM+" );
	histogram10->Fit( fChan, "I0REM+" );

	TF1 *fitResultAm5 = histogram5->GetFunction( "fAmor" );
	TF1 *fitResultCh5 = histogram5->GetFunction( "fChan" );

	TF1 *fitResultAm10 = histogram10->GetFunction( "fAmor" );
	TF1 *fitResultCh10 = histogram10->GetFunction( "fChan" );

	/*
	 *  Dechanneling exponential
	 */

	// Lets fit the exponential of the dechanneling starting from +3 sigma right of amorphous peak to -3 sigma left of channeling peak
	// GetFunction does not copy the parameters' name?
	// These variables have the same name as those above
	// gaus = [0]*exp( ((x-[1])/[2])^2 )
	constexpr int ConstAm = 0;
	constexpr int ConstCh = 0;

	constexpr int MeanAm = 1;
	constexpr int MeanCh = 1;

	constexpr int SigmaAm = 2;
	constexpr int SigmaCh = 2;

	//Extracting the fitted parameters
	auto constAm5 = fitResultAm5->GetParameter( ConstAm );
	auto constAm10 = fitResultAm10->GetParameter( ConstAm );
	auto constCh5 = fitResultCh5->GetParameter( ConstCh );
	auto constCh10 = fitResultCh10->GetParameter( ConstCh );

	auto meanAm5 = fitResultAm5->GetParameter( MeanAm );
	auto meanAm5_err = fitResultAm5->GetParError( MeanAm );
	auto sigmaAm5 = fitResultAm5->GetParameter( SigmaAm );

	DBG( clog << " meanAm5: " << meanAm5 << endl << " sigmaAm5: " << sigmaAm5 << endl
	;, ; )

	auto meanCh5 = fitResultCh5->GetParameter( MeanCh );
	auto meanCh5_err = fitResultCh5->GetParError( MeanCh );
	auto sigmaCh5 = fitResultCh5->GetParameter( SigmaCh );

	DBG( clog << " meanCh5: " << meanCh5 << endl << " sigmaCh5: " << sigmaCh5 << endl
	;, ; )

	auto meanAm10 = fitResultAm10->GetParameter( MeanAm );
	auto meanAm10_err = fitResultAm10->GetParError( MeanAm );
	auto sigmaAm10 = fitResultAm10->GetParameter( SigmaAm );

	auto meanCh10 = fitResultCh10->GetParameter( MeanCh );
	auto meanCh10_err = fitResultCh10->GetParError( MeanCh );
	auto sigmaCh10 = fitResultCh10->GetParameter( SigmaCh );

	// Dechanneling exponentials
	TF1* fDech5 = new TF1( "fDech5", "expo", meanAm5 + 3 * sigmaAm5, meanCh5 - 3 * sigmaCh5 );
	TF1* fDech10 = new TF1( "fDech10", "expo", meanAm10 + 3 * sigmaAm10, meanCh10 - 3 * sigmaCh10 );

	DBG( clog << "xmindech: " << meanAm5 + 3 * sigmaAm5 << " xmaxdech: " << meanCh5 - 3 * sigmaCh5 << endl
	;, ; )

	// expo:  exp([0]+[1]*x)
	fDech5->SetParNames( "ConstDc", "SlopeDc" );
	fDech10->SetParNames( "ConstDc", "SlopeDc" );

	//fDech->SetParameter("MeanAm",-5);

	histogram5->Fit( fDech5, "IREM+" );
	histogram10->Fit( fDech10, "IREM+" );

	TF1 *fitResultDech5 = histogram5->GetFunction( "fDech5" );
	TF1 *fitResultDech10 = histogram10->GetFunction( "fDech10" );

	//expo:  exp([0]+[1]*x)
	constexpr int ConstDc = 0;
	constexpr int SlopeDc = 1;

	//Extracting the fitted parameters
	auto constDc5 = fitResultDech5->GetParameter( ConstDc );
	auto constDc10 = fitResultDech10->GetParameter( ConstDc );

	auto slopeDc5 = fitResultDech5->GetParameter( SlopeDc );
	auto slopeDc5_err = fitResultDech5->GetParError( SlopeDc );
	auto slopeDc10 = fitResultDech10->GetParameter( SlopeDc );
	auto slopeDc10_err = fitResultDech10->GetParError( SlopeDc );

	DBG( std::clog << "slopeDc5: " << slopeDc5 << " +- " << slopeDc5_err << std::endl
	; , ; )
	DBG( std::clog << "slopeDc10: " << slopeDc10 << " +- " << slopeDc10_err << std::endl
	; , ; )

	//Weighted means

	//auto final_bending_angle = ( meanCh5*pow(meanCh5_err,-2) + meanCh10*pow(meanCh10_err,-2) ) /
	//	  	  	  	  	   ( pow(meanCh5_err,-2) + pow(meanCh10_err,-2) );

	//Thickness [mm]
	auto thickness = dati_cristalli_orig.at( nome_cristallo )[(int) FieldCrystalDataTable::thickness];
	DBG( std::clog << "thickness: " << thickness << std::endl
	; , ; )

	// bending angle [microrad]
	auto bending_angle5 = meanCh5;
	auto bending_angle5_err = meanCh5_err;
	auto bending_angle10 = meanCh10;
	auto bending_angle10_err = meanCh10_err;

	/*
	 // Rc [m] = z / thetaBend
	 auto Rc5 = (thickness * MILLI_) / (bending_angle5 * MICRO_);
	 // Error propagation: sigmaRc = (thickness / bending_angle^2) * sigmaBendingAngle
	 auto Rc5_err = (thickness * MILLI_) / pow(bending_angle5 * MICRO_, 2) * bending_angle5_err * MICRO_;
	 auto Rc10 = (thickness * MILLI_) / (bending_angle10 * MICRO_);
	 auto Rc10_err = (thickness * MILLI_) / pow(bending_angle10 * MICRO_, 2) * bending_angle10_err * MICRO_;

	 // e^(s*theta) = e^(-theta/thetad) --> thetad = -1/s
	 // Angular dechanneling characteristic angle ThetaD [microrad]
	 auto thetad5 = -1.0 / slopeDc5;
	 // Error prop on theta = 1/s
	 auto thetad5_err = slopeDc5_err / pow(slopeDc5, 2);
	 auto thetad10 = -1.0 / slopeDc10;
	 auto thetad10_err = slopeDc10_err / pow(slopeDc10, 2);


	 // Ld [m] = thetad * Rc
	 auto dechanneling_lenght5 = (thetad5 * MICRO_) * Rc5;
	 // Error prop: sqrt( (Rc*sigmaThetaD)^2 + (ThetaD*sigmaRc)^2 )
	 auto dechanneling_lenght5_err = sqrt( pow(Rc5 * thetad5_err * MICRO_,2) +
	 pow(thetad5 * MICRO_ * Rc5_err,2) );
	 auto dechanneling_lenght10 = (thetad10 * MICRO_) * Rc10;
	 auto dechanneling_lenght10_err = sqrt( pow(Rc10 * thetad10_err * MICRO_,2) +
	 pow(thetad10 * MICRO_ * Rc10_err,2) );
	 */

	auto map5 = *calculate_cystal_params_from_fit( thickness, bending_angle5, bending_angle5_err, slopeDc5,
			slopeDc5_err );
	auto map10 = *calculate_cystal_params_from_fit( thickness, bending_angle10, bending_angle10_err, slopeDc10,
			slopeDc10_err );

	DBG( std::clog << "dechanneling_lenght5: " << map5["dechanneling_lenght"] << std::endl
	; , ; )
	DBG( std::clog << "dechanneling_lenght5_err: " << map5["dechanneling_lenght_err"] << std::endl
	; , ; )
	DBG( std::clog << "dechanneling_lenght10: " << map10["dechanneling_lenght"] << std::endl
	; , ; )
	DBG( std::clog << "dechanneling_lenght10_err: " << map10["dechanneling_lenght_err"] << std::endl
	; , ; )

	//Weighted mean for the bending angle
	//Create key with name nome_cristallo
	auto& crystal_calc_ref = dati_cristalli_calcolati[nome_cristallo];

	auto particle_type_number = dati_cristalli_orig.at( nome_cristallo )[(int) FieldCrystalDataTable::particle_type_number];
	auto particle_energy = dati_cristalli_orig.at( nome_cristallo )[(int) FieldCrystalDataTable::particle_energy];

	using FCDT = FieldCrystalDataTable510;
	crystal_calc_ref[(int) FCDT::bending_angle5] = bending_angle5;
	crystal_calc_ref[(int) FCDT::bending_angle5_err] = bending_angle5_err;
	crystal_calc_ref[(int) FCDT::bending_angle10] = bending_angle10;
	crystal_calc_ref[(int) FCDT::bending_angle10_err] = bending_angle10_err;
	crystal_calc_ref[(int) FCDT::raggio_curvatura5] = map5["Rc"];
	crystal_calc_ref[(int) FCDT::raggio_curvatura5_err] = map5["Rc_err"];
	crystal_calc_ref[(int) FCDT::raggio_curvatura10] = map10["Rc"];
	crystal_calc_ref[(int) FCDT::raggio_curvatura10_err] = map10["Rc_err"];
	crystal_calc_ref[(int) FCDT::thickness] = thickness;
	crystal_calc_ref[(int) FCDT::slopeDc5] = map5["slopeDc"];
	crystal_calc_ref[(int) FCDT::slopeDc5_err] = map5["slopeDc_err"];
	crystal_calc_ref[(int) FCDT::slopeDc10] = map10["slopeDc"];
	crystal_calc_ref[(int) FCDT::slopeDc10_err] = map10["slopeDc_err"];
	crystal_calc_ref[(int) FCDT::dechanneling_lenght5] = map5["dechanneling_lenght"];
	crystal_calc_ref[(int) FCDT::dechanneling_lenght5_err] = map5["dechanneling_lenght_err"];
	crystal_calc_ref[(int) FCDT::dechanneling_lenght10] = map10["dechanneling_lenght"];
	crystal_calc_ref[(int) FCDT::dechanneling_lenght10_err] = map10["dechanneling_lenght_err"];
	crystal_calc_ref[(int) FCDT::particle_type_number] = particle_type_number;
	crystal_calc_ref[(int) FCDT::particle_energy] = particle_energy;

	/*
	 * Test con somma
	 */
	cout << endl << endl << "Prova fit totale con fTot" << endl;
	// Expanded range

	//TF1* fDech5_2  = new TF1("fDech5_2", "expo(3)", meanAm5, meanCh5);
	/*
	 * Gaussian blurred exponential
	 * (E^(d + (c^2 sigma^2)/4 + c y) (Erf[(-2 a + c sigma^2 + 2 y)/(2 sigma)] +
	 Erf[b/sigma - (c sigma)/2 - y/sigma]))/(2 Sqrt[2])
	 */
	TF1* fTot5 = new TF1( "fTot5", "gaus(0) + "
			"expo(3) * ( x > [5] && x < [6] ) "
			"+ gaus(7)", meanAm5 - 4 * sigmaAm5, meanCh5 + 4 * sigmaCh5 );

	/* Original expression: exponential on an interval (like above) smoothed by a (normalized)
	 * gaussian (with mu=0):
	 * F(x) = (expo*gaus)(x) =
	 * (exp(d + (c*c * sigma*sigma)/4.0 + c*x)*(TMath::Erf((-2*a + c*sigma*sigma + 2*x)/(2*sigma)) +
								 TMath::Erf(b/sigma - (c sigma)/2 - x/sigma)))/(2 Sqrt[2])

	 d = [3] for similarity with the simpler exponential, exp([3]+[4]x) | [5]<x<[6]
	 c = [4]  "
	 a = [5]  "
	 b = [6]  "
	 sigma = [7] gaussian sigma, smoothing degree
	 */
	TF1* fTot5_2 = new TF1("fTot5_smooth", "gaus(0) + "
								 "(exp([3] + ([4]*[4] * [7]*[7])/4.0 + [4]*x)*(TMath::Erf((-2*[5] + [4]*[7]*[7] + 2*x)/(2*[7])) +"
								 " TMath::Erf([6]/[7] - ([4]*[7])/2.0 - x/[7])))/(2 * sqrt(2))"
								 "+ gaus(8)", meanAm5 - 4*sigmaAm5, meanCh5 + 4*sigmaCh5);

	const Double_t param5[10] = {
	//Amorphous peak
			constAm5,          // 0
			meanAm5,           // 1
			sigmaAm5,          // 2
			//Dechanneling
			constDc5,          // 3
			slopeDc5,          // 4
			meanAm5 + 3*sigmaAm5,  // 5
			meanCh5 - 3*sigmaCh5,  // 6
			//Channeling peak
			constCh5,          // 7
			meanCh5,           // 8
			sigmaCh5           // 9
			};

	const Double_t param5_smooth[11] = {
	//Amorphous peak
			constAm5,          // 0
			meanAm5,           // 1
			sigmaAm5,          // 2
			//Dechanneling
			constDc5,          // 3
			slopeDc5,          // 4
			meanAm5 + 3*sigmaAm5,  // 5
			meanCh5 - 3*sigmaCh5,  // 6
			sigmaAm5,		// 7
			//Channeling peak
			constCh5,          // 8
			meanCh5,           // 9
			sigmaCh5           // 10
			};

	fTot5->SetParameters( param5 );
	fTot5_2->SetParameters( param5_smooth );

	fTot5->FixParameter( 5, meanAm5 + 2.5 * sigmaAm5 );
	fTot5->FixParameter( 6, meanCh5 - 2.5 * sigmaCh5 );

	//fTot5_2->FixParameter( 5, meanAm5 + 2.5 * sigmaAm5 );
	//fTot5_2->FixParameter( 6, meanCh5 - 2.5 * sigmaCh5 );
	//fTot5_2->FixParameter( 7, sigmaAm5 );

	fTot5_2->SetParLimits(5,meanAm5 + 1.6 * sigmaAm5,meanAm5 + 3.1 * sigmaAm5);
	fTot5_2->SetParLimits(6,meanCh5 - 1.6 * sigmaCh5,meanCh5 - 3.1 * sigmaCh5);
	fTot5_2->SetParLimits(7,0.1*sigmaAm5,2*sigmaAm5);

	auto f5_to_fit = SMOOTHED_EXPO ? "fTot5_smooth" : "fTot5";
	histogram5->Fit( f5_to_fit, "ILREM+" );

	constexpr int SlopeDc_tot = 4;
	TF1 *fitResultTot5 = histogram5->GetFunction( f5_to_fit );
	auto slopeDc_tot5 = fitResultTot5->GetParameter( SlopeDc_tot );
	auto slopeDc_tot5_err = fitResultTot5->GetParError( SlopeDc_tot );

	DBG( std::clog << "slopeDc_tot5: " << slopeDc_tot5 << " +- " << slopeDc_tot5_err << std::endl
	; , ; )

	//TF1* fDech10_2  = new TF1("fDech10_2", "expo(3)", meanAm10+sigmaAm10, meanCh10-sigmaCh10);
	TF1* fTot10 = new TF1( "fTot10", "gaus(0) + expo(3) * ( x > [5] && x < [6] ) + gaus(7)", meanAm10 - 4 * sigmaAm10,
			meanCh10 + 4 * sigmaCh10 );
//	TF1* fTot10_2 = new TF1("fTot10_smooth", "gaus(0) + "
//								 "(exp([3] + ([4]*[4] * [7]*[7])/4.0 + [4]*x)*(TMath::Erf((-2*[5] + [4]*[7]*[7] + 2*x)/(2*[7])) +"
//								 " TMath::Erf([6]/[7] - ([4]*[7])/2.0 - x/[7])))/(2 * sqrt(2))"
//								 "+ gaus(8)", meanAm10 - 4 * sigmaAm10, meanCh10 + 4 * sigmaCh10);

	TF1* fTot10_2 = new TF1("fTot10_smooth", "gaus(0) + "
								 "(exp([3] + ([4]*[4] * [7]*[7])/4.0 + [4]*x)*(erf((-2*[5] + [4]*[7]*[7] + 2*x)/(2*[7])) +"
								 " erf([6]/[7] - ([4]*[7])/2.0 - x/[7])))/(2 * sqrt(2))"
								 "+ gaus(8)", meanAm10 - 4 * sigmaAm10, meanCh10 + 4 * sigmaCh10);

	const Double_t param10[10] = {
			//Amorphous peak
			constAm10,          // 0
			meanAm10,           // 1
			sigmaAm10,          // 2
			//Dechanneling
			constDc10,          // 3
			slopeDc10,          // 4
			meanAm10 + 3*sigmaAm10, // 5
			meanCh10 - 3*sigmaCh10, // 6
			//Channeling peak
			constCh10,          // 7
			meanCh10,           // 8
			sigmaCh10           // 9
			};
	const Double_t param10_smooth[11] = {
			//Amorphous peak
			constAm10,          // 0
			meanAm10,           // 1
			sigmaAm10,          // 2
			//Dechanneling
			constDc10,          // 3
			slopeDc10,          // 4
			meanAm10 + 3*sigmaAm10, // 5
			meanCh10 - 3*sigmaCh10, // 6
			sigmaAm10,		// 7
			//Channeling peak
			constCh10,          // 8
			meanCh10,           // 9
			sigmaCh10           // 10
			};

	fTot10->SetParameters( param10 );
	fTot10_2->SetParameters( param10_smooth );

	fTot10->FixParameter( 5, meanAm10 + 2.5 * sigmaAm10 );
	fTot10->FixParameter( 6, meanCh10 - 2.5 * sigmaCh10 );

	//fTot10_2->FixParameter( 5, meanAm10 + 2.5 * sigmaAm10 );
	//fTot10_2->FixParameter( 6, meanCh10 - 2.5 * sigmaCh10 );
//	fTot10_2->FixParameter( 7, sigmaAm10 / 3.0 );

	fTot10_2->SetParLimits(5,meanAm10 + 1.6 * sigmaAm10,meanAm10 + 3.1 * sigmaAm10);
	fTot10_2->SetParLimits(6,meanCh10 - 1.6 * sigmaCh10,meanCh10 - 3.1 * sigmaCh10);
	fTot10_2->SetParLimits(7,0.1*sigmaAm10,2*sigmaAm10);

	auto f10_to_fit = SMOOTHED_EXPO ? "fTot10_smooth" : "fTot10";
	histogram10->Fit( f10_to_fit, "IREM+" );

	TF1 *fitResultTot10 = histogram10->GetFunction( f10_to_fit );
	auto slopeDc_tot10 = fitResultTot10->GetParameter( SlopeDc_tot );
	auto slopeDc_tot10_err = fitResultTot10->GetParError( SlopeDc_tot );

	DBG( std::clog << "slopeDc_tot10: " << slopeDc_tot10 << " +- " << slopeDc_tot10_err << std::endl
	; , ; )

	auto npar_meanCh = 9;
	auto bending_angle5_tot = fitResultTot5->GetParameter( npar_meanCh );
	auto bending_angle5_tot_err = fitResultTot5->GetParError( npar_meanCh );
	auto bending_angle10_tot = fitResultTot10->GetParameter( npar_meanCh );
	auto bending_angle10_tot_err = fitResultTot10->GetParError( npar_meanCh );

	auto map5_tot = *calculate_cystal_params_from_fit( thickness, bending_angle5_tot, bending_angle5_tot_err,
			slopeDc_tot5, slopeDc_tot5_err );
	auto map10_tot = *calculate_cystal_params_from_fit( thickness, bending_angle10_tot, bending_angle10_tot_err,
			slopeDc_tot10, slopeDc_tot10_err );

	DBG( std::clog << "dechanneling_lenght5 tot: " << map5_tot["dechanneling_lenght"] << std::endl
	; , ; )
	DBG( std::clog << "dechanneling_lenght5_err tot: " << map5_tot["dechanneling_lenght_err"] << std::endl
	; , ; )
	DBG( std::clog << "dechanneling_lenght10 tot: " << map10_tot["dechanneling_lenght"] << std::endl
	; , ; )
	DBG( std::clog << "dechanneling_lenght10_err tot: " << map10_tot["dechanneling_lenght_err"] << std::endl
	; , ; )

	//Weighted mean for the bending angle
	//Create key with name nome_cristallo
	auto& crystal_calctot_ref = dati_cristalli_calcolati_ftot[nome_cristallo];

	using FCDT = FieldCrystalDataTable510;
	crystal_calctot_ref[(int) FCDT::bending_angle5] = bending_angle5_tot;
	crystal_calctot_ref[(int) FCDT::bending_angle5_err] = bending_angle5_tot_err;
	crystal_calctot_ref[(int) FCDT::bending_angle10] = bending_angle10_tot;
	crystal_calctot_ref[(int) FCDT::bending_angle10_err] = bending_angle10_tot_err;
	crystal_calctot_ref[(int) FCDT::raggio_curvatura5] = map5_tot["Rc"];
	crystal_calctot_ref[(int) FCDT::raggio_curvatura5_err] = map5_tot["Rc_err"];
	crystal_calctot_ref[(int) FCDT::raggio_curvatura10] = map10_tot["Rc"];
	crystal_calctot_ref[(int) FCDT::raggio_curvatura10_err] = map10_tot["Rc_err"];
	crystal_calctot_ref[(int) FCDT::thickness] = thickness;
	crystal_calctot_ref[(int) FCDT::slopeDc5] = map5_tot["slopeDc"];
	crystal_calctot_ref[(int) FCDT::slopeDc5_err] = map5_tot["slopeDc_err"];
	crystal_calctot_ref[(int) FCDT::slopeDc10] = map10_tot["slopeDc"];
	crystal_calctot_ref[(int) FCDT::slopeDc10_err] = map10_tot["slopeDc_err"];
	crystal_calctot_ref[(int) FCDT::dechanneling_lenght5] = map5_tot["dechanneling_lenght"];
	crystal_calctot_ref[(int) FCDT::dechanneling_lenght5_err] = map5_tot["dechanneling_lenght_err"];
	crystal_calctot_ref[(int) FCDT::dechanneling_lenght10] = map10_tot["dechanneling_lenght"];
	crystal_calctot_ref[(int) FCDT::dechanneling_lenght10_err] = map10_tot["dechanneling_lenght_err"];
	crystal_calctot_ref[(int) FCDT::particle_type_number] = particle_type_number;
	crystal_calctot_ref[(int) FCDT::particle_energy] = particle_energy;

	//TCanvas* mio_c1 = new TCanvas();
	//vCanvases.push_back(new TCanvas(nome_cristallo.c_str()));
	//vHistograms.front()->Draw();
	//histogram5->Draw();
	root_output->cd();
	//root_output->Write();
	histogram5->Write();
	histogram10->Write();

	/*	} else {
	 DBG(
	 clog << nome_cristallo << ": File .dat not found" << endl; clog << "Nome cercato: " << pathfiledati << endl; clog << "Current Dir: " << system("pwd") << endl;
	 , ; )
	 clog << "[WARNING] For crystal " << nome_cristallo
	 << ", File .dat not found" << endl
	 << "          Trying to use old dech.C macro and .root data file"
	 << endl << endl;

	 //ROOT_PROJDIR->cd();
	 //rimetterla; no non serve piu' faccio qua l'analisi ROOT
	 //dech(nome_cristallo,output_analisi_dech,dati_cristalli_orig,dati_cristalli_calcolati,dati_cristalli_calcolati_ftot);
	 }*/

}

}
