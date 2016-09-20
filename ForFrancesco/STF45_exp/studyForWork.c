#include"func_sim.h"
#include"func_sim.c"
/*
Double_t gauss_peak(Double_t *x, Double_t *par){

  return par[0]*TMath::Exp(-(x[0]-par[1])*(x[0]-par[1])/(2*par[2]*par[2]))
*/
void studyForWork(){

  char* filename;
  Int_t nhead,dim,i;
  Float_t *xp_i,*xp_o;

  //filename="FirstImpacts.dat_total.dat";
  //nhead=1;

  filename="recoDataSimple_415.torsion.correction.histo.dat";
  nhead=0;

  dim=count_line(filename);

  xp_i=(Float_t *)malloc((dim-nhead)*sizeof(Float_t)); 
  xp_o=(Float_t *)malloc((dim-nhead)*sizeof(Float_t)); 

  extr_var_float(filename,dim,xp_i,1,nhead); 
  extr_var_float(filename,dim,xp_o,3,nhead); 

  /*--------------*/

  Float_t fat_molt=1.0;

  TCanvas *cin0 = new TCanvas("cin0","Deflection VS theta_in",1);
  cin0->cd();
  
  TPad *padcin0 = new TPad("padcin0","",0,0,1,1);
  padcin0->SetFillColor(10);
  padcin0->SetLogz(1);
  padcin0->SetGridx(1);
  padcin0->SetGridy(1);
  padcin0->Draw();
  //padcin0->cd();

  gStyle->SetPalette(1);
  //gStyle->SetOptStat(0);
  //gStyle->SetOptTitle(0);

  /*--------------*/

  TCanvas *cin1 = new TCanvas("cin1","Histo deflections 5urad cuts",1);
  cin1->cd();
  
  TPad *padcin1 = new TPad("padcin1","",0,0,1,1);
  padcin1->SetFillColor(10);
  padcin1->SetLogz(1);
  padcin1->SetGridx(1);
  padcin1->SetGridy(1);
  padcin1->Draw();
  //padcin1->cd();

  /*--------------*/


  TCanvas *cin2 = new TCanvas("cin2","Histo deflections 10urad cuts",1);
  cin2->cd();
  
  TPad *padcin2 = new TPad("padcin2","",0,0,1,1);
  padcin2->SetFillColor(10);
  padcin2->SetLogz(1);
  padcin2->SetGridx(1);
  padcin2->SetGridy(1);
  padcin2->Draw();
  //padcin2->cd();

  /*--------------*/

  TH2F *hH8=new TH2F("hH8","histo H8 style",100,-50,50,250,-50,200);
  /*
  for(i=0;i<dim-nhead;i++)
      hH8->Fill(-xp_i[i]*fat_molt,-(xp_o[i]-xp_i[i])*fat_molt);
  */
  hH8->GetXaxis()->SetLabelFont(132);
  hH8->GetYaxis()->SetLabelFont(132);

  hH8->GetXaxis()->SetTitle("#theta_{in} [#murad]");
  hH8->GetYaxis()->SetTitle("#theta_{out}-#theta_{in} [#murad]");
  //hH8->GetZaxis()->SetTitle("Entries");
  hH8->GetXaxis()->SetTitleFont(132);
  hH8->GetYaxis()->SetTitleFont(132);
  //hH8->GetZaxis()->SetTitleFont(132);
  hH8->GetZaxis()->SetLabelFont(132);

  // hH8->GetZaxis()->SetLabelOffset(0.05);
  //hH8->GetZaxis()->SetTitleOffset(0.5);
  //hH8->GetZaxis()->CenterTitle(1);

  TGaxis::SetMaxDigits(3);
 
  //hH8->Draw("CONTZ");

  /*--------------*/

  TH1F *def5=new TH1F("def5","def histo 5urad",62,-50,200);

  def5->GetXaxis()->SetLabelFont(132);
  def5->GetYaxis()->SetLabelFont(132);
  def5->GetXaxis()->SetTitle("Entries");
  def5->GetXaxis()->SetTitle("#theta_{out}-#theta_{in} [#murad]");
  def5->GetXaxis()->SetTitleFont(132);
  def5->GetYaxis()->SetTitleFont(132);

  /*--------------*/

  TH1F *def10=new TH1F("def10","def histo 10urad",62,-50,200);

  def10->GetXaxis()->SetLabelFont(132);
  def10->GetYaxis()->SetLabelFont(132);
  def10->GetXaxis()->SetTitle("Entries");
  def10->GetXaxis()->SetTitle("#theta_{out}-#theta_{in} [#murad]");
  def10->GetXaxis()->SetTitleFont(132);
  def10->GetYaxis()->SetTitleFont(132);

  /*--------------*/

  for(i=0;i<dim-nhead;i++){

    hH8->Fill(xp_i[i]*fat_molt,(xp_o[i]-xp_i[i])*fat_molt);

    if(xp_i[i]*fat_molt > -5.0 && xp_i[i]*fat_molt < 5.0)
      def5->Fill((xp_o[i]-xp_i[i])*fat_molt);

    if(xp_i[i]*fat_molt > -10.0 && xp_i[i]*fat_molt < 10.0)
      def10->Fill((xp_o[i]-xp_i[i])*fat_molt);

  }

  padcin0->cd();
  hH8->Draw("CONTZ");
  /*
  padcin1->cd();
  def5->Draw();

  padcin2->cd();
  def10->Draw();
  */

  padcin1->cd();
   
  TAxis *axis = hH8->GetXaxis();

  int min5 = axis->FindBin(-5.);
  int max5 = axis->FindBin(5.); 

  TH1D *h5= hH8->ProjectionY("cut +/- 5 urad",min5,max5);
  h5->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  h5->Rebin(4);  

  h5->Draw();

  /*--------------*/
  /*
  padcin1->cd();

  TF1 *g1=new TF1("ga1","gaus(0)",100,200);

  g1->SetParameters(1.0,150.0,10.0);

  def5->Fit(g1,"R");

  g1->SetNpx(1000);
  g1->SetLineColor(kRed);
  g1->Draw("same");

  Float_t sigma1,mean1;

  mean1=g1->GetParameter(1);
  sigma1=g1->GetParameter(2);

  cout<<mean1<<" "<<sigma1<<endl;

  Float_t chan5,tot5,eff5;

  chan5=0.0;
  tot5=0.0;
  eff5=0.0;

  cout<<mean1-3.0*sigma1<<" "<<mean1+3.0*sigma1<<endl;

  for(i=0;i<dim-nhead;i++){

    if(xp_i[i]*fat_molt > -5.0 && xp_i[i]*fat_molt < 5.0){

      tot5+=1.0;

      if((xp_o[i]-xp_i[i])*fat_molt > (mean1-3.0*sigma1) && (xp_o[i]-xp_i[i])*fat_molt < (mean1+3.0*sigma1)){

	chan5+=1.0;

      }

    }

  }

  eff5=chan5/tot5;

  cout<<eff5<<endl;

  /*--------------*/
  /*
  padcin2->cd();

  TF1 *g2=new TF1("ga1","gaus(0)",100,200);

  g2->SetParameters(1.0,150.0,10.0);

  def10->Fit(g2,"R");

  g2->SetNpx(1000);
  g2->SetLineColor(kRed);
  g2->Draw("same");

  Float_t sigma2,mean2;

  mean2=g2->GetParameter(1);
  sigma2=g2->GetParameter(2);

  cout<<mean2<<" "<<sigma2<<endl;

  Float_t chan10,tot10,eff10;

  chan10=0.0;
  tot10=0.0;
  eff10=0.0;

  cout<<mean2-3.0*sigma2<<" "<<mean2+3.0*sigma2<<endl;

  for(i=0;i<dim-nhead;i++){

    if(xp_i[i]*fat_molt > -10.0 && xp_i[i]*fat_molt < 10.0){

      tot10+=1.0;

      if((xp_o[i]-xp_i[i])*fat_molt > (mean2-3.0*sigma2) && (xp_o[i]-xp_i[i])*fat_molt < (mean2+3.0*sigma2)){

	chan10+=1.0;

      }

    }

  }

  eff10=chan10/tot10;

  cout<<eff10<<endl;

	


  /*--------------*/

  free(xp_i);
  free(xp_o);

}
