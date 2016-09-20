//#include "librerie.h"
#include "func_sim.h"

void eff50() {

  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TGaxis::SetMaxDigits(3);
  
  //Apre file dati
  //TFile * in_file = new TFile("recoDataSimple_608.torsion.correction.histo.root");
  
  TFile * in_file = new TFile("STF51Rob_Exp.root");
  TString outputfile = "STF51Rob.efficiency_Exp.root"; 

 TFile *outfile = new TFile(outputfile,"RECREATE");
  
  //TFile * in_file = new TFile("STF47Rob_plusErrors.root");
  //TString outputfile = "STF47Rob.efficiency_plusErrors.root"; 
  
  //Selezione histogramma
  /*
  TH2D * h = (TH2D*) in_file->Get("dTheta_x_vs_Impact_x_cor");
  TH1D * h1 = (TH1D*) in_file->Get("dTheta_x"); 
  */

  TH2F * h = (TH2F*) in_file->Get("hH8");
  
  TCanvas *c = new TCanvas("colz");
  h->Draw();

  /*
  TCanvas *c1 = new TCanvas();
  h1->Draw();
  
  
  Double_t par_ch[3];
  TF1 *g    = new TF1("g","gaus",20,80);
  g->SetLineColor(kRed); 
  h1->Fit(g,"R");
  g->GetParameters(&par_ch[0]);
  */
  /*
  cout << "paramteri fit"<<endl;
  cout << par_ch[2] << endl;
  cout << par_ch[1] << endl;
  
  Int_t ch_min = par_ch[1]-(par_ch[2]*3);
  Int_t ch_max = par_ch[1]+(par_ch[2]*3);
  */

  //cout << "intorno di channeling " << endl;
  //cout << "[" << ch_min << "," << ch_max << "]" << endl; 

 
  //TCanvas *c2 = new TCanvas();
  //h1_vs_Impact_x_nocut.Draw("colz");


  //analisi efficenza -> tagli angolari

  
  TAxis *axis = h->GetXaxis();
  //taglio +/- 2,5 urad

 //Picco AM
  int minAM = axis->FindBin(-30); 
  int maxAM = axis->FindBin(-20);

  //Picco VR
  int minVR = axis->FindBin(20);
  int maxVR = axis->FindBin(30); 

  int min5 = axis->FindBin(-5.);
  int max5 = axis->FindBin(5.); 

  int min10 = axis->FindBin(-10.);
  int max10 = axis->FindBin(10.); 

  cout << "bin cut" << endl;
  cout << "bin cut" << endl;
  cout << " -30 -> "<< minAM <<" -20 -> "<< maxAM << endl;
  cout << "  20 -> "<< minVR <<"  30 -> "<< maxVR << endl;
  cout << "  -5 -> "<< min5  <<"   5 -> "<< max5 << endl;
  cout << " -10 -> "<< min10 <<"  10 -> "<< max10 << endl;

  TH1D *hAM= h->ProjectionY("cut AM",minAM,maxAM);
  //hAM->Rebin(4);
  hAM->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  
  TH1D *hVR= h->ProjectionY("cut VR",minVR,maxVR);
  //hVR->Rebin(4);
  hVR->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
   
  TH1D *h5= h->ProjectionY("cut +/- 5 urad",min5,max5);
  h5->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  //h5->Rebin(4);  
  h5->Write("h5");
  
  TH1D *h10= h->ProjectionY("cut +/- 10 urad",min10,max10);
  h10->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  //h10->Rebin(4);
  h10->Write("h10");
 //plot e fit 


TCanvas *cAM = new TCanvas("cAM","cAM",1100,1100);
  hAM->Draw();
  Double_t par_am[3];
  TF1 *gAM    = new TF1("gAM","gaus",-50,50);
  gAM->SetLineColor(kRed); 
  hAM->Fit(gAM,"R");
  gAM->GetParameters(&par_am[0]);
  
  cout << "paramteri fit picco amorfo"<<endl;
  cout << par_am[2] << endl;
  cout << par_am[1] << endl;
  
  TCanvas *cVR = new TCanvas("cVR","cVR",1100,1100);
  hVR->Draw();
  Double_t par_vr[3];
  TF1 *gVR    = new TF1("gVR","gaus",-50,50);
  gVR->SetLineColor(kRed); 
  hVR->Fit(gVR,"R");
  gVR->GetParameters(&par_vr[0]);
  
  cout << "paramteri fit picco volume reflection"<<endl;
  cout << par_vr[2] << endl;
  cout << par_vr[1] << endl;
  


  TCanvas *c5 = new TCanvas();
  h5->Draw();
  Double_t par_ch5[3];
  TF1 *g5    = new TF1("g5","gaus",25,80);
  g5->SetLineColor(kRed); 
  h5->Fit(g5,"R");
  g5->GetParameters(&par_ch5[0]);
  double sigma5_err = g5->GetParError(2);  
  double cost5_err = g5->GetParError(0);

  cout << "paramteri fit"<<endl;
  cout << par_ch5[2] << endl;
  cout << par_ch5[1] << endl;
  cout << sigma5_err << " " << cost5_err << endl;  

  Int_t ch_min5 = par_ch5[1]-(par_ch5[2]*3);
  Int_t ch_max5 = par_ch5[1]+(par_ch5[2]*3);


  TCanvas *c10 = new TCanvas();
  h10->Draw();
  Double_t par_ch10[3];
  TF1 *g10    = new TF1("g10","gaus",25,80);
  g10->SetLineColor(kRed); 
  h10->Fit(g10,"R");
  g10->GetParameters(&par_ch10[0]);
  double sigma10_err = g10->GetParError(2);  
  double cost10_err = g10->GetParError(0);
  
  cout << "paramteri fit"<<endl;
  cout << par_ch10[2] << endl;
  cout << par_ch10[1] << endl;
  cout << sigma10_err << " " << cost10_err << endl;  

  Int_t ch_min10 = par_ch10[1]-(par_ch10[2]*3);
  Int_t ch_max10 = par_ch10[1]+(par_ch10[2]*3);


  cout << "integral bound"<< endl;

  cout << "  5 lower "<<ch_min5 <<" higher " << ch_max5 << endl;
  cout << " 10 lower "<<ch_min10 <<" higher " << ch_max10 << endl;

  //integrali
 
  
  
    

  TAxis *x_axis5 = h5->GetXaxis();
  int bmin = x_axis5->FindBin(-200);
  int bmax = x_axis5->FindBin(200);   

  int bmin_ch5 = x_axis5->FindBin(ch_min5);
  int bmax_ch5 = x_axis5->FindBin(ch_max5); 

  double int5_err;
  // double int_ch5_err;

  //double int5 = h5->IntegralAndError(bmin,bmax,int5_err);    
double int5 = h5->Integral(bmin,bmax);    
  //  double int_ch5 = h5->Integral(bmin_ch5,bmax_ch5);
  double int_ch5 = sqrt(2.*TMath::Pi())*par_ch5[2]*par_ch5[0];
  double int_ch5_err = int_ch5*((sigma5_err/par_ch5[2])+(cost5_err/par_ch5[0]));
       
  double eff5 = int_ch5/int5;
  double eff5_err = eff5 * ((int_ch5_err/int_ch5)+(int5_err/int5));

  cout << "angle cut @ +/- 5 urad" << endl;
  cout << "#tot events " << int5 << " #ch events "<< int_ch5 << "  --> efficiency "<<eff5  <<" +/- " << eff5_err<< endl; 


  TAxis *x_axis10 = h10->GetXaxis();
  
  int bmin_ch10 = x_axis10->FindBin(ch_min10);
  int bmax_ch10 = x_axis10->FindBin(ch_max10); 

  double int10_err;
  //  double int_ch10_err;

  //double int10 = h10->IntegralAndError(bmin,bmax,int10_err);    
double int10 = h10->Integral(bmin,bmax);    
  //  double int_ch10 = h10->Integral(bmin_ch10,bmax_ch10);
  double int_ch10 = sqrt(2.*TMath::Pi())*par_ch10[2]*par_ch10[0];
  double int_ch10_err = int_ch10*((sigma10_err/par_ch10[2])+(cost10_err/par_ch10[0]));
       
  double eff10 = int_ch10/int10;
  double eff10_err = eff10 * ((int_ch10_err/int_ch10)+(int10_err/int10));

  cout << "angle cut @ +/- 10 urad" << endl;
  cout << "#tot events " << int10 << " #ch events "<< int_ch10 << "  --> efficiency "<<eff10  <<" +/- " << eff10_err<< endl; 





  TCanvas *c51 = new TCanvas("c51","c51",1100,1100);
  h5->Draw();
  TLegend *leg5 = new TLegend(0.1,0.7,0.48,0.9);
  leg5->SetHeader("efficiency at +/- 5 #murad");
  leg5->AddEntry(h5,Form("number of total event = %5.0f",int5));
  leg5->AddEntry(h5,Form("number of channeled event = %5.0f",int_ch5));
  leg5->AddEntry(h5,Form("efficiency = %5.3f",eff5));
  leg5->Draw();

  TCanvas *c101 = new TCanvas("c101","c101",1100,1100);
  h10->Draw();
  TLegend *leg10 = new TLegend(0.1,0.7,0.48,0.9);
  leg10->SetHeader("efficiency at +/- 10 #murad");
  leg10->AddEntry(h10,Form("number of total event = %5.0f",int10));
  leg10->AddEntry(h10,Form("number of channeled event = %5.0f",int_ch10));
  leg10->AddEntry(h10,Form("efficiency = %5.3f",eff10));
  leg10->Draw();


  Int_t nbins = h10->GetSize();

  cout<<nbins<<endl;

  Float_t tot;

  for(i=0;i<nbins-2;i++)
    tot+=h10->GetBinContent(i);

  cout<<tot<<endl;

  cout<<par_ch10[2]<<" "<<par_ch10[0]<<" "<<par_ch10[1]<<endl;


} 










