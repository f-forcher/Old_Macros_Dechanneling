/*
 * mia_macro.C
 *
 *  Created on: Sep 20, 2016
 *
 *  Copia di dech per analizzare i cristalli
 *
 */

//#include "librerie.h"
#include "func_sim.h"

Double_t myfunction(Double_t *x, Double_t *par) {
	// TODO Float?
	// Float_t xx = x[0];

	Float_t xx = x[0];
	Double_t f = par[0] * exp( -xx / par[1] );
	return f;
}

// TODO Provata con STF45, "generalizzare" agli altri
/*
 * Macro to calculate dechanneling lenght for a crystal.
 * @nome_cristallo: Nabe of the folder with the crystal data
 * @output_dech: file in which to append the "Ldech"s, in the format
 *               Crystal | dechanneling L at +-5 microrad [m] | dechanneling L at +-10 microrad [m]
 */
void dech(std::string nome_cristallo, std::shared_ptr<std::ofstream> output_dech) {

	gStyle->SetPalette( 1 );
	gStyle->SetOptStat( 0 );
	gStyle->SetOptTitle( 0 );
	TGaxis::SetMaxDigits( 3 );

	// TODO Togliere i nomi specifici e usare quelli _renamed
	std::string cartella_cristallo = "ForFrancesco/" + nome_cristallo + "_exp/";
	gSystem->ChangeDirectory( cartella_cristallo.c_str() );
	std::string pathfiledati = cartella_cristallo + "recoDataSimple_renamed.torsion.correction.histo.root";

	// TODO Generalizzare il nome del file
	std::string filedati = "recoDataSimple_renamed.torsion.correction.histo.root";

	//Apre file dati
	// TFile * in_file = new TFile("./ForFrancesco/STF45_exp/Analysis/recoDataSimple_415_1-37.torsion.correction.histo.root");
	TFile * in_file = new TFile( filedati.c_str() );
	//TString outputfile = "recoDataSimple_415.dechanneling.root";

	//TFile * in_file = new TFile("STF45Rob.root");
	//TString outputfile = "STF45Rob.dechanneling.root";

	//TFile * in_file = new TFile("STF45Rob_Exp.root");
	TString outputfile = nome_cristallo + ".dechanneling_Exp_new.root";

	//xtal length and curvature TODO Controllare!
	Double_t z = 2e-3;
	Double_t R;

	//Selezione histogramma

	TH2D * h = (TH2D*) in_file->Get( "dTheta_x_vs_Impact_x_cor" );
	//TH1D * h1 = (TH1D*) in_file->Get("dTheta_x_5");

	//TH2F * h = (TH2F*) in_file->Get("hH8");
	TH1D *h1 = h->ProjectionY();

	TCanvas *c = new TCanvas();
	h->Draw( "colz" );

	TCanvas *c1 = new TCanvas();
	h1->Draw();

	//test preliminare e inizializzazione parametri

	// TODO chiedere dei nomi, dei numeri, etc
	Double_t par_ch[3] = { 0 };
	TF1 *g = new TF1( "g", "gaus", 110, 180 );
	g->SetLineColor( kRed );
	h1->Fit( g, "R" );
	g->GetParameters( &par_ch[0] );
	R = (z / (par_ch[1] * 1e-6));
	cout << R << endl;

	Double_t min_ch = par_ch[1] - (par_ch[2] * 3.);
	Double_t max_ch = par_ch[1] + (par_ch[2] * 3.);

	Double_t par_de[3] = { 0 };
	TF1 *de = new TF1( "de", "expo", 20, 110 );
	de->SetLineColor( kGreen + 2 );
	//de->SetParameters(1,1);
	h1->Fit( de, "R+" );
	de->GetParameters( &par_de[0] );

	cout << par_de[0] << " " << par_de[1] << endl;

	Double_t zero = exp( par_de[0] );
	Double_t uno = -(1 / par_de[1]);

	cout << zero << " " << uno << endl;

	// tagli angolari
	TAxis *axis = h->GetXaxis();

	int min5 = axis->FindBin( -5. );
	int max5 = axis->FindBin( 5. );

	int min10 = axis->FindBin( -10. );
	int max10 = axis->FindBin( 10. );

	cout << "bin cut" << endl;
	cout << "  -5 -> " << min5 << "   5 -> " << max5 << endl;
	cout << " -10 -> " << min10 << "  10 -> " << max10 << endl;

	TH1D *h5 = h->ProjectionY( "cut +/- 5 urad", min5, max5 );
	h5->GetXaxis()->SetTitle( "#Delta#theta_{x} [#murad]" );
	h5->Rebin( 4 );
	TH1D *h10 = h->ProjectionY( "cut +/- 10 urad", min10, max10 );
	h10->GetXaxis()->SetTitle( "#Delta#theta_{x} [#murad]" );
	h10->Rebin( 4 );

	//plot e fit
	TCanvas *c10 = new TCanvas( "c10", "c10", 1100, 1100 );
	c10->SetFillColor( 10 );
	c10->SetLogy( 1 );
	h10->GetYaxis()->SetLabelFont( 132 );
	h10->GetYaxis()->SetTitleFont( 132 );
	h10->GetXaxis()->SetLabelFont( 132 );
	h10->GetXaxis()->SetTitleFont( 132 );
	h10->GetYaxis()->SetTitle( "N" );
	h10->Draw();

	Double_t par_10[8];
	Double_t par_am10[3] = { 0 };
	Double_t par_ch10[3] = { 0 };
	TF1 *g10 = new TF1( "g10", "gaus", 120, 180 );
	TF1 *g10am = new TF1( "g10am", "gaus", -50, 10 );
	g10->SetLineColor( kRed );
	h10->Fit( g10, "R+" );
	g10->GetParameters( &par_ch10[0] );
	g10am->SetLineColor( kYellow );
	h10->Fit( g10am, "R+" );
	g10am->GetParameters( &par_am10[0] );
	g10am->GetParameters( &par_10[0] );
	g10->GetParameters( &par_10[5] );

	cout << "parametri fit" << endl;
	cout << par_ch10[2] << endl;
	cout << par_ch10[1] << endl;

	Double_t ch_min10 = par_ch10[1] - (par_ch10[2] * 2);
	Double_t ch_max10 = par_ch10[1] + (par_ch10[2] * 2);

	cout << "parametri fit" << endl;
	cout << par_am10[2] << endl;
	cout << par_am10[1] << endl;

	// Calcolo limiti dei picchi
	Double_t am_min10 = par_am10[1] - (par_am10[2] * 2);
	Double_t am_max10 = par_am10[1] + (par_am10[2] * 2);

	Double_t R10 = (z / (par_ch10[1] * 1e-6)); // uso sempre questo raggio a +/- 10

	Double_t ch_min101 = par_ch10[1] - (par_ch10[2] * 3);
	Double_t am_max101 = par_am10[1] + (par_am10[2] * 3);

	Double_t ch_min102 = par_ch10[1] - (par_ch10[2] * 4.);
	Double_t am_max102 = par_am10[1] + (par_am10[2] * 4.);

	/*
	 TLine *l_ch_min10 = new TLine(ch_min10,0,ch_min10,5000);
	 l_ch_min10->SetLineColor(kRed);
	 TLine *l_ch_min101 = new TLine(ch_min101,0,ch_min101,5000);
	 l_ch_min101->SetLineColor(kRed);
	 TLine *l_ch_min102 = new TLine(ch_min102,0,ch_min102,5000);
	 l_ch_min102->SetLineColor(kRed);
	 TLine *l_am_max10 = new TLine(am_max10,0,am_max10,5000);
	 l_am_max10->SetLineColor(kYellow);
	 TLine *l_am_max101 = new TLine(am_max101,0,am_max101,5000);
	 l_am_max101->SetLineColor(kYellow);
	 TLine *l_am_max102 = new TLine(am_max102,0,am_max102,5000);
	 l_am_max102->SetLineColor(kYellow);
	 */

	//fit dechanneling  uso sempre R10
	Double_t par_de10[3] = { 0 };
	TF1 *de10 = new TF1( "de10", myfunction, am_max101, ch_min102, 2 );
	de10->SetLineColor( kGreen );
	de10->SetParameters( zero, uno );
	h10->Fit( de10, "R" );
	gStyle->SetOptFit( 1111 );
	h10->Fit( g10, "R+" );
	h10->Fit( g10am, "R+" );

	c10->Update();
	de10->GetParameters( &par_de10[0] );

	cout << "min = " << am_max10 << "max = " << ch_min10 << endl << endl;

	//TODO salvare su un file
	Double_t dechanneling10 = R10 * (par_de10[1] * 1e-6);
	cout << nome_cristallo << "_lunghezza_di_dechanneling_10:" << dechanneling10 << endl << endl;

	/* TEST VARIE DIMENSIONI FIT
	 Double_t par_de10n[3]=0;
	 TF1 *de10n   = new TF1("de10n","expo",am_max10,ch_min10);
	 de10n->SetLineColor(kGreen+1);
	 //de10->SetParameters(zero,uno);
	 //h10->Fit(de10n,"R+");
	 gStyle->SetOptFit(1111);

	 de10n->GetParameters(&par_de10n[0]);

	 cout << "min = " << am_max10 << "max = " << ch_min10 << endl;
	 cout << "lunghezza di dechanneling " << R5/(par_de10n[1]*1e6) << endl;


	 Double_t par_de101[3]=0;
	 TF1 *de101   = new TF1("de101",myfunction,am_max101,ch_min101,2);
	 de101->SetLineColor(kGreen+2);
	 de101->SetParameters(zero,uno);
	 //h10->Fit(de101,"R+");

	 de101->GetParameters(&par_de101[0]);
	 cout << "min = " << am_max101 << "max = " << ch_min101 << endl;
	 cout << "lunghezza di dechanneling " << R5*(par_de101[1]*1e-6) << endl;

	 Double_t par_de101n[3]=0;
	 TF1 *de101n   = new TF1("de101n","expo",am_max101,ch_min101);
	 de101n->SetLineColor(kGreen+3);
	 //de101n->SetParameters(zero,uno);
	 //h10->Fit(de101n,"R+");

	 de101n->GetParameters(&par_de101n[0]);
	 cout << "min = " << am_max101 << "max = " << ch_min101 << endl;
	 cout << "lunghezza di dechanneling " << R5/(par_de101n[1]*1e6) << endl;


	 Double_t par_de102[3]=0;
	 TF1 *de102   = new TF1("de102",myfunction,am_max102,ch_min102,2);
	 de102->SetLineColor(kGreen+4);
	 de102->SetParameters(zero,uno);
	 // h10->Fit(de102,"R+");

	 de102->GetParameters(&par_de102[0]);
	 cout << "min = " << am_max102 << "max = " << ch_min102 << endl;
	 cout << "lunghezza di dechanneling " << R5*(par_de102[1]*1e-6) << endl;


	 Double_t par_de102n[3]=0;
	 TF1 *de102n   = new TF1("de102n","expo",am_max102,ch_min102);
	 de102n->SetLineColor(kGreen+5);
	 //de102->SetParameters(zero,uno);
	 //  h10->Fit(de102n,"R+");

	 de102n->GetParameters(&par_de102n[0]);
	 cout << "min = " << am_max102 << "max = " << ch_min102 << endl;
	 cout << "lunghezza di dechanneling " << R5/(par_de102n[1]*1e6) << endl;
	 */
	/*
	 leg10 = new TLegend(0.1,0.7,0.48,0.9);
	 leg10->SetHeader("dechanneling legth at +/- 10 #murad");
	 leg10->AddEntry(de10,Form("L_{D} = %5.3f mm",R10*(par_de10[1]*1e-3)));
	 // leg10->AddEntry(de101,Form("L_{D} = %5.3f mm",R5*(par_de101[1]*1e-3)));
	 // leg10->AddEntry(de102,Form("L_{D} = %5.3f mm",R5*(par_de102[1]*1e-3)));
	 //  leg10->AddEntry(de10n,Form("L_{D} = %5.3f mm",R5/(par_de10n[1]*1e3)));
	 //  leg10->AddEntry(de101n,Form("L_{D} = %5.3f mm",R5/(par_de101n[1]*1e3)));
	 //  leg10->AddEntry(de102n,Form("L_{D} = %5.3f mm",R5/(par_de102n[1]*1e3)));

	 leg10->Draw();
	 */

	auto leg10 = new TLegend( 0.35, 0.25, 0.7, 0.4 );
	leg10->SetHeader( "Dechanneling length for - #theta_{c} < #theta_{in} < #theta_{c}" );
	leg10->AddEntry( de10, Form( "L_{D} = %.2f mm", R10 * (par_de10[1] * 1e-3) ), "l" );
	leg10->SetFillColor( 10 );
	leg10->SetTextFont( 132 );
	leg10->SetTextSize( 0.035 );
	leg10->Draw();

	TCanvas *c5 = new TCanvas( "c5", "c5", 1100, 1100 );
	c5->SetFillColor( 10 );
	c5->SetLogy( 1 );
	h5->GetYaxis()->SetLabelFont( 132 );
	h5->GetYaxis()->SetTitleFont( 132 );
	h5->GetYaxis()->SetTitle( "Entries" );
	h5->Draw();

	Double_t par_am5[3] = { 0 };
	Double_t par_ch5[3] = { 0 };
	TF1 *g5 = new TF1( "g5", "gaus", min_ch, max_ch );
	TF1 *g5am = new TF1( "g5am", "gaus", -50, 50 );

	g5->SetLineColor( kRed );
	g5am->SetLineColor( kYellow );
	h5->Fit( g5, "R+" );
	h5->Fit( g5am, "R+" );

	g5->GetParameters( &par_ch5[0] );
	g5am->GetParameters( &par_am5[0] );

	cout << "paramteri fit picco channeling" << endl;
	cout << par_ch5[2] << endl;
	cout << par_ch5[1] << endl;

	cout << "paramteri fit picco amorfo" << endl;
	cout << par_am5[2] << endl;
	cout << par_am5[1] << endl;

	Double_t ch_min5 = par_ch5[1] - (par_ch5[2] * 3);
	Double_t ch_max5 = par_ch5[1] + (par_ch5[2] * 3);
	Double_t R5 = (z / (par_ch5[1] * 1e-6));

	Double_t am_min5 = par_am5[1] - (par_am5[2] * 3);
	Double_t am_max5 = par_am5[1] + (par_am5[2] * 3);

	Double_t ch_min51 = par_ch5[1] - (par_ch5[2] * 3.5);
	Double_t am_max51 = par_am5[1] + (par_am5[2] * 3.5);

	Double_t ch_min52 = par_ch5[1] - (par_ch5[2] * 4.);
	Double_t am_max52 = par_am5[1] + (par_am5[2] * 4.);

	//fit dechanneling uso sempre R10
	Double_t par_de5[3] = { 0 };
	gStyle->SetOptFit( 1111 );
	TF1 *de5 = new TF1( "de5", myfunction, am_max5, ch_min51, 2 );
	de5->SetLineColor( kGreen );
	de5->SetParameters( zero, uno );
	h5->Fit( de5, "R" );
	h5->Fit( g5, "R+" );
	h5->Fit( g5am, "R+" );

	c5->Update();

	de5->GetParameters( &par_de5[0] );
	//cout << "lunghezza di dechanneling " << R10 * (par_de5[1] * 1e-6) << endl;
	//TODO salvare su un file
	Double_t dechanneling5 = R10 * (par_de5[1] * 1e-6);
	cout << nome_cristallo << "_lunghezza_di_dechanneling_5:" << dechanneling5 << endl << endl;

	/*TEST VARIE DIMENSIONI FIT
	 Double_t par_de51[3]=0;
	 TF1 *de51   = new TF1("de51",myfunction,am_max5,ch_min51,2);
	 de51->SetLineColor(kGreen+2);
	 de51->SetParameters(zero,uno);
	 //h5->Fit(de51,"R+");

	 de51->GetParameters(&par_de51[0]);
	 cout << "lunghezza di dechanneling " << R5*(par_de51[1]*1e-6) << endl;

	 Double_t par_de52[3]=0;
	 TF1 *de52   = new TF1("de52",myfunction,am_max5,ch_min52,2);
	 de52->SetLineColor(kGreen+4);
	 de52->SetParameters(zero,uno);
	 //h5->Fit(de52,"R+");

	 de52->GetParameters(&par_de52[0]);
	 cout << "lunghezza di dechanneling " << R5*(par_de52[1]*1e-6) << endl;
	 */

	auto leg5 = new TLegend( 0.35, 0.25, 0.7, 0.4 );
	leg5->SetHeader( "Dechanneling length for - #frac{#theta_{c}}{2} < #theta_{in} < #frac{#theta_{c}}{2}" );
	leg5->AddEntry( de5, Form( "L_{D} = %.2f mm", R10 * (par_de5[1] * 1e-3) ), "l" );
	leg5->SetFillColor( 10 );
	leg5->SetTextFont( 132 );
	leg5->SetTextSize( 0.035 );
	leg5->Draw();

	/*
	 TPaveText *leg5=new TPaveText(0.25,0.45,0.55,0.7,"NDC");
	 leg5->SetFillColor(10);
	 //leg51->SetTextFont(22);
	 leg5->SetTextSize(0.035);
	 leg5->AddText("Average deflection for - #frac{#theta_{c}}{2} < #theta_{in} < #frac{#theta_{c}}{2}");
	 leg5->Draw();
	 leg5->SetTextFont(132);
	 leg5->AddText(Form("Mean = %.2f #murad",par_ch5[1]));
	 leg5->AddText(Form("Sigma = %.2f #murad",par_ch5[2]));
	 leg5->AddText(Form("Channeling efficiency = %.1f %c",eff5*100.0,'%'));

	 leg51->Draw();
	 */

	/*
	 l_ch_min10->Draw();
	 l_ch_min101->Draw();
	 l_ch_min102->Draw();
	 l_am_max10->Draw();
	 l_am_max101->Draw();
	 l_am_max102->Draw();
	 */
	//TCanvas *c66 = new TCanvas();
	//h10->Draw();
	cout << "R = " << R << endl;

	cout << "R5 = " << R5 << endl;

	cout << "R10 = " << R10 << endl;

	//std::ofstream* outputdechanneling = new ofstream("dechanneling_table.txt");

	// Assumendo che abbiamo cambiato cartella e siamo in ./ForFrancesco/$nome_cristallo/Analysis
	//std::ofstream outputdechanneling("../../../dechanneling_table.txt", std::ofstream::out | std::ofstream::trunc);

	//outputdechanneling << "# File generated by the macro mia_dech() for the dechanneling lenghts." << endl;
	//outputdechanneling << "# Crystal | dechanneling L at +-5 microrad [m] | dechanneling L at +-10 microrad [m]" << endl;
	*output_dech << nome_cristallo << "  " << double( dechanneling5 ) << "  " << double( dechanneling10 ) << endl;

	TFile *outfile = new TFile( outputfile, "RECREATE" );
	c5->Write( "dech_5" );
	c10->Write( "dech_10" );

	/*
	 TF1 *den   = new TF1("den","expo",20,110);

	 //de102->SetParameters(zero,uno);
	 h10->Fit(den,"R+");
	 den->GetParameters(&par_10[3]);
	 cout << par_10[0] << endl;
	 cout << par_10[1] << endl;
	 cout << par_10[2] << endl;
	 cout << par_10[3] << endl;
	 cout << par_10[4] << endl;
	 cout << par_10[5] << endl;
	 cout << par_10[6] << endl;
	 cout << par_10[7] << endl;

	 TF1 *tot  = new TF1("tot","gaus(0)+expo(3)+gaus(5)",-100,200);
	 tot->SetLineColor(kOrange);
	 tot->SetParameters(par_10);
	 h10->Fit(tot,"R+");
	 */
}

