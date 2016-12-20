#include "librerie.h"


Double_t myfunction(Double_t *x, Double_t *par) {
  Float_t xx = x[0];
  Double_t f = par[0]*exp(-xx/par[1]);
  return f;
}

Double_t erf(Double_t *x, Double_t *par) {
  return  par[0]*TMath::Erf(x[0])+par[1];
  }

//Volume capture Analysis

void capture() {

  //Apre file dati
  TFile * in_file = new TFile("recoDataSimple_2260.torsion.correction.histo.root");
   
  //xtal length and curvature
  Double_t z = 1.988*1e-3; 
  Double_t R;

  //Select histo
  
  TH2D * h = (TH2D*) in_file->Get("dTheta_x_vs_Impact_x_cor_w");
  //TH1D * ht = (TH1D*) in_file->Get("dTheta_x"); 

  TH1D *ht = h->ProjectionY();
 

  Double_t par_ch[3]=0;
  TF1 *g    = new TF1("g","gaus",120,210);
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
  
  TCanvas *c1 = new TCanvas();
  ht->Draw();
  
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

    minch = axis1->FindBin(-20+(1*i));
    maxch = axis1->FindBin(-19+(1*i));
    cout << minch << "  " << maxch << endl;
    ang[i] = (-20+(1*i));//axis->GetBinCenter(-10+(1*i));
    err_ang[i] = (0.5);  
    h_slice_ch[i]=(TH1D *)malloc(nc*sizeof(TH1D));
    h_slice_ch[i] = h->ProjectionY(Form("cut %d > Impact angle x > %d #murad",(-20+(i*1)),(-19+(i*1))),minch,maxch);
    h_slice_ch[i]->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
    h_slice_ch[i]->SetLineColor(i);
    //l_x[i] = 0;
  }

 


  Double_t *ch_mean;
  ch_mean = (double *) malloc(nc*sizeof(double));
  Double_t *ch_sigma;
  ch_sigma = (double *) malloc(nc*sizeof(double));  

  TF1 *g2 = new TF1("g2","gaus",120,210);
  g2->SetParameter(1,-1.);
  double par_ch_l[3];
  Double_t sigma_lin(0),int_min(0),int_max(0);

  Double_t imp[40];
  Double_t imp_err[40];
  Double_t kick[40];
  Double_t kick_err[40];

  int j = 0;

  for(i=0;i<nc;i++) {
    h_slice_ch[i]->Fit(g2,"QR+");   
    g2->GetParameters(&par_ch_l[0]);
    ch_mean[i]=par_ch_l[1];
    if (i == 20 ) {
      sigma_lin = par_ch_l[2];
    }

    int_min = h_slice_ch[i]->GetXaxis()->FindBin(110);
    int_max = h_slice_ch[i]->GetXaxis()->FindBin(180);
    
    h_slice_ch[i]->GetXaxis()->SetRange(int_min,int_max);
    ch_sigma[i]=h_slice_ch[i]->GetRMS();//TMath::Sqrt(h_slice_ch[i]->Integral());//par_ch[2];
    cout << i << " " << ch_mean[i] << "  " << ch_sigma[i] << " " << ang[i] << " " << h_slice_ch[i]->Integral() <<  endl;
   
    
    if ( h_slice_ch[i]->Integral() > 700 ) {
      imp[j] = (-20+(1*i));
      imp_err[j] = 0.5;
      kick[j] = par_ch_l[1];
      kick_err[j] = h_slice_ch[i]->GetRMS();

      j++;
    }


    if ( h_slice_ch[i]->Integral() > 80 && h_slice_ch[i]->Integral() < 115 ) {
      imp[j] = (-20+(1*i));
      imp_err[j] = 0.5;
      kick[j] = par_ch_l[1];
      kick_err[j] = h_slice_ch[i]->GetRMS();
      j++;
    }
    

    h_slice_ch[i]->GetXaxis()->SetRange();
    cout << h_slice_ch[i]->Integral() << endl;
  }

 TCanvas *slch = new TCanvas();
  h_slice_ch[20]->Draw();

  TCanvas *cslice = new TCanvas();
  cslice->SetFillColor(41);
  for(i=0;i<nc;i++) {
    h_slice_ch[i]->Draw("same");
  }
 
  
  TCanvas *cch = new TCanvas("cch","CH Peak Position");
  TGraphErrors *gr_lin_ch = new TGraphErrors(nc,ang,ch_mean,err_ang,ch_sigma);
  gr_lin_ch->SetTitle("Peak position in Channeling zone");
  gr_lin_ch->GetXaxis()->SetTitle("Impact angle #theta_{x} [#murad]");
  gr_lin_ch->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  
  Double_t par_lin[3];
  TF1 *chl = new TF1("chl","pol1",-10,10);
  gr_lin_ch->Fit(chl,"R");
  chl->GetParameters(&par_lin[0]);
  gr_lin_ch->Draw("ap");

const int n = 120;  
  TH1D **h_slice;
  Int_t *l_x;

  h_slice=(TH1D **)malloc(n*sizeof(TH1D *));
  // l_x=(Int_t *)malloc(n*sizeof(Int_t));


  TAxis *axis = h->GetXaxis();
  Double_t *imp_ang;
  Double_t *err_ang_vr;
  imp_ang = (double *) malloc(n*sizeof(double));
  err_ang_vr = (double *) malloc(n*sizeof(double));
  int min(0),max(0);

  for(int i=0;i<n;i++) {

    min = axis->FindBin(20+(1*i));
    max = axis->FindBin(21+(1*i));
    cout << min << "  " << max << endl;
    imp_ang[i] = (20+(1*i));//axis->GetBinCenter(-10+(1*i));
    err_ang_vr[i] = 0.5;
    h_slice[i]=(TH1D *)malloc(n*sizeof(TH1D));
    h_slice[i] = h->ProjectionY(Form("cut %d > Impact angle x > %d #murad",(20+(i*1)),(21+(i*1))),min,max);
    h_slice[i]->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
    h_slice[i]->SetLineColor(i);
    //l_x[i] = 0;
  }



  Double_t *vr_mean;
  Double_t *vc_eff;
  Double_t *vr_err;

  vr_mean = (double *) malloc(n*sizeof(double));
  vc_eff = (double *) malloc(n*sizeof(double));
  vr_err = (double *) malloc(n*sizeof(double));

  Double_t *int_de;
  Double_t *int_tot;
  Double_t *int_vc;
  Double_t *eff_vc;
  Double_t *err_vc;
  Double_t *err_tot;
  Double_t *err_pop;
 


  eff_vc = (double *) malloc(n*sizeof(double));
  int_vc = (double *) malloc(n*sizeof(double));
  int_de = (double *) malloc(n*sizeof(double));
  int_tot = (double *) malloc(n*sizeof(double));
  err_vc = (double *) malloc(n*sizeof(double));
  err_tot = (double *) malloc(n*sizeof(double));
  err_pop = (double *) malloc(n*sizeof(double));

  TF1 *g1 = new TF1("g1","gaus",-50,50);
  TF1 *g2 = new TF1("g2","gaus",120,210);


  int bvc_min(0), bvc_max(0), tot_min(0), tot_max(0),bvr_min(0), bvr_max(0);;
  double par_vr[3];
  double par_vc[3];
  Double_t par_de5[3]=0;
  Double_t somma, somma1;

  TCanvas *set = new TCanvas();

  for(i=0;i<n;i++) {

    h_slice[i]->Fit(g1,"QR+");
    // h_slice[i]->Fit(g2,"QR+");
   
    g1->GetParameters(&par_vr[0]);
    // g2->GetParameters(&par_vc[0]);

    vr_mean[i]=par_vr[1];
    vr_err[i]=par_vr[2];

    Double_t vr_min = par_vr[1]-(par_vr[2]*3);
    Double_t vr_max = par_vr[1]+(par_vr[2]*3);
    // Double_t vc_max = par_vc[1]+(par_vc[2]*3);
    // Double_t vc_min = par_vc[1]-(par_vc[2]*3);
    //Double_t vc_max = ((20+(1*i))*(par_lin[1])+(par_lin[0]))+(sigma_lin*3);
    //Double_t vc_min = ((20+(1*i))*(par_lin[1])+(par_lin[0]))-(sigma_lin*3);
    Double_t vc_max = ((20+(1*i))*(-1.)+(144.5))+(sigma_lin*3);
    Double_t vc_min = ((20+(1*i))*(-1.)+(144.5))-(sigma_lin*3);

    cout << vc_min << " " << vc_max << " " << vr_max << "("<< par_vr[1]<<","<<par_vr[2]<<")"<< endl;

    somma += vr_max[i];
    somma1 += vr_min[i];

    TAxis *axis = h_slice[i]->GetXaxis();
    /*  if (par_vc[2] < 5. && par_vc[2] > 15.) {
      bvc_min = axis->FindBin(vc_min);
      bvc_max = axis->FindBin(vc_max);
    } else {
      bvc_min = axis->FindBin(20);
      bvc_max = axis->FindBin(400);
      }*/

    bvc_min = axis->FindBin(vc_min);
    bvc_max = axis->FindBin(vc_max);

    bvr_min = axis->FindBin(vr_min);
    bvr_max = axis->FindBin(vr_max);

    tot_min = axis->FindBin(-200);
    tot_max = axis->FindBin(400);
    
    
    if (vc_min > vr_max) {
      int_vc[i] = h_slice[i]->IntegralAndError(bvc_min,bvc_max,err_vc[i]);
      cout << "normal -> "<< endl;
    } else {
      int_vc[i] = h_slice[i]->IntegralAndError(bvr_max,bvc_max,err_vc[i]);
      cout << "crossing bounds -> " << endl;    
      }
    
    //int_vc[i] = h_slice[i]->IntegralAndError(bvr_max,bvc_max,err_vc[i]);

    int_tot[i] = h_slice[i]->IntegralAndError(bvr_min,bvr_max,err_tot[i]);

    // int_de[i] = h_slice[i]->Integral(de_min,de_max);
    //cout<<"channeled event" <<int_ch[i][j]<<endl;
    cout << int_tot[i] << " " << int_vc[i] << endl;


    eff_vc[i] = int_vc[i]/int_tot[i];//274891;//(int_tot[i]);
    //err_pop[i] = eff_vc[i]*TMath::Sqrt(TMath::Power((err_vc[i]/int_vc[i]),2)+TMath::Power((err_tot[i]/int_tot[i]),2));

    err_pop[i] = h_slice[i]->GetRMS()/(h_slice[i]->Integral());	
  }

TCanvas *cgr = new TCanvas();
  TGraphErrors *gr = new TGraphErrors(n,imp_ang,vr_mean,err_ang_vr,vr_err);
  //gr->Draw("ap");
  gr->SetTitle("Peak position in Volume Reflection zone");
  gr->GetXaxis()->SetTitle("Impact angle #theta_{x} [#murad]");
  gr->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  /* TF1 *gerr    = new TF1("gerr",erf,-20.,20.,2);
  //gerr->SetParameter(0,2);
  gerr->SetLineColor(kRed); 
  gr->Fit("gerr");
  */

  gr->Draw("ap");
 

  
  TCanvas *cde = new TCanvas();
  TGraphErrors *grd = new TGraphErrors(n,imp_ang,eff_vc,err_ang_vr,err_pop);
  grd->SetTitle("Volume capture population (normalized to total event)");
  grd->GetXaxis()->SetTitle("Impact angle #theta_{x} [#murad]");
  grd->GetYaxis()->SetTitle("#volume_capture_events / #total_events");
  grd->Draw("ap");
 

  Double_t media =  somma/n;
  Double_t media1 =  somma1/n;


 TCanvas *c = new TCanvas();
 h->Draw("colz");


 
 TLine *up_vc = new TLine(-170,(sigma_lin*3),-30,(130+sigma_lin*3));
 up_vc->Draw("same");
 TLine *dw_vc = new TLine(-170,(-sigma_lin*3),-30,(130-sigma_lin*3));
 dw_vc->Draw("same");
 
 
 //TLine *up_vc = new TLine(20,((20)*(par_lin[1])+(par_lin[0])+sigma_lin*3),140,((140)*(par_lin[1])+(par_lin[0])+sigma_lin*3));
 up_vc->SetLineStyle(1);
 up_vc->SetLineWidth(2);
 up_vc->SetLineColor(4);
 up_vc->Draw("same");
 // TLine *dw_vc = new TLine(20,((20)*(par_lin[1])+(par_lin[0])-sigma_lin*3),140,((140)*(par_lin[1])+(par_lin[0])-sigma_lin*3));
 dw_vc->SetLineStyle(1);
 dw_vc->SetLineWidth(2);
 dw_vc->SetLineColor(4);
 dw_vc->Draw("same");
 TLine *vc_up = new TLine(-180,media,-70,media);
 vc_up->SetLineStyle(1);
 vc_up->SetLineWidth(2);
 vc_up->SetLineColor(2);
 vc_up->Draw("same");
	
}
