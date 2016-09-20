#include "func_sim.h"

void Comphist(){

  TFile * in_Exp = new TFile("QMP29Rob_Exp.root");

  TFile * in_Sim = new TFile("QMP29Rob_Exp_old.root");


  TH1D *h10e = (TH1D*) in_Exp->Get("h10");
  TH1D *h10s = (TH1D*) in_Sim->Get("h10");
  
  Float_t ne=h10e->GetEntries();
  h10e->Scale(1./ne);

 Float_t ns=h10s->GetEntries();
  h10s->Scale(1./ns);
  /*
  h10e->Rebin(5);
  h10s->Rebin(5);
  */
  h10e->Draw();
  h10s->Draw("same");

}
