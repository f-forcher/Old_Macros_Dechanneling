//#include "librerie.h"
#include "func_sim.h"

void eff() {

  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TGaxis::SetMaxDigits(3);
  
  //Apre file dati
  TFile * in_file = new TFile("recoDataSimple_415_1-37.torsion.correction.histo.root");
  //TFile * in_file = new TFile("STF45Rob_plusErrors.root");
  //TString outputfile = "STF45Rob.efficiency_plusErrors.root"; 

  //TFile * in_file = new TFile("STF45Rob_Exp.root");

  TString outputfile = "STF45Rob.efficiency_Exp_new.root"; 

  TFile *outfile = new TFile(outputfile,"RECREATE");






  //Selezione istogramma
  
  //TH2F * h = (TH2F*) in_file->Get("hH8");
  TH2F * h = (TH2F*) in_file->Get("dTheta_x_vs_Impact_x_cor");

  //TH1D * h1 = (TH1D*) in_file->Get("dTheta_x"); 

  
  TCanvas *c = new TCanvas(1);
  h->Draw("contz");
  
  h->Write("histo");

  //TCanvas *c1 = new TCanvas();
  //h1->Draw();
  

  //lettura dati da istogramma deflessione vs. angolo d'ingresso
  

  //test iniziale su picco di channeling
  /*
  Double_t par_ch[3];
  TF1 *g    = new TF1("g","gaus",120,180);
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
  //cout << "intorno di channeling " << endl;
  //cout << "[" << ch_min << "," << ch_max << "]" << endl; 
  //TCanvas *c2 = new TCanvas();
  //h1_vs_Impact_x_nocut.Draw("colz");
  */


  //tagli anche per analisi picchi in zona AM e VR
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
  cout << " -30 -> "<< minAM <<" -20 -> "<< maxAM << endl;
  cout << "  20 -> "<< minVR <<"  30 -> "<< maxVR << endl;
  cout << "  -5 -> "<< min5  <<"   5 -> "<< max5 << endl;
  cout << " -10 -> "<< min10 <<"  10 -> "<< max10 << endl;

  TH1D *hAM= h->ProjectionY("cut AM",minAM,maxAM);
  hAM->Rebin(4);
  hAM->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  hAM->Write("hAM");

  TH1D *hVR= h->ProjectionY("cut VR",minVR,maxVR);
  hVR->Rebin(4);
  hVR->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  hVR->Write("hVR");
 
  TH1D *h5= h->ProjectionY("cut +/- 5 urad",min5,max5);
  h5->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  h5->Rebin(4);  
  h5->Write("h5");
  
  TH1D *h10= h->ProjectionY("cut +/- 10 urad",min10,max10);
  h10->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  h10->Rebin(4);
  h10->Write("h10");
 //plot e fit 

  TCanvas *cAM = new TCanvas("cAM","cAM",1100,1100);
  cAM->SetFillColor(10);
  hAM->GetYaxis()->SetLabelFont(132);
  hAM->GetYaxis()->SetTitleFont(132);
  hAM->GetYaxis()->SetTitle("Entries");
  hAM->Draw();
  Double_t par_am[3];
  TF1 *gAM    = new TF1("gAM","gaus",-50,50);
  gAM->SetLineColor(kRed); 
  hAM->Fit(gAM,"R");
  gAM->GetParameters(&par_am[0]);
  
  cout << "paramteri fit picco amorfo"<<endl;
  cout << par_am[2]  << endl;
  cout << par_am[1] <<  endl;

  TPaveText *legAM=new TPaveText(0.65,0.75,0.95,0.95,"NDC");
  legAM->SetFillColor(10);
  legAM->SetTextFont(132);
  
  
  legAM->AddText("Average deflection for -3#theta_{c}<#theta_{in}<-2#theta_{c}");
  legAM->AddText(Form("Mean = %.2f #murad",par_am[1]));
  legAM->AddText(Form("Sigma = %.2f #murad",par_am[2]));
  legAM->Draw();

  TCanvas *cVR = new TCanvas("cVR","cVR",1100,1100);
  cVR->SetFillColor(10);
  hVR->GetYaxis()->SetLabelFont(132);
  hVR->GetYaxis()->SetTitleFont(132);
  hVR->GetYaxis()->SetTitle("Entries");
  hVR->Draw();
  Double_t par_vr[3];
  TF1 *gVR    = new TF1("gVR","gaus",-50,50);
  gVR->SetLineColor(kRed); 
  hVR->Fit(gVR,"R");
  gVR->GetParameters(&par_vr[0]);
  
  cout << "paramteri fit picco volume reflection"<<endl;
  cout << par_vr[2] << endl;
  cout << par_vr[1] << endl;
  

  TPaveText *legVR=new TPaveText(0.65,0.75,0.95,0.95,"NDC");
  legVR->SetFillColor(10);
  legVR->SetTextFont(132);
  legVR->AddText("Average deflection for 2#theta_{c}<#theta_{in}<3#theta_{c}");
  legVR->AddText(Form("Mean = %.2f #murad",par_vr[1]));
  legVR->AddText(Form("Sigma = %.2f #murad",par_vr[2]));
  legVR->Draw();

  // fit e calcolo estremi del picco di CH
  TCanvas *c5 = new TCanvas();
  h5->Draw();
  Double_t par_ch5[3];
  TF1 *g5    = new TF1("g5","gaus",120,180);
  g5->SetLineColor(kRed); 
  h5->Fit(g5,"R");
  g5->GetParameters(&par_ch5[0]);
  
  cout << "paramteri fit picco channeling cut +/- 5 urad"<<endl;
  cout << par_ch5[2] << endl;
  cout << par_ch5[1] << endl;
  
  Float_t ch_min5 = par_ch5[1]-(par_ch5[2]*3);
  Float_t ch_max5 = par_ch5[1]+(par_ch5[2]*3);


  // fit e calcolo estremi del picco di CH
  TCanvas *c10 = new TCanvas();
  h10->Draw();
  Double_t par_ch10[3];
  TF1 *g10    = new TF1("g10","gaus",120,180);
  g10->SetLineColor(kRed); 
  h10->Fit(g10,"R");
  g10->GetParameters(&par_ch10[0]);
  
  cout << "paramteri fit picco channeling cut +/- 5 urad"<<endl;
  cout << par_ch10[2] << endl;
  cout << par_ch10[1] << endl;
  
  Float_t ch_min10 = par_ch10[1]-(par_ch10[2]*3);
  Float_t ch_max10 = par_ch10[1]+(par_ch10[2]*3);


  cout << "integral bound"<< endl;
  cout << "  5 lower "<<ch_min5 <<" higher " << ch_max5 << endl;
  cout << " 10 lower "<<ch_min10 <<" higher " << ch_max10 << endl;

  //integrali efficienza ed errori

  TAxis *x_axis5 = h5->GetXaxis();
  //TAxis *y_axis5 = h5->GetYaxis();

  int bmin5 = x_axis5->FindBin(-200);
  int bmax5 = x_axis5->FindBin(200); 
  
  int bmin_ch5 = x_axis5->FindBin(ch_min5);
  int bmax_ch5 = x_axis5->FindBin(ch_max5); 
  /*
  double miny5 = y_axis5->GetMinimum();
  double maxy5 = y_axis5->GetMaximum();
  */


  double int5_err;
  double int_ch5_err;

  //double int5 = h5->IntegralAndError(bmin5,bmax5,int5_err);   
  //double int_ch5 = h5->IntegralAndError(bmin_ch5,bmax_ch5,int_ch5_err);
      
  double int5 = h5->Integral(bmin5,bmax5);   
  double int_ch5 = h5->Integral(bmin_ch5,bmax_ch5);

  double eff5 = int_ch5/int5;
  //double eff5_err = eff5 * ((int_ch5_err/int_ch5)+(int5_err/int5));

  cout << "angle cut @ +/- 5 urad" << endl;
  //cout << "#tot events " << int5 << " #ch events "<< int_ch5 << "  --> efficiency "<<eff5 <<" +/- " << eff5_err<< endl; 
  cout << "#tot events " << int5 << " #ch events "<< int_ch5 << "  --> efficiency "<<eff5 << endl; 


  TAxis *x_axis10 = h10->GetXaxis();
  
  int bmin10 = x_axis10->FindBin(-200);
  int bmax10 = x_axis10->FindBin(200); 

  int bmin_ch10 = x_axis10->FindBin(ch_min10);
  int bmax_ch10 = x_axis10->FindBin(ch_max10); 

  double int10_err;
  double int_ch10_err;

  //double int10 = h10->IntegralAndError(bmin10,bmax10,int10_err);   
  //double int_ch10 = h10->IntegralAndError(bmin_ch10,bmax_ch10,int_ch10_err);
  
  double int10 = h10->Integral(bmin10,bmax10);   
  double int_ch10 = h10->Integral(bmin_ch10,bmax_ch10);
    
  double eff10 = int_ch10/int10;
  //double eff10_err = eff10 * ((int_ch10_err/int_ch10)+(int10_err/int10));

  cout << "angle cut @ +/- 10 urad" << endl;
  //cout << "#tot events " << int10 << " #ch events "<< int_ch10 << "  --> efficiency "<<eff10 <<" +/- " << eff10_err<< endl; 

cout << "#tot events " << int10 << " #ch events "<< int_ch10 << "  --> efficiency "<<eff10 << endl; 


  //plot
 

 TCanvas *c51 = new TCanvas("c51","ch eff 5urad",1100,1100);
  c51->SetFillColor(10);
  h5->GetYaxis()->SetLabelFont(132);
  h5->GetYaxis()->SetTitleFont(132);
  h5->GetXaxis()->SetLabelFont(132);
  h5->GetXaxis()->SetTitleFont(132);
  h5->GetYaxis()->SetTitle("N");
  h5->GetXaxis()->SetRangeUser(-50,200);
  h5->SetLineColor(1);
  h5->Draw();

  
  TPaveText *leg51=new TPaveText(0.25,0.45,0.55,0.7,"NDC");
  leg51->SetFillColor(10);
  //leg51->SetTextFont(22);
  leg51->SetTextSize(0.035);
  leg51->AddText("Average deflection for - #frac{#theta_{c}}{2} < #theta_{in} < #frac{#theta_{c}}{2}");
  leg51->Draw();
  leg51->SetTextFont(132);
  leg51->AddText(Form("Mean = %.2f #murad",par_ch5[1]));
  leg51->AddText(Form("Sigma = %.2f #murad",par_ch5[2]));
  //leg51->AddLine(.0,0.4,1.,0.4);
leg51->AddLine(.0,0.25,1.,0.25);
  //leg51->SetTextFont(22);
  //leg51->AddText(Form("Integration range channeled particles: %.1f<#Delta#theta_{x}<%.1f #murad",ch_min5,ch_max5));
  //leg51->SetTextFont(132);
  leg51->AddText(Form("Channeling efficiency = %.1f %c",eff5*100.0,'%')); 
 
  leg51->Draw();
  /*
  TLegend *leg5 = new TLegend(0.1,0.7,0.48,0.9);
  leg5->SetHeader("efficiency at +/- 5 #murad");
  leg5->AddEntry(h5,Form("number of total event = %5.0f",int5));
  leg5->AddEntry(h5,Form("number of channeled event = %5.0f",int_ch5));
  leg5->AddEntry(h5,Form("efficiency = %5.3f",eff5));
  leg5->Draw();
  */  
  c51->Update();
  double miny5 = gPad->GetUymin();
  double maxy5 = gPad->GetUymax();


  TBox *b5=new TBox(ch_min5,miny5,ch_max5,maxy5);
  b5->SetFillColor(kGreen);
  b5->SetFillStyle(3002);
    b5->Draw("same");



  TCanvas *c101 = new TCanvas("c101","ch eff 10urad",1);
  c101->SetFillColor(10);
  h10->GetYaxis()->SetLabelFont(132);
  h10->GetYaxis()->SetTitleFont(132);
  h10->GetYaxis()->SetTitle("Entries");
  h10->Draw();
  /*
  TLegend *leg10 = new TLegend(0.1,0.7,0.48,0.9);
  leg10->SetHeader("efficiency at +/- 10 #murad");
  leg10->AddEntry(h10,Form("number of total event = %5.0f",int10));
  leg10->AddEntry(h10,Form("number of channeled event = %5.0f",int_ch10));
  leg10->AddEntry(h10,Form("efficiency = %5.3f",eff10));
  leg10->Draw();
  */

  TPaveText *leg101=new TPaveText(0.25,0.45,0.55,0.7,"NDC");
  leg101->SetFillColor(10);
  leg101->SetTextSize(0.035);
  leg101->AddText("Average deflection for - #theta_{c} < #theta_{in} < #theta_{c}");
  leg101->Draw();
  leg101->SetTextFont(132);
  leg101->AddText(Form("Mean = %.2f #murad",par_ch10[1]));
  leg101->AddText(Form("Sigma = %.2f #murad",par_ch10[2]));
  //lg51->AddLine(.0,0.4,1.,0.4);
  leg101->AddLine(.0,0.25,1.,0.25);
  //lg51->SetTextFont(22);
  //lg51->AddText(Form("Integration range channeled particles: %.1f<#Delta#theta_{x}<%.1f #murad",ch_min5,ch_max5));
  //lg51->SetTextFont(132);
  leg101->AddText(Form("Channeling efficiency = %.1f %c",eff10*100.0,'%')); 
  leg101->Draw();

  c101->Update();
  double miny10 = gPad->GetUymin();
  double maxy10 = gPad->GetUymax();


  TBox *b10=new TBox(ch_min10,miny10,ch_max10,maxy10);
  b10->SetFillColor(kGreen);
  b10->SetFillStyle(3002);
    b10->Draw("same");

  /*------------------------*/


 
  c101->Write("eff_10");
  c51->Write("eff_5");
  cAM->Write("fitAM");
  cVR->Write("fitVR");


} 










