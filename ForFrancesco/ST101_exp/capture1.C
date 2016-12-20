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
 

  
  
  TCanvas *c1 = new TCanvas();
  ht->Draw();
  
  
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

  cout << zero << " " << uno << endl;

  // istogrammi




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

  /*

  TCanvas *ctest = new TCanvas("cch","CH Peak Position");
  TGraphErrors *gr_lin_test = new TGraphErrors(10,imp,kick,imp_err,kick_err);
  gr_lin_test->SetTitle("Peak position in Channeling zone");
  gr_lin_test->GetXaxis()->SetTitle("Impact angle #theta_{x} [#murad]");
  gr_lin_test->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  gr_lin_test->Draw("ap");
  */
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

 /*TLine *vc_dw = new TLine(20,media1,140,media1);
 vc_dw->SetLineStyle(1);
 vc_dw->SetLineWidth(7);
 vc_dw->SetLineColor(2);
 vc_dw->Draw("same");
 /*
 double p1 = ((20)*(par_lin[1])+(par_lin[0])+sigma_lin*3);
 double p2 = ((140)*(par_lin[1])+(par_lin[0])+sigma_lin*3);
 double p3 = ((140)*(par_lin[1])+(par_lin[0])-sigma_lin*3);
 double p4 = ((20)*(par_lin[1])+(par_lin[0])-sigma_lin*3);

 Int_t p = 5;
 Double_t x[5] = {20,140,140,20,20};
 Double_t y[5] = {p1,p2,p3,p4,p1};

 TPolyLine *quad = new TPolyLine{p,x,y};
 //quad->SetFillColor(2);
 //quad->SetFillStile(1001);

 quad->Draw("same");
 */

 /*
 TLine *up_vc = new TLine(-50,((-50)*(-1.078)+(par_lin[0])+sigma_lin*3),200,((200)*(-1.078)+(par_lin[0])+sigma_lin*3));
 up_vc->Draw("same");
 TLine *dw_vc = new TLine(-50,((-50)*(-1.078)+(par_lin[0])-sigma_lin*3),200,((200)*(-1.078)+(par_lin[0])-sigma_lin*3));
 dw_vc->Draw("same");
 */
 
  /*
  TCanvas *cslice = new TCanvas();
  cslice->SetFillColor(41);
  for(i=0;i<n;i++) {
    h_slice[i]->Draw("same");
  }

  TCanvas *test = new TCanvas();
  h_slice[6]->Draw();
  */
  /*
  TAxis *axis = h->GetXaxis();
  
  int min1 = axis->FindBin(-10.);
  int max1 = axis->FindBin(-8.); 

  int min2 = axis->FindBin(-8.);
  int max2 = axis->FindBin(-6.); 

  int min3 = axis->FindBin(-6.);
  int max3 = axis->FindBin(-4.); 

  int min4 = axis->FindBin(-4.);
  int max4 = axis->FindBin(-2.); 

  int min5 = axis->FindBin(-2.);
  int max5 = axis->FindBin(0.); 

  int min6 = axis->FindBin(0.);
  int max6 = axis->FindBin(2.); 

  int min7 = axis->FindBin(2.);
  int max7 = axis->FindBin(4.); 

  int min8 = axis->FindBin(4.);
  int max8 = axis->FindBin(6.); 

  int min9 = axis->FindBin(6.);
  int max9 = axis->FindBin(8.); 

  int min10 = axis->FindBin(8.);
  int max10 = axis->FindBin(10.); 

  cout << "bin cut" << endl;
 
  cout << "  -5 -> "<< min5 <<"   5 -> "<< max5 << endl;
  cout << " -10 -> "<< min10 <<"  10 -> "<< max10 << endl;

  TH1D *h1= h->ProjectionY("cut (-10,-8) #murad",min1,max1);
  h1->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  h1->SetLineColor(kBlue);
  TH1D *h2= h->ProjectionY("cut (-8,-6) #murad",min2,max2);
  h2->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  h2->SetLineColor(kBlue+2);  
  TH1D *h3= h->ProjectionY("cut (-6,-4) #murad",min3,max3);
  h3->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  h3->SetLineColor(kBlue+4);
  TH1D *h4= h->ProjectionY("cut (-4,-2) #murad",min4,max4);
  h4->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  h4->SetLineColor(kBlue+6);
  TH1D *h5= h->ProjectionY("cut (-2,0) #murad",min5,max5);
  h5->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  h5->SetLineColor(kRed);
  TH1D *h6= h->ProjectionY("cut (0,2) #murad",min6,max6);
  h6->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  h6->SetLineColor(kRed+2);
  TH1D *h7= h->ProjectionY("cut (2,4) #murad",min7,max7);
  h7->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  h7->SetLineColor(kGreen);
  TH1D *h8= h->ProjectionY("cut (4,6) #murad",min8,max8);
  h8->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  h8->SetLineColor(kGreen+2);
  TH1D *h9= h->ProjectionY("cut (6,8) #murad",min9,max9);
  h9->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  h9->SetLineColor(kGreen+4);
  TH1D *h10= h->ProjectionY("cut (8,10) #murad",min10,max10);
  h10->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  h10->SetLineColor(kGreen+6);

 //plot e fit

 
  TCanvas *c5 = new TCanvas();
  h1->Draw();
  h2->Draw("same");
  h3->Draw("same");
  h4->Draw("same");
  h5->Draw("same");
  h6->Draw("same");
  h7->Draw("same");
  h8->Draw("same");
  h9->Draw("same");
  h10->Draw("same");

 TCanvas *c2 = new TCanvas();
  h1->Draw();
  h2->Draw("same");
  h3->Draw("same");
TCanvas *c3 = new TCanvas();
  h4->Draw("same");
  h5->Draw("same");
  h6->Draw("same");
  h7->Draw("same");
TCanvas *c4 = new TCanvas();
  h8->Draw("same");
  h9->Draw("same");
  h10->Draw("same");

  */


  /*
  TCanvas *c5 = new TCanvas();
  h5->Draw();
 
  Double_t par_vr5[3]=0;
  Double_t par_ch5[3]=0;
  TF1 *g5    = new TF1("g5","gaus",min_ch,max_ch);
  TF1 *g5am    = new TF1("g5am","gaus",-50,50);
  
  g5->SetLineColor(kRed); 
  g5am->SetLineColor(kYellow); 
  h5->Fit(g5,"R");
  h5->Fit(g5am,"R+");

  g5->GetParameters(&par_ch5[0]);
  g5am->GetParameters(&par_am5[0]);
  
  cout << "paramteri fit picco channeling"<<endl;
  cout << par_ch5[2] << endl;
  cout << par_ch5[1] << endl;
  

  cout << "paramteri fit picco amorfo"<<endl;
  cout << par_am5[2] << endl;
  cout << par_am5[1] << endl;

  Double_t ch_min5 = par_ch5[1]-(par_ch5[2]*3);
  Double_t ch_max5 = par_ch5[1]+(par_ch5[2]*3);
  Double_t R5 = (z / (par_ch5[1]*1e-6));

  Double_t am_min5 = par_am5[1]-(par_am5[2]*3);
  Double_t am_max5 = par_am5[1]+(par_am5[2]*3);

  Double_t ch_min51 = par_ch5[1]-(par_ch5[2]*3.5);
  Double_t am_max51 = par_am5[1]+(par_am5[2]*3.5);

  Double_t ch_min52 = par_ch5[1]-(par_ch5[2]*4.);
  Double_t am_max52 = par_am5[1]+(par_am5[2]*4.);


    Double_t par_de5[3]=0;
  TF1 *de5   = new TF1("de5",myfunction,am_max5,ch_min5,2);
  de5->SetLineColor(kGreen); 
  de5->SetParameters(zero,uno);
  h5->Fit(de5,"R+");

 de5->GetParameters(&par_de5[0]);
  cout << "lunghezza di dechanneling " << R5*(par_de5[1]*1e-6) << endl; 

  Double_t par_de51[3]=0;
  TF1 *de51   = new TF1("de51",myfunction,am_max51,ch_min5,2);
  de51->SetLineColor(kGreen+2); 
  de51->SetParameters(zero,uno);
  h5->Fit(de51,"R+");

 de51->GetParameters(&par_de51[0]);
  cout << "lunghezza di dechanneling " << R5*(par_de51[1]*1e-6) << endl; 

  Double_t par_de52[3]=0;
  TF1 *de52   = new TF1("de52",myfunction,am_max52,ch_min5,2);
  de52->SetLineColor(kGreen+4); 
  de52->SetParameters(zero,uno);
  h5->Fit(de52,"R+");
  
  de52->GetParameters(&par_de52[0]);
  cout << "lunghezza di dechanneling " << R5*(par_de52[1]*1e-6) << endl; 

  leg5 = new TLegend(0.1,0.7,0.48,0.9);
  leg5->SetHeader("dechanneling legth at +/- 5 #murad");
  leg5->AddEntry(de5,Form("L_{D} = %5.3f mm",R5*(par_de5[1]*1e-3)));
  leg5->AddEntry(de51,Form("L_{D} = %5.3f mm",R5*(par_de51[1]*1e-3)));
  leg5->AddEntry(de52,Form("L_{D} = %5.3f mm",R5*(par_de52[1]*1e-3)));
  leg5->Draw();



  TCanvas *c10 = new TCanvas();
  h10->Draw();


  Double_t par_am10[3]=0;
  Double_t par_ch10[3]=0;
  TF1 *g10    = new TF1("g10","gaus",min_ch,max_ch);
  TF1 *g10am    = new TF1("g10am","gaus",-50,50);
  g10->SetLineColor(kRed); 
  h10->Fit(g10,"R");
  g10->GetParameters(&par_ch10[0]);
  g10am->SetLineColor(kYellow); 
  h10->Fit(g10am,"R+");
  g10am->GetParameters(&par_am10[0]);
    
  cout << "paramteri fit"<<endl;
  cout << par_ch10[2] << endl;
  cout << par_ch10[1] << endl;
  
  Double_t ch_min10 = par_ch10[1]-(par_ch10[2]*3);
  Double_t ch_max10 = par_ch10[1]+(par_ch10[2]*3);


  cout << "paramteri fit"<<endl;
  cout << par_am10[2] << endl;
  cout << par_am10[1] << endl;
  
  Double_t am_min10 = par_am10[1]-(par_am10[2]*3);
  Double_t am_max10 = par_am10[1]+(par_am10[2]*3);

  Double_t R10 = (z / (par_ch10[1]*1e-6));

  Double_t ch_min101 = par_ch10[1]-(par_ch10[2]*3.5);
  Double_t am_max101 = par_am10[1]+(par_am10[2]*3.5);

  Double_t ch_min102 = par_ch10[1]-(par_ch10[2]*4.);
  Double_t am_max102 = par_am10[1]+(par_am10[2]*4.);

  Double_t par_de10[3]=0;
  TF1 *de10   = new TF1("de10",myfunction,am_max10,ch_min10,2);
  de10->SetLineColor(kGreen); 
  de10->SetParameters(zero,uno);
  h10->Fit(de10,"R+");
  gStyle->SetOptFit(1111);
  
  de10->GetParameters(&par_de10[0]);
  cout << "lunghezza di dechanneling " << R10*(par_de10[1]*1e-6) << endl; 

  Double_t par_de101[3]=0;
  TF1 *de101   = new TF1("de101",myfunction,am_max101,ch_min10,2);
  de101->SetLineColor(kGreen+2); 
  de101->SetParameters(zero,uno);
  h10->Fit(de101,"R+");

 de101->GetParameters(&par_de101[0]);
  cout << "lunghezza di dechanneling " << R10*(par_de101[1]*1e-6) << endl; 

  Double_t par_de102[3]=0;
  TF1 *de102   = new TF1("de102",myfunction,am_max102,ch_min10,2);
  de102->SetLineColor(kGreen+4); 
  de102->SetParameters(zero,uno);
  h10->Fit(de102,"R+");
  
  de102->GetParameters(&par_de102[0]);
  cout << "lunghezza di dechanneling " << R10*(par_de102[1]*1e-6) << endl; 


  leg10 = new TLegend(0.1,0.7,0.48,0.9);
  leg10->SetHeader("dechanneling legth at +/- 10 #murad");
  leg10->AddEntry(de10,Form("L_{D} = %5.3f mm",R10*(par_de10[1]*1e-3)));
  leg10->AddEntry(de101,Form("L_{D} = %5.3f mm",R10*(par_de101[1]*1e-3)));
 leg10->AddEntry(de102,Form("L_{D} = %5.3f mm",R10*(par_de102[1]*1e-3)));
  leg10->Draw();



  */



} 










