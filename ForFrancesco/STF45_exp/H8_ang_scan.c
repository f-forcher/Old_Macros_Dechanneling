#include"func_sim.h"
#include"func_sim.c"

void H8_ang_scan(){

  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TGaxis::SetMaxDigits(3);

 TFile * in_file = new TFile("recoDataSimple_408_1-73.torsion.correction.histo.root");

  TCanvas *H8 = new TCanvas("H8","H8 style",1);
  H8->cd();

  TPad *padH8 = new TPad("padH8","",0,0,1,1);
  padH8->SetFillColor(10);
  padH8->Draw();
  padH8->cd();
  padH8->SetGridx(1);
  padH8->SetGridy(1);


  TH2F * h = (TH2F*) in_file->Get("dTheta_x_vs_Impact_x_cor_w");
  h->GetXaxis()->SetLabelFont(132);
  h->GetYaxis()->SetLabelFont(132);
  h->GetZaxis()->SetLabelFont(132);

  h->GetXaxis()->SetTitle("#theta_{in} [#murad]");
  h->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  h->GetXaxis()->SetTitleFont(132);
  h->GetYaxis()->SetTitleFont(132);
  h->Draw("contz");

}
