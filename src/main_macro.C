/*
 * main_macro.C
 *
 *  Created on: Sep 21, 2016
 *      Author: fforcher
 */

//============================================================================
// Name        : bP_05.cpp
// Author      : Francesco Forcher
// Version     : 1.1
// Description : Modify Plot_v04 and use dispatcher&observers to handle begin/end job
//============================================================================
//#include "AnalysisFactory.h"
//#include "AnalysisInfo.h"
//#include "EventSource.h"
//#include "SourceFactory.h"
//#include "util/include/Dispatcher.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <cstdlib>
#include <unistd.h>
#include <map>
#include <sstream>
#include <array>
#include <cmath>

#include <TROOT.h>
#include <TH1.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>

#include "mia_dech.h"
//#include "analizza_dechanneling.h"
#include "dbg_macro.h"
#include "my_typedefs.h"
#include "dech.h"
#include "analisi_VRtoAM.h"
#include "analisi_VRtoAM_chi2.h"





// Per poter usare questa macro sia compilando che eseguendo.
// https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html#moving-between-interpreter-and-compiler
#ifndef __CLING__

#include "DatiChanneling.h"

#else
// Altrimenti includi il file della macro. Nel compilatore naturalmente direbbe multiple definition quando linka l'altro
// file.

#include "DatiChanneling.cpp"

#endif

//GLOBALS meglio in un file a parte
//Directory del progetto
TDirectory* ROOT_PROJDIR = nullptr;
char PROJECT_DIR[FILENAME_MAX] = "[NOT SET]";
std::vector<TH1*> vHistograms;
std::vector<TCanvas*> vCanvases;
bool PREFER_DAT_FILES = true; // If true
bool SMOOTHED_EXPO = false; // If true
std::vector<std::string> lista_crist_sia_root_sia_dat( 5 );

/*
 * First argument from command line
 */
int main_macro(int argc, char* argv[]) {

	using namespace std;
	using mions::FieldCrystalDataTable;
	using mions::CrystalDataTable;
	using mions::FieldCrystalDataTable510;
	using mions::CrystalDataTable510;
	using mions::electronic_dechanneling;
	using mions::analisi_VRtoAM;
	using mions::analisi_VRtoAM_chi2;
	using std::pow;


//	// No arguments:  argc=1, argv[0]=nome_comando
	// TODO WIP
	if (argc >= 2) {

		//First Setting: dat files vs root files
		if (string( argv[1] ) == string( "-dat" )){
			cout << "[SETTING]: Prefers to use .dat file if exist, else try with .root" << endl;
			PREFER_DAT_FILES = true;
		} else if (string( argv[1] ) == string( "-root" )) {
			cout << "[SETTING]: Prefers to use .root file if exist, else try with .dat" << endl;
			PREFER_DAT_FILES = false;
		} else {
			cout << "[ERROR]: either you use no options to use default, or you must indicate all of the following:" << endl;
			cout <<	"	1) first option should be either \"-dat\" or \"-root\", to select preferred file type" << endl;
			cout <<	"	2) second option should be either \"-smooth\" or \"-sharp\", to select preferred file type" << endl;
			return -1;
		}


		//Second setting: smoothed or sharp expo for dechanneling fitting
		if (string( argv[2] ) == string( "-smooth" )){
			cout << "[SETTING]: Use gausssian-smoothed interval expo for dechanneling fitting" << endl;
			SMOOTHED_EXPO = true;
		} else if ( string( argv[2] ) == string( "-sharp" )) {
			cout << "[SETTING]: Use sharp interval expo for dechanneling fitting" << endl;
			SMOOTHED_EXPO = false;
		} else {
			cout << "[ERROR]: either you use no options to use default, or you must indicate all of the following:" << endl;
			cout <<	"	1) first option should be either \"-dat\" or \"-root\", to select preferred file type" << endl;
			cout <<	"	2) second option should be either \"-smooth\" or \"-sharp\", to select preferred dechanneling fitting function" << endl;
			return -1;
		}

	} else if (argc < 2) {
		cout
				<< "[WARNING][SETTING]: No file preference indicated. Prefers to use .dat file if exist, else try with .root"
				<< endl;
		cout << "[WARNING]: use option \"-dat\" or \"-root\" (but without ticks) to select a preferred file type."
				<< endl;
		cout
				<< "[WARNING][SETTING]: No smoothed exponential preference indicated. Use default (sharp interval) exponential for the fitting"
				<< endl;
		cout << "[WARNING]: use second option \"-smooth\" or \"-sharp\", to select preferred fitting function for dechanneling"
				<< endl;
		PREFER_DAT_FILES = true;
		SMOOTHED_EXPO = false;
	}




//	if (argc >= 2 && string( argv[1] ) == string( "-dat" )) {
//		cout << "[SETTING]: Prefers to use .dat file if exist, else try with .root" << endl;
//		PREFER_DAT_FILES = true;
//	} else if (argc >= 2 && string( argv[1] ) == string( "-root" )) {
//		cout << "[SETTING]: Prefers to use .root file if exist, else try with .dat" << endl;
//		PREFER_DAT_FILES = false;
//	} else if (argc < 2) {
//		cout
//				<< "[WARNING][SETTING]: No file preference indicated. Prefers to use .dat file if exist, else try with .root"
//				<< endl;
//		cout << "[WARNING]: use option \"-dat\" or \"-root\" (but without ticks) to select a preferred file type."
//				<< endl;
//		cout
//				<< "[WARNING][SETTING]: No smoothed exponential preference indicated. Use default (sharp interval) exponential for the fitting"
//				<< endl;
//		cout << "[WARNING]: use second option \"-smooth\" or \"-sharp\", to select preferred file type"
//				<< endl;
//		PREFER_DAT_FILES = true;
//		SMOOTHED_EXPO = false;
//	} else {
//		cout << "[ERROR]: either you use no options to use default, or you must indicate all of the following:" << endl;
//		cout <<	"	1) first option should be either \"-dat\" or \"-root\", to select preferred file type" << endl;
//		cout <<	"	2) second option should be either \"-smooth\" or \"-sharp\", to select preferred fitting function for dechanneling" << endl;
//		return -1;
//	}



	// Save the directory of the project (remember you are expected to start it from the
	// top folder of the repo)
	getcwd( PROJECT_DIR, FILENAME_MAX );
	// store command line parameters

	{ // Start dechanneling scope here, TODO? transfer into function?

	int argc2 = 7;
	const char* argv2[] = { "./Debug/Ronch_braggPlot_v05", "input", "bragg_events.txt", "hist", "hh", "ranges",
			"energyRanges" };

	//Elenco dei cristalli
	std::vector<const char*> elenco_cristallistf { "STF38", "STF45", "STF47", "STF49", "STF51" };
	std::vector<const char*> elenco_cristalliqmp { "QMP27", "QMP29", "QMP32" };
	std::vector<const char*> elenco_cristalli { "STF38", "STF45", "STF47", "STF49", "STF51", "QMP27", "QMP29", "QMP32" };

	std::vector<const char*> elenco_cristalli_buoni_orig { "STF45", "STF38", "STF49", "QMP27", "QMP32" };
	std::vector<const char*> elenco_cristalli_buoni {
		"QMP25",
		"QMP27",
	//	"QMP29",
			"QMP32",
			"QMP36",
			//	"QMP38", // Low Efficiency
			"STF38",
			"STF45",
			//	"STF47",  //45 ThetaB, Troppo poco piegato
			"STF49",
			// "STF51",  //30 ThetaB Troppo poco piegato
			// "STF9",
			"STF99"
	};

	//std::vector<const char*> elenco_cristalli { "QMP32" };
	clog << "Start main_macro..." << endl;

	DBG( clog << "DEBUG VERSION " << endl
	; ,
	clog << "RELEASE VERSION " << endl; )

	// Corso root lunardon/garfagnini
	// carica la macro generica che legge il file di testo
	//gROOT->LoadMacro("ReadHistoFromTextFile.C");
	//gSystem->Load("ReadHistoFromTextFile_C.so"); // altra opzione per la precompilata

	/*
	 #if defined(__ROOTCLING__)
	 #pragma link C++ class MyOtherClass;
	 #endif
	 */
	//gROOT->LoadMacro("src/mia_dech.C");
	//gSystem->AddIncludePath(" -I./src/ ");
	//gROOT->ProcessLine(".L ./src/mia_dech.C+");
	//clog << "Fin qua" << endl;
	DBG( clog << "Current Project Dir: " << PROJECT_DIR << endl
	; , ; )

	/* Chiamare mia_dech su tutti i cristalli
	 * 1. Ottenere i file recoDataSimple_546_31-59.torsion.correction.histo.root
	 *
	 * 2. Scoprire dove sono le variabili
	 * 	//xtal length and curvature
	 Double_t z = 2e-3;
	 Double_t R;
	 * per gli altri cristalli (tabella in tesi di roberto?)
	 *
	 * 3. ???
	 * 4. Profit!
	 */

	cout << "LDe(1,400): " << electronic_dechanneling( 1, 400 ) << endl;

	//return 0;
	//Shared because every crystal has to use it. Maybe not the best solution...
	std::shared_ptr<std::ofstream> outputdechanneling(
			new std::ofstream( "./crystal_calc_table.txt", std::ofstream::out | std::ofstream::trunc ) );
	std::ofstream out_dech5( "./dech5_table.txt", std::ofstream::out | std::ofstream::trunc );
	std::ofstream out_dech10( "./dech10_table.txt", std::ofstream::out | std::ofstream::trunc );

	auto& outdatafile = *outputdechanneling;

	outdatafile << "# File generated by the macro mia_dech() for the dechanneling lenghts." << endl;
//	*outputdechanneling
//			<< "# Crystal | dechanneling L at +-5 microrad [m] | dechanneling L at +-10 microrad [m]"
//			<< endl;
	outdatafile << "#Crystal,"
			<< "raggio_curvatura5[m],"
			<< "raggio_curvatura5_err[m],"
			<< "raggio_curvatura10[m],"
			<< "raggio_curvatura10_err[m],"
			<< "bending_angle5[microrad],"
			<< "bending_angle5_err[microrad],"
			<< "bending_angle10[microrad],"
			<< "bending_angle10_err[microrad],"
			<< "thickness[mm],"
			<< "slopeDc5[1/microrad],"
			<< "slopeDc5_err[1/microrad],"
			<< "slopeDc10[1/microrad],"
			<< "slopeDc10_err[1/microrad],"
			<< "dechanneling_lenght5[m],"
			<< "dechanneling_lenght5_err[m],"
			<< "dechanneling_lenght10[m],"
			<< "dechanneling_lenght10_err[m]"
			<< endl;

	ROOT_PROJDIR = gDirectory;
	TDirectory* currentDir = gDirectory;

	std::string path_file_output_root = string( PROJECT_DIR ) + "/Dechanneling_Histograms.root";
	auto file_output_root = std::make_shared<TFile>( path_file_output_root.c_str(), "RECREATE" );

	//Read crystal data
	// File format: Crystal name | Rc [m]
	CrystalDataTable map_dati_crist_orig;

	{
		string nome_file_raggio_cristallo = string( PROJECT_DIR ) + "/tabella_dati_cristalli_corretti.txt";
		ifstream file_raggi_cristalli( nome_file_raggio_cristallo );

		string riga_estratta;
		stringstream ss;

		// Contenuto di una riga del file
		//Ignora la prima linea con i nomi delle colonne
		std::getline( file_raggi_cristalli, riga_estratta );

		while (file_raggi_cristalli) {
			string cristallo;
			Double_t raggio_curvatura;
			Double_t bending_angle;
			Double_t thickness;

			std::getline( file_raggi_cristalli, riga_estratta );
			//cout << riga_estratta << endl;
			ss << riga_estratta;

			ss >> cristallo;
			ss >> raggio_curvatura;
			ss >> bending_angle;
			ss >> thickness;

			if (cristallo != string( "" )) {
				DBG( clog << cristallo << ": Rc " << raggio_curvatura << " Thb " << bending_angle << endl; ;, ; )
				//map_dati_crist[cristallo][(int)FieldCrystalDataTable::raggio_curvatura] = raggio_curvatura;
				//map_dati_crist[cristallo][(int)FieldCrystalDataTable::bending_angle] = bending_angle;

				// The other elements will be initialized to zero
				map_dati_crist_orig[cristallo] = {raggio_curvatura, bending_angle, thickness};
			}

			ss.clear();
		}
	}

	std::string path_file_output_data = string( PROJECT_DIR ) + "/Calculated_Crystal_Data.txt";
	//auto file_output_data = std::make_shared<TFile>(
	//		path_file_output_root.c_str(), "RECREATE");

	//Here we store the results of the analysis (ie the fit parameters)
	mions::CrystalDataTable510 map_dati_crist_calc;
	mions::CrystalDataTable510 map_dati_crist_calc_tot;

	for (const auto& ch : elenco_cristalli_buoni) {
		cout << endl << endl;
		//dech(ch, outputdechanneling);
		cout << "Crystal: " << ch << " Rc[m]: "
				<< map_dati_crist_orig[ch][(int) FieldCrystalDataTable::raggio_curvatura] << " Thetab[muRad]: "
				<< map_dati_crist_orig[ch][(int) FieldCrystalDataTable::bending_angle];

		mions::mia_dech( ch, outputdechanneling, file_output_root, map_dati_crist_orig, map_dati_crist_calc,
				map_dati_crist_calc_tot );
		//currentDir->cd();

		outdatafile << ch << " ";
		for (auto d : map_dati_crist_calc[ch]) {
			outdatafile << d << " ";
		}

		outdatafile << endl;
		cout << endl << endl;
	}
	//file_output_root->Close();

	//dech("STF45", outputdechanneling);

	cout << "Analyzed all crystals in list" << endl;
	chdir( PROJECT_DIR );
	/*
	 * Plot the four TGraphErrors (Rc5 vs LD5, Rc5/LDe vs LD5, Rc10 vs LD10, Rc10/LDe ) * 2 (ie fTot vs fDech)
	 * LDe = Analytic extimation of the electronic dechanneling lenght (see Biryukov, Chesnokov), using 5/10 cuts data.
	 * LDe = 256/Pi^2 * p*v/ln(2*m_e*c^2*gamma/I) * a_tf/(Z_i * r_e * m_e * c^2)
	 * With
	 *   p: incoming particle momentumcrysdata
	 *   v: speed of incoming particle
	 *   m_e: electron mass
	 *   c: speed of light
	 *   gamma: relativistic gamma of incoming particle
	 *   I: Mean Excitation Energy (like in http://pdg.lbl.gov/2016/AtomicNuclearProperties/HTML/silicon_Si.html)
	 *   a_tf: Thomas-Fermi atomic radius
	 *   Z_i: electric charge of incoming particle
	 *   r_e: classical electron radius
	 */

	vector<Double_t> Rc5;
	vector<Double_t> Rc10;
	vector<Double_t> Rc5_err;
	vector<Double_t> Rc10_err;
	vector<Double_t> Ld5;
	vector<Double_t> Ld10;
	vector<Double_t> Ld5_err;
	vector<Double_t> Ld10_err;

	vector<Double_t> Rc5_tot;
	vector<Double_t> Rc10_tot;
	vector<Double_t> Rc5_tot_err;
	vector<Double_t> Rc10_tot_err;
	vector<Double_t> Ld5_tot;
	vector<Double_t> Ld10_tot;
	vector<Double_t> Ld5_tot_err;
	vector<Double_t> Ld10_tot_err;

	auto LDe = electronic_dechanneling( 1, 400 );
	const auto Rcrit = 1.0; // Critical radius at 400 GeV


	for (const auto& crys : elenco_cristalli_buoni) {
		const auto& crysdata_three_pieces = map_dati_crist_calc[crys];
		const auto& crysdata_tot = map_dati_crist_calc_tot[crys];
		const auto& crysdata = crysdata_three_pieces;

		using CDT = FieldCrystalDataTable510;

		// THeory: Lnuclear = Lelectronic/k, L
		// Sigma(1-Rc/R) = Rc*sigmaR / R^2
		Rc5.emplace_back( pow(1 - Rcrit / crysdata[(int) CDT::raggio_curvatura5] ,2) );
		Rc5_err.emplace_back( 2 * (crysdata[(int) CDT::raggio_curvatura5] - Rcrit)* Rcrit * crysdata[(int) CDT::raggio_curvatura5_err] /
							  pow(crysdata[(int) CDT::raggio_curvatura5], 3) );
		Ld5.emplace_back( crysdata[(int) CDT::dechanneling_lenght5] / LDe);
		Ld5_err.emplace_back( crysdata[(int) CDT::dechanneling_lenght5_err] / LDe );
		Rc10.emplace_back( pow(1 - Rcrit / crysdata[(int) CDT::raggio_curvatura10] ,2) );
		Rc10_err.emplace_back( 2 * (crysdata[(int) CDT::raggio_curvatura10] - Rcrit)* Rcrit * crysdata[(int) CDT::raggio_curvatura10_err] /
				  	  	  	  pow(crysdata[(int) CDT::raggio_curvatura10], 3) );
		Ld10.emplace_back( crysdata[(int) CDT::dechanneling_lenght10] / LDe );
		Ld10_err.emplace_back( crysdata[(int) CDT::dechanneling_lenght10_err] / LDe );

		//Total data
		Rc5_tot.emplace_back( pow(1 - Rcrit / crysdata_tot[(int) CDT::raggio_curvatura5] ,2) );
		Rc5_tot_err.emplace_back( 2 * (crysdata_tot[(int) CDT::raggio_curvatura5] - Rcrit)* Rcrit * crysdata_tot[(int) CDT::raggio_curvatura5_err] /
	  	  	  	  	  	  	  	  pow(crysdata_tot[(int) CDT::raggio_curvatura5], 3) );
		Ld5_tot.emplace_back( crysdata_tot[(int) CDT::dechanneling_lenght5] / LDe );
		Ld5_tot_err.emplace_back( crysdata_tot[(int) CDT::dechanneling_lenght5_err] / LDe );

		Rc10_tot.emplace_back( pow(1 - Rcrit / crysdata_tot[(int) CDT::raggio_curvatura10] ,2) );
		Rc10_tot_err.emplace_back( 2 * (crysdata_tot[(int) CDT::raggio_curvatura10] - Rcrit)* Rcrit * crysdata_tot[(int) CDT::raggio_curvatura10_err] /
	  	  	  	  	  	  	  	  	pow(crysdata_tot[(int) CDT::raggio_curvatura10], 3) );
		Ld10_tot.emplace_back( crysdata_tot[(int) CDT::dechanneling_lenght10]  / LDe );
		Ld10_tot_err.emplace_back( crysdata_tot[(int) CDT::dechanneling_lenght10_err] / LDe );

		cout << crys << " Le/Ln [cut 10]: " << LDe / crysdata[(int) CDT::dechanneling_lenght10] << endl;
//				" +- " << LDe / crysdata_tot[(int) CDT::dechanneling_lenght10_err] << endl;

		cout << crys << " Ln/Le [cut 10]: " << crysdata[(int) CDT::dechanneling_lenght10] / LDe // << endl;
						<< " +- " << crysdata[(int) CDT::dechanneling_lenght10_err] / LDe << endl;

		//cout << crys << " Le * slope [cut 10]: " << LDe * crysdata[(int) CDT::slopeDc10] << " +- "
		//		<< LDe * crysdata[(int) CDT::slopeDc10_err] << endl;
	}


	// Print out on a text file the "10 cuts" graph
	// Because ROOT 5 can't read ROOT 6 files, so we save the data in a text file and then a ROOT 5/6 macro can replot them
	ofstream file_10_cuts_dat("./10_cuts_dat");
	for (auto i = 0; i < Rc10.size(); ++i) {
		file_10_cuts_dat
		   << Rc10[i] << "    "
		   << Ld10[i] << "    "
		   << Rc10_err[i] << "    "
		   << Ld10_err[i] << endl;
	}




/*	for (const auto& crys : elenco_cristalli_buoni) {
		const auto& crysdata_three_pieces = map_dati_crist_calc[crys];
		const auto& crysdata_tot = map_dati_crist_calc_tot[crys];
		const auto& crysdata = crysdata_three_pieces;

		//TODO finire! mettere il raggio diretto e poi fittare con [0]*(1-[1]/x)^2
		// THeory: Lnuclear = Lelectronic/k, L
		// Sigma(1-Rc/R) = Rc*sigmaR / R^2
		Rc5.emplace_back( crysdata[(int) FieldCrystalDataTable510::raggio_curvatura5]);
		Rc5_err.emplace_back( crysdata[(int) FieldCrystalDataTable510::raggio_curvatura5_err] );
		Ld5.emplace_back( crysdata[(int) FieldCrystalDataTable510::dechanneling_lenght5] / LDe);
		Ld5_err.emplace_back( crysdata[(int) FieldCrystalDataTable510::dechanneling_lenght5_err] / LDe );
		Rc10.emplace_back( crysdata[(int) FieldCrystalDataTable510::raggio_curvatura10] );
		Rc10_err.emplace_back( crysdata[(int) FieldCrystalDataTable510::raggio_curvatura10_err] );
		Ld10.emplace_back( crysdata[(int) FieldCrystalDataTable510::dechanneling_lenght10] / LDe );
		Ld10_err.emplace_back( crysdata[(int) FieldCrystalDataTable510::dechanneling_lenght10_err] / LDe );

		//Total data
		Rc5_tot.emplace_back( crysdata_tot[(int) FieldCrystalDataTable510::raggio_curvatura5] );
		Rc5_tot_err.emplace_back( crysdata_tot[(int) FieldCrystalDataTable510::raggio_curvatura5_err] );
		Ld5_tot.emplace_back( crysdata_tot[(int) FieldCrystalDataTable510::dechanneling_lenght5] / LDe );
		Ld5_tot_err.emplace_back( crysdata_tot[(int) FieldCrystalDataTable510::dechanneling_lenght5_err] / LDe );

		Rc10_tot.emplace_back( crysdata_tot[(int) FieldCrystalDataTable510::raggio_curvatura10] );
		Rc10_tot_err.emplace_back( crysdata_tot[(int) FieldCrystalDataTable510::raggio_curvatura10_err] );
		Ld10_tot.emplace_back( crysdata_tot[(int) FieldCrystalDataTable510::dechanneling_lenght10]  / LDe );
		Ld10_tot_err.emplace_back( crysdata_tot[(int) FieldCrystalDataTable510::dechanneling_lenght10_err] / LDe );
	}*/

	//If I put other options, I could write the other options directly on canvas title

	string canvas_name = (argc >= 2) ? "LDnuc/LDelet vs 1-Rc/R " + string( argv[1] ) + " " +
			string(argv[2]) :  "Rc vs Ld " + string( "-dat" ) + string( "-sharp" );
//	string canvas_name = (argc >= 2) ? "LDnuc/LDelet vs 1-Rc/R " + string( argv[1] ) + " " +
//			string(argv[2]) :  "Rc vs Ld " + string( "-dat" ) + string( "-sharp" );



	for (int i = 0; i < 10; ++i) {
		cout << endl;
	}


	auto cRcLd = new TCanvas( "cRcLd", canvas_name.c_str() );
	// 2 rows, 2 columns
	cRcLd->Divide( 2, 2 );
	{
		//auto RcLd_5 = new TGraphErrors(n,x,y,ex,ey);
		// choose upper left pad https://root.cern.ch/canvases-and-pads
		cRcLd->cd( 1 );
		auto len = Rc5.size();
		auto RcLd_5 = new TGraphErrors( len, Rc5.data(), Ld5.data(), Rc5_err.data(), Ld5_err.data() );
		/*
		 * Fit Options (https://root.cern.ch/doc/master/classTGraph.html#aa978c8ee0162e661eae795f6f3a35589):
		 *  "W" 	Set all weights to 1; ignore error bars
		 *  "U" 	Use a User specified fitting algorithm (via SetFCN)
		 *	"Q" 	Quiet mode (minimum printing)
		 *	"V" 	Verbose mode (default is between Q and V)
		 *	"E" 	Perform better Errors estimation using Minos technique
		 *	"B" 	User defined parameter settings are used for predefined functions like "gaus", "expo", "poln", "landau". Use this option when you want to fix one or more parameters for these functions.
		 *	"M" 	More. Improve fit results. It uses the IMPROVE command of TMinuit (see TMinuit::mnimpr). This algorithm attempts to improve the found local minimum by searching for a better one.
		 *	"R" 	Use the Range specified in the function range
		 *	"N" 	Do not store the graphics function, do not draw
		 *	"0" 	Do not plot the result of the fit. By default the fitted function is drawn unless the option "N" above is specified.
		 *	"+" 	Add this new fitted function to the list of fitted functions (by default, any previous function is deleted)
		 *	"C" 	In case of linear fitting, do not calculate the chisquare (saves time)
		 *	"F" 	If fitting a polN, use the minuit fitter
		 *	"EX0" 	When fitting a TGraphErrors or TGraphAsymErrors do not consider errors in the coordinate
		 *	"ROB" 	In case of linear fitting, compute the LTS regression coefficients (robust (resistant) regression), using the default fraction of good points "ROB=0.x" - compute the LTS regression coefficients, using 0.x as a fraction of good points
		 *	"S" 	The result of the fit is returned in the TFitResultPtr (see below Access to the Fit Result)
		 */
		/*
		 *  Graphic options (https://root.cern.ch/doc/master/classTGraphPainter.html):
		 *  "A" 	Axis are drawn around the graph
		 *  "I" 	Combine with option 'A' it draws invisible axis
		 *  "L" 	A simple polyline is drawn
		 *	"F" 	A fill area is drawn ('CF' draw a smoothed fill area)
		 *	"C" 	A smooth Curve is drawn
		 *	"*" 	A Star is plotted at each point
		 *	"P" 	The current marker is plotted at each point
		 *	"B" 	A Bar chart is drawn
		 *	"1" 	When a graph is drawn as a bar chart, this option makes the bars start from the bottom of the pad. By default they start at 0.
		 *	"X+" 	The X-axis is drawn on the top side of the plot.
		 *	"Y+" 	The Y-axis is drawn on the right side of the plot.
		 */

		TF1 *fRcLd_5 = new TF1( "fRcLd_5", "pol1" );
		//TF1 *fRcLd_5 = new TF1( "fRcLd_5", "[0]*pow(1-[b]/x,2)" );
		TFitResultPtr fit_RcLd_5 = RcLd_5->Fit( fRcLd_5, "S" );
		cout << "Results for the +-5 cuts sharp fit" << endl;
		fit_RcLd_5->Print("V");

		clog << "k: " << fit_RcLd_5->Parameter(1) << endl;
		clog << "kerr: " << fit_RcLd_5->Error(1) << endl;
		cout << "1/k = " << 1.0/fit_RcLd_5->Parameter(1)
			 << " +- "<< fit_RcLd_5->Error(1)/pow(fit_RcLd_5->Parameter(1),2)
			 << endl;

		//RcLd_5->SetTitle( "Rc vs Ld (cuts at 5)" );
		RcLd_5->SetTitle( "Ln/Le vs (1-Rc/R)^2 (cuts at 5)" );
		RcLd_5->GetXaxis()->SetTitle( "Rc [m]" );
		RcLd_5->GetYaxis()->SetTitle( "Ld [m]" );
		RcLd_5->Draw( "AP*" );
		RcLd_5->Write();
	}

	// Plots at 10 cuts
	{
		// choose upper right pad https://root.cern.ch/canvases-and-pads
		cRcLd->cd( 2 );
		auto len = Rc10.size();
		auto RcLd_10 = new TGraphErrors( len, Rc10.data(), Ld10.data(), Rc10_err.data(), Ld10_err.data() );

		RcLd_10->Print();

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
		RcLd_10->Write();
		RcLd_10->Print();
	}

	// Plots Total at 5 cuts
	{
		// choose lower left pad https://root.cern.ch/canvases-and-pads
		cRcLd->cd( 3 );
		auto len = Rc5_tot.size();
		auto RcLd_5_tot = new TGraphErrors( len, Rc5_tot.data(), Ld5_tot.data(), Rc5_tot_err.data(),
				Ld5_tot_err.data() );

		TF1 *fRcLd_5_tot = new TF1( "fRcLd_5_tot", "pol1" );
		//TF1 *fRcLd_5_tot = new TF1( "fRcLd_5", "[0]*pow(1-[b]/x,2)" );
		TFitResultPtr fit_RcLd_5_tot = RcLd_5_tot->Fit( fRcLd_5_tot, "S" );
		RcLd_5_tot->SetTitle( "Ln/Le vs (1-Rc/R)^2 (cuts at 5), total sum fit" );
		RcLd_5_tot->GetXaxis()->SetTitle( "Rc [m]" );
		RcLd_5_tot->GetYaxis()->SetTitle( "Ld [m]" );
		RcLd_5_tot->Draw( "AP*" );
		RcLd_5_tot->Write();
	}

	// Plots Total at 10 cuts
	{
		// choose lower right pad https://root.cern.ch/canvases-and-pads
		cRcLd->cd( 4 );
		auto len = Rc10_tot.size();
		auto RcLd_10_tot = new TGraphErrors( len, Rc10_tot.data(), Ld10_tot.data(), Rc10_err.data(), Ld10_err.data() );

		TF1 *fRcLd_10_tot = new TF1( "fRcLd_10_tot", "pol1" );
		//TF1 *fRcLd_10_tot = new TF1( "fRcLd_5", "[0]*pow(1-[b]/x,2)" );
		TFitResultPtr fit_RcLd_5 = RcLd_10_tot->Fit( fRcLd_10_tot, "S" );
		RcLd_10_tot->SetTitle( "Ln/Le vs (1-Rc/R)^2 (cuts at 10), total sum fit" );
		RcLd_10_tot->GetXaxis()->SetTitle( "Rc [m]" );
		RcLd_10_tot->GetYaxis()->SetTitle( "Ld [m]" );
		RcLd_10_tot->Draw( "AP*" );
		RcLd_10_tot->Write();
	}

	cout << "LDe(1,400): " << electronic_dechanneling( 1, 400 ) << endl;
	currentDir->cd();
	//mia_dech(); TCanvas


	} // End dech scope here



	{ // Start VRAM analysis

	// Parte per analisi del triangolino finale
//	auto deltaslice = 1; //[murad]
//	system("rm -f Varie/Video/*.png");
//	system("rm -f Varie/Video/GIF_slices.gif");
//	vector<TH1D*> vhist;
//	vhist.reserve(32);
//	for (int i = 160; i < 190; i=i+deltaslice) {
//		TH1D* thistogram;
//		slices(i,i+deltaslice,thistogram,true);
//		vhist.push_back(thistogram);
//	}
//
//	// Requires imagemagick
//	// sudo apt-get install imagemagick
//	auto gifreturnval = system("convert -delay 20 -loop 0 Varie/Video/*.png Varie/Video/GIF_slices.gif");
//	if ( bool(gifreturnval) ) {
//		clog << "[WARNING]: Failed making the .gif" << endl;
//		clog << "[WARNING]: Maybe you have to install imagemagick?" << endl;
//		clog << "[WARNING]: sudo apt-get install imagemagick" << endl;
//	}
//
//	TH1D* hVR;
//	TH1D* hAM;
//	slices(160,160+deltaslice,hVR);
//	slices(190,190+deltaslice,hAM);

	// Non sono sicuro sia una buona idea
	//AnalisiVRAM_param crystalparamsSTF45;
	//crystalparamsSTF45.start_analysis =

	// "Generic" container for various parameter to use in the analysis
	/*
	 * Orientation:
	 * 1)
	 *
	 */

	// Parameters of STF45
	std::map<std::string, double> STF99_par;
	STF99_par["chi2_threshold"] = 1;

	std::map<std::string, double> ST101_par;
	ST101_par["chi2_threshold"] = 80;
	ST101_par["lowYrange_slices"] = -40; // murad. What range save of the slices
	ST101_par["highYrange_slices"] = 30; // murad. What range save of the slices
	//TODO passarlo anche a slices dentro analisi


	clog << "\n\n\n***********************************************************\n"
			"Now analyzing the VR to AM transition\n\n\n";
	//analisi_VRtoAM("ST101","exp",-210, 75);//70
	//analisi_VRtoAM("STF99","exp",-150, 50);
	//analisi_VRtoAM("STF45", "exp", 160, 30);

	//analisi_VRtoAM_chi2("STF99","exp",-160, 90, STF99_par);
	analisi_VRtoAM_chi2("ST101","exp",-190, 50, ST101_par);


	}
	return 0;
}

