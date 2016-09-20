#include"func_sim.h"
#include"func_sim.c"

void H8_style_study(){
  
  char* filename;
  Int_t nhead,dim,i;
  Float_t *xp_i,*yp_i,*xp_o,*yp_o;

  filename="recoDataSimple_889.torsion.correction.histo.dat";
  nhead=0;

  dim=count_line(filename);

  xp_i=(Float_t *)malloc((dim-nhead)*sizeof(Float_t)); 
  yp_i=(Float_t *)malloc((dim-nhead)*sizeof(Float_t)); 
  xp_o=(Float_t *)malloc((dim-nhead)*sizeof(Float_t)); 
  yp_o=(Float_t *)malloc((dim-nhead)*sizeof(Float_t));

  extr_var_float(filename,dim,xp_i,1,nhead); 
  extr_var_float(filename,dim,yp_i,2,nhead); 
  extr_var_float(filename,dim,xp_o,3,nhead); 
  extr_var_float(filename,dim,yp_o,4,nhead); 

  gStyle->SetPalette(1);
  //gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  /*--------------------*/

  TCanvas *H8 = new TCanvas("H8","H8 style",1);
  H8->cd();

  TPad *padH8 = new TPad("padH8","",0,0,1,1);
  padH8->SetFillColor(10);
  padH8->Draw();
  padH8->cd();
  padH8->SetGridx(1);
  padH8->SetGridy(1);


  TH2F *hH8=new TH2F("hH8","histo H8 style",100,-50,50,250,-50,200);

  for(i=0;i<dim-nhead;i++)
      hH8->Fill(xp_i[i],xp_o[i]-xp_i[i]);


  hH8->GetXaxis()->SetLabelFont(132);
  hH8->GetYaxis()->SetLabelFont(132);

  hH8->GetXaxis()->SetTitle("#theta_{in} [#murad]");
  hH8->GetYaxis()->SetTitle("#theta_{out}-#theta_{in} [#murad]");
  hH8->GetXaxis()->SetTitleFont(132);
  hH8->GetYaxis()->SetTitleFont(132);

  //hH8->GetZaxis()->SetTitleFont(132);
  //TPaletteAxis()->SetLabelFont(132);

  //TPaletteAxis()::palette->SetLabelFont(132);
  TGaxis::SetMaxDigits(3);
  //TGaxis::SetLabelFont(132);

  hH8->Draw("CONTZ");

  TFile *f=new TFile("STF50Rob_Exp.root","new");

  hH8->Write();

  f->Close();

  /*--------------------*/

  TCanvas *cin0 = new TCanvas("cin0","kick distr AM",1);
  cin0->cd();
  
  TPad *padcin0 = new TPad("padcin0","",0,0,1,1);
  padcin0->SetFillColor(10);
  padcin0->Draw();
  padcin0->cd();

  TH1F *in0=new TH1F("in0","kick AM",100,-50,50);

  in0->GetXaxis()->SetTitle("kick [#murad]");

  //in0->Draw();

      /*------------------------*/
  
  TCanvas *cin1 = new TCanvas("cin1","kick distr VR",1);
  cin1->cd();
  
  TPad *padcin1 = new TPad("padcin1","",0,0,1,1);
  padcin1->SetFillColor(10);
  padcin1->Draw();
  padcin1->cd();

  TH1F *in1=new TH1F("in1","kick VR",70,-50,20);

  in1->GetXaxis()->SetTitle("kick [#murad]");

  //in1->Draw();

      /*------------------------*/
  
  TCanvas *cin2 = new TCanvas("cin2","kick distr CH & DC",1);
  cin2->cd();
  
  TPad *padcin2 = new TPad("padcin2","",0,0,1,1);
  padcin2->SetFillColor(10);
  padcin2->Draw();
  padcin2->cd();

  TH1F *in2=new TH1F("in2","kick CH",230,-30,200);

  in2->GetXaxis()->SetTitle("kick [#murad]");

       /*------------------------*/

  for(i=0;i<dim-nhead;i++){

    if(xp_i[i]>(-30.0) && xp_i[i]<(-20.0))
      in0->Fill(xp_o[i]-xp_i[i]);

    if(xp_i[i]>(20.0) && xp_i[i]<(30.0))
      in1->Fill(xp_o[i]-xp_i[i]);

    if(xp_i[i]>(-10.0) && xp_i[i]<(10.0))
      in2->Fill(xp_o[i]-xp_i[i]);

  }

      /*------------------------*/

  padcin0->cd();
  in0->Draw();
  
  padcin1->cd();
  in1->Draw();
  
  padcin2->cd();
  in2->Draw();




  free(xp_i);
  free(xp_o);
  free(yp_i);
  free(yp_o);

}
