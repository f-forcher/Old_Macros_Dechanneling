//#include "librerie.h"
#include "func_sim.h"

Double_t myfunction(Double_t *x, Double_t *par) {
  Float_t xx = x[0];
  Double_t f = par[0]*exp(-xx/par[1]);
  return f;
}

Double_t erf(Double_t *x, Double_t *par) {
  return  par[0]*TMath::Erf(x[0])+par[1];
  }


void ch_spot() {


  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  TGaxis::SetMaxDigits(3);

  //Apre file dati
  
  TFile * in_file = new TFile("recoDataSimple_415_1-37.torsion.correction.histo.root");
  //TFile * in_file = new TFile("recoDataSimple_408_1-73.torsion.correction.histo.root");
  //Salva plot
  //TString outputfile = "recoDataSimple_415.peak.graph.root"; 
  

  //TFile * in_file = new TFile("STF45Rob_Exp.root");
  TString outputfile = "STF45Rob.chspot_Exp_new.root"; 

  //xtal length and curvature
  Double_t z = 2e-3; 
  Double_t R;

  //Selezione histogramma
  
  TH2D * h = (TH2D*) in_file->Get("dTheta_x_vs_Impact_x_cor");
  //TH1D * ht = (TH1D*) in_file->Get("dTheta_x"); 
  

  //TH2F * h = (TH2F*) in_file->Get("hH8");
  TH1D *ht= h->ProjectionY();

  TCanvas *c = new TCanvas();
  h->Draw("colz");
 
  TCanvas *c1 = new TCanvas();
  ht->Draw();
  

  //test fit  
  Double_t par_ch[3]=0;
  TF1 *g    = new TF1("g","gaus",110,180);
  g->SetLineColor(kRed); 
  ht->Fit(g,"R");
  g->GetParameters(&par_ch[0]);
  R = (z / (par_ch[1]*1e-6));
  cout << R << endl;

  Double_t min_ch = par_ch[1]-(par_ch[2]*3.);
  Double_t max_ch = par_ch[1]+(par_ch[2]*3.);

  Double_t par_de[3]=0;
  TF1 *de = new TF1("de","expo",20,110);
  de->SetLineColor(kGreen+2); 
  //de->SetParameters(1,1);
  ht->Fit(de,"R+");
  de->GetParameters(&par_de[0]);

  cout << par_de[0] << " " << par_de[1] << endl;


  Double_t zero = exp(par_de[0]);
  Double_t uno = -(1/par_de[1]);

  cout << zero << " " << uno << endl;



  //creazione istogrammi
  const int nc = 40;  
  TH1D **h_slice_ch;

  h_slice_ch=(TH1D **)malloc(nc*sizeof(TH1D *));
  // l_x=(Int_t *)malloc(n*sizeof(Int_t));

  TAxis *axis1 = h->GetXaxis();
  Double_t *ang;
  Double_t *err_ang;
  ang = (double *) malloc(nc*sizeof(double));
  err_ang = (double *) malloc(nc*sizeof(double));
  int minch(0),maxch(0);



  for(int i=0;i<nc;i++) {

    minch = axis1->FindBin(-21+(1*i));
    maxch = axis1->FindBin(-19+(1*i));
    cout << minch << "  " << maxch << endl;
    ang[i] = (-20+(1*i))/10.;//axis->GetBinCenter(-10+(1*i));
    err_ang[i] = (0.05);  
    //ang[i] = (-20+(1*i));//axis->GetBinCenter(-10+(1*i));
    //err_ang[i] = (0.5);  
    h_slice_ch[i]=(TH1D *)malloc(nc*sizeof(TH1D));
    h_slice_ch[i] = h->ProjectionY(Form("cut %d > Impact angle x > %d #murad",(-21+(i*1)),(-19+(i*1))),minch,maxch);
    h_slice_ch[i]->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
    h_slice_ch[i]->SetLineColor(i);
    //l_x[i] = 0;
  }

 

  //definizione variabili
  Double_t *ch_mean;
  ch_mean = (double *) malloc(nc*sizeof(double));
  Double_t *ch_sigma;
  ch_sigma = (double *) malloc(nc*sizeof(double));  

  Double_t *ch_err;
  ch_err = (double *) malloc(nc*sizeof(double));
  Double_t *sigma_err;
  sigma_err = (double *) malloc(nc*sizeof(double));  


  TF1 *g2 = new TF1("g2","gaus",110,180);
  g2->SetParameter(1,-1.);
  double par_ch_l[3];
  Double_t sigma_lin(0),int_min(0),int_max(0);
  Int_t max3(0),min(0);



  //fit ed estrazione dei parameteri
  for(i=0;i<nc;i++) {
    h_slice_ch[i]->Fit(g2,"QR+");   
    g2->GetParameters(&par_ch_l[0]);
    ch_mean[i]=par_ch_l[1];
    ch_sigma[i]=par_ch_l[2];
    sigma_err[i]=g2->GetParError(2);

    if (i == 20 ) {
      sigma_lin = par_ch_l[2];
    }

    max3 = ch_mean[i] + (3*sigma_lin[i]);
    min3 = ch_mean[i] - (3*sigma_lin[i]);

    int_min = h_slice_ch[i]->GetXaxis()->FindBin(min3);
    int_max = h_slice_ch[i]->GetXaxis()->FindBin(max3);
    
    h_slice_ch[i]->GetXaxis()->SetRange(int_min,int_max);
    ch_err[i]=h_slice_ch[i]->GetMeanError();//TMath::Sqrt(h_slice_ch[i]->Integral());//par_ch[2];
    cout << i << " " << ch_mean[i] << "  " << ch_sigma[i] << " " << ang[i] << " " << h_slice_ch[i]->Integral() <<  endl;

    h_slice_ch[i]->GetXaxis()->SetRange();

  }


  TCanvas *cslice = new TCanvas();
  cslice->SetFillColor(41);
  for(i=0;i<nc;i++) {
    h_slice_ch[i]->Draw("same");
  }
 
  //produzione plot
  
  TCanvas *cch = new TCanvas("cch","CH Peak Position",1100,1100);
  cch->SetFillColor(10);
  TGraphErrors *gr_lin_ch = new TGraphErrors(nc,ang,ch_mean,err_ang,ch_err);
  //gr_lin_ch->SetTitle("Peak position in Channeling zone");
  //gr_lin_ch->GetXaxis()->SetTitle("#theta_{x}^{in}/#theta_{c}");
  gr_lin_ch->GetXaxis()->SetTitle("#theta_{x}^{in}/#theta_{c} ");
  gr_lin_ch->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  gr_lin_ch->GetXaxis()->SetTitleFont(132);
  gr_lin_ch->GetYaxis()->SetTitleFont(132);
  gr_lin_ch->GetXaxis()->SetLabelFont(132);
  gr_lin_ch->GetYaxis()->SetLabelFont(132);
  gr_lin_ch->SetMarkerColor(kRed);
  gr_lin_ch->SetMarkerStyle(20);
  //gr_lin_ch->GetXaxis()->SetRangeUser(-1,1);
  //Double_t par_lin[3];
  //TF1 *chl = new TF1("chl","pol1",-10,10);
  //gr_lin_ch->Fit(chl,"R");
  //chl->GetParameters(&par_lin[0]);
  gr_lin_ch->Draw("ap");


  TPaveText *legP=new TPaveText(0.65,0.85,0.95,0.95,"NDC");
  legP->SetFillColor(10);
  legP->SetTextFont(132);
  
  legP->AddText("Mean deflection of channeled particles");
  legP->SetTextSize(0.035);
  legP->Draw();

  TCanvas *csig = new TCanvas("csig","CH Peak Position",1100,1100);
  csig->SetFillColor(10);
  TGraphErrors *gr_sig = new TGraphErrors(nc,ang,ch_sigma,err_ang,sigma_err);
  gr_sig->SetTitle("Sigma Peak in Channeling zone");
  //gr_sig->GetXaxis()->SetTitle("#theta_{x}^{in}/#theta_{c}");
  gr_sig->GetXaxis()->SetTitle("#theta_{x}^{in}/#theta_{c}");
  gr_sig->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  gr_sig->GetXaxis()->SetTitleFont(132);
  gr_sig->GetYaxis()->SetTitleFont(132);
  gr_sig->GetXaxis()->SetLabelFont(132);
  gr_sig->GetYaxis()->SetLabelFont(132);

  //Double_t par_lin[3];
  //TF1 *chl = new TF1("chl","pol1",-10,10);
  //gr_lin_ch->Fit(chl,"R");
  //chl->GetParameters(&par_lin[0]);
  gr_sig->SetMarkerColor(kRed);
  gr_sig->SetMarkerStyle(20);
  gr_sig->Draw("ap");

  TPaveText *legS=new TPaveText(0.6,0.85,0.95,0.95,"NDC");
  legS->SetFillColor(10);
  legS->SetTextFont(132);
  legS->SetTextSize(0.035);
  legS->AddText("Sigma mean deflection of channeled particles");

  legS->Draw();


  TFile *outfile = new TFile(outputfile,"RECREATE");
 cch->Write("mean_peak");
 csig->Write("sigma_peak");
 gr_lin_ch->Write("mpeak");
 gr_sig->Write("speak");
 outfile->Close();
}
