#include "librerie.h"

void eff50() {

  //Apre file dati
  TFile * in_file = new TFile("recoDataSimple_2262.torsion.correction.histo.root");
   
 
  //Selezione histogramma
  
  TH2D * h = (TH2D*) in_file->Get("dTheta_x_vs_Impact_x_cor");
  TH1D * h1 = (TH1D*) in_file->Get("dTheta_x"); 

  
  TCanvas *c = new TCanvas("colz");
  h->Draw();

  
  TCanvas *c1 = new TCanvas();
  h1->Draw();
  
  
  Double_t par_ch[3]=0;
  TF1 *g    = new TF1("g","gaus",140,190);
  g->SetLineColor(kRed); 
  h1->Fit(g,"R");
  g->GetParameters(&par_ch[0]);
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

 
  int min2 = axis->FindBin(-20);
  int max2 = axis->FindBin(20);

  int min5 = axis->FindBin(-5.);
  int max5 = axis->FindBin(5.); 

  int min10 = axis->FindBin(-10.);
  int max10 = axis->FindBin(10.); 

  cout << "bin cut" << endl;
  cout << "-20 -> "<< min2 <<" 20 -> "<< max2 << endl;
  cout << "  -5 -> "<< min5 <<"   5 -> "<< max5 << endl;
  cout << " -10 -> "<< min10 <<"  10 -> "<< max10 << endl;

  TH1D *h2= h->ProjectionY("cut +/- 20 urad",min2,max2);
  TH1D *h5= h->ProjectionY("cut +/- 5 urad",min5,max5);
  TH1D *h10= h->ProjectionY("cut +/- 10 urad",min10,max10);

 //plot e fit 

  TCanvas *c2 = new TCanvas();
  h2->Draw();
  Double_t par_ch2[3]=0;
  TF1 *g2    = new TF1("g2","gaus",140,190);
  g2->SetLineColor(kRed); 
  h2->Fit(g2,"R");
  g2->GetParameters(&par_ch2[0]);
  double sigma2_err = g2->GetParError(2);  
  double cost2_err = g2->GetParError(0);

  cout << "paramteri fit"<<endl;
  cout << par_ch2[2] << endl;
  cout << par_ch2[1] << endl;
  cout << sigma2_err << " " << cost2_err << endl;    

  Int_t ch_min2 = par_ch2[1]-(par_ch2[2]*3);
  Int_t ch_max2 = par_ch2[1]+(par_ch2[2]*3);

  cout << "integral bound"<< endl;
  cout << "2.5 lower "<<ch_min2 <<" higher " << ch_max2 << endl;

  TCanvas *c5 = new TCanvas();
  h5->Draw();
  Double_t par_ch5[3]=0;
  TF1 *g5    = new TF1("g5","gaus",140,190);
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
  Double_t par_ch10[3]=0;
  TF1 *g10    = new TF1("g10","gaus",140,190);
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
  cout << "20 lower "<<ch_min2 <<" higher " << ch_max2 << endl;
  cout << "  5 lower "<<ch_min5 <<" higher " << ch_max5 << endl;
  cout << " 10 lower "<<ch_min10 <<" higher " << ch_max10 << endl;

  //integrali
  TAxis *x_axis2 = h2->GetXaxis();
  
  int bmin = x_axis2->FindBin(-200);
  int bmax = x_axis2->FindBin(200); 
    
  cout << "bound" << endl;
  cout << "frist " << bmin << "and last bin "<< bmax << endl;
  
  int bmin_ch2 = x_axis2->FindBin(ch_min2);
  int bmax_ch2 = x_axis2->FindBin(ch_max2); 
 
  double int2_err;
  double int_ch2_err;

  double int2 = h2->IntegralAndError(bmin,bmax,int2_err);    
  //  double int_ch2 = h2->Integral(bmin_ch2,bmax_ch2);
  double int_ch2 = sqrt(2.*TMath::Pi())*par_ch2[2]*par_ch2[0];
  double int_ch2_err = int_ch2*((sigma2_err/par_ch2[2])+(cost2_err/par_ch2[0]));
       
  double eff2 = int_ch2/int2;
  double eff2_err = eff2 * ((int_ch2_err/int_ch2)+(int2_err/int2));

  cout << "angle cut @ +/- 20 urad" << endl;
  cout << "#tot events " << int2 << " #ch events "<< int_ch2 << "  --> efficiency "<<eff2  <<" +/- " << eff2_err<< endl; 


  TAxis *x_axis5 = h5->GetXaxis();
  
  int bmin_ch5 = x_axis5->FindBin(ch_min5);
  int bmax_ch5 = x_axis5->FindBin(ch_max5); 

 double int5_err;
  double int_ch5_err;

  double int5 = h5->IntegralAndError(bmin,bmax,int5_err);    
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
  double int_ch10_err;

  double int10 = h10->IntegralAndError(bmin,bmax,int10_err);    
  //  double int_ch10 = h10->Integral(bmin_ch10,bmax_ch10);
  double int_ch10 = sqrt(2.*TMath::Pi())*par_ch10[2]*par_ch10[0];
  double int_ch10_err = int_ch10*((sigma10_err/par_ch10[2])+(cost10_err/par_ch10[0]));
       
  double eff10 = int_ch10/int10;
  double eff10_err = eff10 * ((int_ch10_err/int_ch10)+(int10_err/int10));

  cout << "angle cut @ +/- 10 urad" << endl;
  cout << "#tot events " << int10 << " #ch events "<< int_ch10 << "  --> efficiency "<<eff10  <<" +/- " << eff10_err<< endl; 



TCanvas *c21 = new TCanvas("c21","c21",1100,1100);
  h2->Draw();
  leg2 = new TLegend(0.1,0.7,0.48,0.9);
  leg2->SetHeader("efficiency at +/- 20 #murad");
  leg2->AddEntry(h2,Form("number of total event = %5.0f",int2));
  leg2->AddEntry(h2,Form("number of channeled event = %5.0f",int_ch2));
  leg2->AddEntry(h2,Form("efficiency = %5.3f",eff2));
  leg2->Draw();

TCanvas *c51 = new TCanvas("c51","c51",1100,1100);
  h5->Draw();
  leg5 = new TLegend(0.1,0.7,0.48,0.9);
  leg5->SetHeader("efficiency at +/- 5 #murad");
  leg5->AddEntry(h5,Form("number of total event = %5.0f",int5));
  leg5->AddEntry(h5,Form("number of channeled event = %5.0f",int_ch5));
  leg5->AddEntry(h5,Form("efficiency = %5.3f",eff5));
  leg5->Draw();

TCanvas *c101 = new TCanvas("c101","c101",1100,1100);
  h10->Draw();
  leg10 = new TLegend(0.1,0.7,0.48,0.9);
  leg10->SetHeader("efficiency at +/- 10 #murad");
  leg10->AddEntry(h10,Form("number of total event = %5.0f",int10));
  leg10->AddEntry(h10,Form("number of channeled event = %5.0f",int_ch10));
  leg10->AddEntry(h10,Form("efficiency = %5.3f",eff10));
  leg10->Draw();

  /*
  //Cerco bordi asse x
  int max = h->GetYaxis()->GetLast(); 
  int min = h->GetYaxis()->GetFirst();   
  Int_t n = h->GetNbinsY(); 

  cout << max << " " << min << " " << n <<  endl; 

  //TVector
  TVectorD int_am(n+1);   
  TVectorD int_ch(n+1);
  TVectorD eff_int(n+1);
  TVectorD eff(n+1);
  TVectorD evt_am(n+1);
  TVectorD evt_ch(n+1);
  TVectorD mean_am(n+1);
  TVectorD sigma_am(n+1);
  TVectorD cost_am(n+1);
  TVectorD mean_ch(n+1);
  TVectorD sigma_ch(n+1);
  TVectorD cost_ch(n+1);
  Double_t par[6]=0;
  TVectorD yaxis(n+1);
  TVectorD yerr(n+1);
  TVectorD xerr(n+1);
  TVectorD cov02_am(n+1); 
  TVectorD cov00_am(n+1);
  TVectorD cov22_am(n+1); 
  TVectorD cov02_ch(n+1); 
  TVectorD cov00_ch(n+1); 
  TVectorD cov22_ch(n+1); 
  TVectorD varevt_am(n+1); 
  TVectorD varevt_ch(n+1);

  */
  //  yaxis[5][2]=20.;
  //  cout << yaxis[5][2] << endl;
  /*
  int i,j;
  cout << "inserire valore slice su x angle" << endl;
  cin >> i ;
  cout << endl;
  cout << "inserire valore slice su y impact" << endl;
  cin >> j ;
  cout << endl;
  
  TCanvas *c2 = new TCanvas();
  TH1D *h= dTheta_x_vs_impact_x_vs_Impact_y->ProjectionX("z slice",j,j,i,i);
  h->Draw();
  */
  
  //ciclo per riempire array  
   
  /*
  for (int i = 0; i <= n; i++) {
    //histo sezione
    TH1D *h1= h->ProjectionX("scan d0_x",i,i);
    cout << i << endl;
    //riempo asse x eff
    yaxis[i]=h->GetYaxis()->GetBinCenter(i);
    yerr[i]=(h->GetYaxis()->GetBinWidth(i))/2.;
    cout << yaxis[i] << "  " << yerr[i] << endl;
    
    /*  
	h1->Draw();
	h1->SetLineColor(kRed);
	h1->SetLineWidth(2.0);  
	
	
	c_p->cd(2);
	TH1D *h2= h->ProjectionX("test",,);
	
	h2->Draw();
	h2->SetLineColor(kBlue);
	h2->SetLineWidth(2.0);  
	
    *
    
    //Cerco bin per Integrali
    
    TAxis *axis = h1->GetXaxis();
    int bmin = axis->FindBin(-200);
    int bmax = axis->FindBin(200); 
    
    int bmin_ch = axis->FindBin(120);
    int bmax_ch = axis->FindBin(165); 

    //integrali
    int_am[i] = h1->Integral(bmin,bmax);   
    int_ch[i] = h1->Integral(bmin_ch,bmax_ch);
      
    eff_int[i] = int_ch[i]/(int_am[i]+int_ch[i]);
    // cout << "numero di eventi integrali in channeling "<< int_ch[i] <<endl;


    /*

      TCanvas *c1 = new TCanvas();
      c1->SetTitle("efficency");
      c1->Divide(2,1);
      c1->cd(1);
      h->Draw();

    *

    if (int_ch[i] > 0) {  
    //fit
    // Double_t par[6]=0;
    //  cout<<"control";
    TF1 *g1    = new TF1("g1","gaus",-50,50);
    TF1 *g2    = new TF1("g2","gaus",120,165);
  
    //TF1 *total = new TF1("total","gaus(0)+gaus(3)",-100,200);
    g1->SetLineColor(kBlue);
    g2->SetLineColor(kRed); 
    g2->SetParLimits(1,120,150);

    TFitResultPtr frp1 = h1->Fit(g1, "SR");
    frp1->Print(); 
    TMatrixDSym covMatrix1 (frp1->GetCovarianceMatrix());
    covMatrix1.Print();

   

    //      cout << "control";
      TFitResultPtr frp2 = h1->Fit(g2, "SR+");
      frp2->Print(); 
      TMatrixDSym covMatrix2 (frp2->GetCovarianceMatrix());
      covMatrix2.Print();
    
    //    h1->Fit(g1,"R");
    //    h1->Fit(g2,"R+");
      g1->GetParameters(&par[0]);
      g2->GetParameters(&par[3]);
    //total->SetParameters(par);
    //c1->cd(1);
    //    h1->Draw();
   
    // h->Fit(total,"R+");
  


  
    //c1->cd(2);
    //total->Draw();

    //stampe varie
    //cout << par[0] <<" "<< par[2] << endl;
    
      mean_am[i] = par[1];
      sigma_am[i] = par[2];
      cost_am[i]= par[0];
      mean_ch[i]= par[4];
      sigma_ch[i]= par[5];
      cost_ch[i]= par[3];
  

      evt_am[i] = sqrt(2.*TMath::Pi())*sigma_am[i]*cost_am[i];
    //cout << evt_am << endl;

    //cout << par[3] <<" "<<par[5] << endl;
      evt_ch[i] = sqrt(2.*TMath::Pi())*sigma_ch[i]*cost_ch[i];
    //cout << evt_ch << endl;

  
      eff[i] = evt_ch[i]/(evt_am[i]+evt_ch[i]);
    //cout << eff << endl;

    //cout << int_am <<" "<< int_ch << endl;
    //cout << int_ch/int_am << endl;
    
      cov02_am[i] = covMatrix1(0,2);
      cov00_am[i] = covMatrix1(0,0);
      cov22_am[i] = covMatrix1(2,2);
      cov02_ch[i] = covMatrix2(0,2);
      cov00_ch[i] = covMatrix2(0,0);
      cov22_ch[i] = covMatrix2(2,2);
      
      varevt_am[i] = sqrt(sqrt(2.*TMath::Pi())*(pow(sigma_am[i],2)*pow(cov00_am[i],2)+pow(cost_am[i],2)*pow(cov22_am[i],2)+2*cov02_am[i]*sigma_am[i]*cost_am[i]));

      varevt_ch[i] = sqrt(sqrt(2.*TMath::Pi())*(pow(sigma_ch[i],2)*pow(cov00_ch[i],2)+pow(cost_ch[i],2)*pow(cov22_ch[i],2)+2*cov02_ch[i]*sigma_ch[i]*cost_ch[i]));

      xerr[i] = sqrt(TMath::Abs(sqrt(2.*TMath::Pi())*(pow(varevt_ch[i],2)/pow(evt_am[i],2)-pow(varevt_am[i],2)*pow(evt_ch[i]/pow(evt_am[i],2),2))));
   
    } else {

      eff[i] = 0;
      xerr[i] = 0;

    }
*/


} 










