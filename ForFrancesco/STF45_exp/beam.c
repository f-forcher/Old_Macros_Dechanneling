#include"func_sim.h"
#include"func_sim.c"

void beam(){

  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TGaxis::SetMaxDigits(3);

 TFile * in_file = new TFile("recoDataSimple_411.alignment.histo.root");

  TCanvas *H8 = new TCanvas("be","beam",1);
  H8->cd();

  TPad *padH8 = new TPad("padH8","",0,0,1,1);
  padH8->SetFillColor(10);
  padH8->Draw();
  padH8->cd();
  padH8->SetGridx(1);
  padH8->SetGridy(1);


  //TH1D * h = (TH1D*) in_file->Get("d0x_h");
  //TH1D * h = (TH1D*) in_file->Get("d0y_h");
  //TH1D * h = (TH1D*) in_file->Get("ThetaIn_x");
  TH1D * h = (TH1D*) in_file->Get("ThetaIn_y");
  h->GetXaxis()->SetLabelFont(132);
  h->GetYaxis()->SetLabelFont(132);
  h->GetZaxis()->SetLabelFont(132);

  //h->GetXaxis()->SetTitle("x_{in} [mm]");
  h->GetXaxis()->SetTitle("#theta_{in} [#murad]");
  h->GetYaxis()->SetTitle("Entries");
  h->GetXaxis()->SetTitleFont(132);
  h->GetYaxis()->SetTitleFont(132);
  h->GetXaxis()->SetTitleSize(0.05);
  h->GetYaxis()->SetTitleSize(0.05);
  h->GetXaxis()->SetLabelSize(0.05);
  h->GetYaxis()->SetLabelSize(0.05);
  h->GetXaxis()->SetTitleOffset(0.85);
  h->GetYaxis()->SetTitleOffset(0.7);
  h->Draw();

  TPaveText *leg = new TPaveText(0.6,0.75,0.95,0.95,"NDC");
  //leg->AddText("Horizontal profile H8 beam");
  //leg->AddText("Vertical profile H8 beam");
  //leg->AddText("Horizontal divergence H8 beam");
  leg->AddText("Vertical divergence H8 beam");
  leg->AddLine(0.0,0.5,1.0,0.5);
  //leg->AddText("#sigma = 0.959 #pm 0.002 mm");
  //leg->AddText("#sigma = 0.719 #pm 0.001 mm");
  //leg->AddText("#sigma = 10.65 #pm 0.02 #murad");
  leg->AddText("#sigma = 7.68 #pm 0.01 #murad");
  leg->SetFillColor(10);
  leg->SetTextFont(132);
  leg->SetTextSize(0.05);
  leg->Draw();



}
