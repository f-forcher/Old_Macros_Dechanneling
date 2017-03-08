#include "librerie.h"

void torsion_analysis_scan() {
  ofstream myfile;
  //Apre file dati
  TFile * in_file = new TFile("recoDataSimple_408.torsion.histo.root");
   
  TString outputfile = "recoDataSimple_408.torsion.graph.root"; 


  myfile.open("roottext.txt");
  //Selezione histogramma
  
  TH3D * h = (TH3D*) in_file->Get("dTheta_x_vs_Impact_x_vs_Impact_y");
  TH1D * h1 = (TH1D*) in_file->Get("dTheta_x"); 

  /*
  TCanvas *c2 = new TCanvas();
  h->Draw();

  TCanvas *c1 = new TCanvas();
  h1->Draw();
  */

  Double_t par_ch[3]=0;
  TF1 *g    = new TF1("g","gaus",120,180);
  g->SetLineColor(kRed); 
  h1->Fit(g,"R");
  g->GetParameters(&par_ch[0]);
  
  //cout << "paramteri fit"<<endl;
  //cout << par_ch[2] << endl;
  //cout << par_ch[1] << endl;
  
  Int_t ch_min = par_ch[1]-(par_ch[2]*3);
  Int_t ch_max = par_ch[1]+(par_ch[2]*3);
  
  //cout << "intorno di channeling " << endl;
  //cout << "[" << ch_min << "," << ch_max << "]" << endl; 

 
  //TCanvas *c2 = new TCanvas();
  //h1_vs_Impact_x_nocut.Draw("colz");


 //efficiecy

  int ymax = h->GetYaxis()->GetLast(); 
  int ymin = h->GetYaxis()->GetFirst();   
  Int_t yn = h->GetNbinsY(); 

  //cout << ymax << " " << ymin << " " << yn <<  endl;
  
  int zmax = h->GetZaxis()->GetLast(); 
  int zmin = h->GetZaxis()->GetFirst();   
  Int_t zn = h->GetNbinsZ(); 

  //cout << zmax << " " << zmin << " " << zn <<  endl;


  TMatrixD int_am(yn+1,zn+1);   
  TMatrixD int_ch(yn+1,zn+1);
  TMatrixD eff_int(yn+1,zn+1); 
  TMatrixD eff(yn+1,zn+1);
  TMatrixD evt_am(yn+1,zn+1);
  TMatrixD evt_ch(yn+1,zn+1);
  TMatrixD mean_am(yn+1,zn+1);
  TMatrixD sigma_am(yn+1,zn+1);
  TMatrixD cost_am(yn+1,zn+1);
  TMatrixD mean_ch(yn+1,zn+1);
  TMatrixD sigma_ch(yn+1,zn+1);
  TMatrixD cost_ch(yn+1,zn+1);
  Double_t par[3]=0;
  //TMatrixD yaxis(yn+1,zn+1);
  TVectorD xaxis(yn+1);
  TVectorD yaxis(zn+1);
  TMatrixD yerr(yn+1,zn+1);
  TMatrixD xerr(yn+1,zn+1);
  TMatrixD cov02_am(yn+1,zn+1); 
  TMatrixD cov00_am(yn+1,zn+1);
  TMatrixD cov22_am(yn+1,zn+1); 
  TMatrixD cov02_ch(yn+1,zn+1); 
  TMatrixD cov00_ch(yn+1,zn+1); 
  TMatrixD cov22_ch(yn+1,zn+1); 
  TMatrixD varevt_am(yn+1,zn+1); 
  TMatrixD varevt_ch(yn+1,zn+1);


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
   
  int bound(0),mean_bound(0);
  
  for (int i = 0; i < yn; i++) {
    //xaxis[i]=dTheta_x_vs_impact_x_vs_Impact_y->GetYaxis()->GetBinCenter(i);
    for (int j = 0; j < zn; j++) {
      
      /*   
	   for (int i = 10; i < 30; i++) {
    for (int j =0; j < 20; j++) {
      */      
    

      TH1D *hx= h->ProjectionX("z slice",i,i,j,j);
     
      mean_ch[i][j] = 0;
      sigma_ch[i][j] = 0;
      cost_ch[i][j] = 0;
      bound = 0;
      mean_bound = 0;

      //cout<<"**************fit*******************"<<endl;
      TAxis *axis = hx->GetXaxis();
      int bmin = axis->FindBin(-200);
      int bmax = axis->FindBin(300); 
      int_am[i][j] = hx->Integral(bmin,bmax);   
      
      //cout << "total event "<<int_am[i][j] << endl;

      if (int_am[i][j] != 0) { 
	if (int_am[i][j] > 1000) {
	  TF1 *g2    = new TF1("g2","gaus",ch_min,ch_max);
	  g2->SetLineColor(kRed); 
	  g2->SetParLimits(1,120.,180.);
	  g2->SetParLimits(2,0.,15.);
	  
	  hx->Fit(g2,"ONQR");
	  g2->GetParameters(&par[0]);
	  
	  mean_ch[i][j] = par[1];
	  sigma_ch[i][j] = par[2];
	  cost_ch[i][j] = par[0];
	  //cout << "slice" << i << " " << j << endl;
	  //cout <<"parameter " <<mean_ch[i][j] <<" "<<sigma_ch[i][j]<<endl;	
	  
	  Int_t kick_min = 0;
	  Int_t kick_max = 0;
	  if (sigma_ch[i][j] < 10.  && sigma_ch[i][j] > 5.) bound = 1;
	  //if (mean_ch[i][j] < 150. && mean_ch[i][j] > 120.) mean_bound = 1;
	  
	  if(bound) {
	  
	    kick_min = mean_ch[i][j]-(sigma_ch[i][j]*3);
	    kick_max = mean_ch[i][j]+(sigma_ch[i][j]*3);
	  //cout << "good "<<kick_min<<" "<<kick_max<< endl;
	    
	  } else {
	    kick_max = ch_max;
	    kick_min = ch_min;
	  //cout << "warning "<<kick_min<<" "<<kick_max<<endl;
	
	  }
	  //Cerco bin per Integrali
	  int bmin_ch = axis->FindBin(kick_min);
	  int bmax_ch = axis->FindBin(kick_max); 
	  //integrali
	  int_ch[i][j] = hx->Integral(bmin_ch,bmax_ch);
	  //cout<<"channeled event" <<int_ch[i][j]<<endl;
	  eff_int[i][j] = int_ch[i][j]/(int_am[i][j]);
	  //cout<<"efficiency "<< eff_int[i][j]<<endl;
	
	} else {
	
	  cout << "control < 1000" << endl;  
	  kick_max = ch_max;
	  kick_min = ch_min;
	  //Cerco bin per Integrali
	  int bmin_ch = axis->FindBin(kick_min);
	  int bmax_ch = axis->FindBin(kick_max); 
	  //integrali
	  int_ch[i][j] = hx->Integral(bmin_ch,bmax_ch);
	  //cout<<"channeled event" <<int_ch[i][j]<<endl;
	  eff_int[i][j] = int_ch[i][j]/(int_am[i][j]);
	  //cout<<"efficiency "<< eff_int[i][j]<<endl;
	}
      } else {
	eff_int[i][j] = 0.;
      }
      // cout << "numero di eventi integrali in channeling "<< int_ch[i] <<endl;
     
    }
  } 

  
  TH1D *hs= h->ProjectionX("z slice",81,81,47,47);
  TCanvas * c4 = new TCanvas();
  hs->Draw();
  
  cout << i << " " << j << endl;
  cout << eff_int[81][47] << endl; 
  

  TH2D * efficiency_vs_impact_x_vs_Impact_y = new TH2D("efficiency_vs_impact_x_vs_Impact_y","efficiency_vs_impact_x_vs_Impact_y",100,-3.,3.,400,-50., 350.);
  efficiency_vs_impact_x_vs_Impact_y->SetTitle("Efficiency in Impact Angle x vs Impact Position y");
  efficiency_vs_impact_x_vs_Impact_y->GetXaxis()->SetTitle("Impact Position y [mm]");
  efficiency_vs_impact_x_vs_Impact_y->GetYaxis()->SetTitle("Impact Angle x [#murad]");

  cout << "intorno di channeling " << endl;
  cout << "[" << ch_min << "," << ch_max << "]" << endl; 

  cout<<"Processing finished!"<<endl;
    
  /*
  TVectorD r_y(zn+1);
  TVectorD err_y(zn+1);
  TMatrixD eff_int_turn(zn+1,yn+1);
  */ 

  Double_t r_y[600];
  Double_t err_y[600];
  Double_t counter[600];
  Double_t eff_int_turn[600][600];

  
  //TVectorD mean_x(zn+1);
  //TVectorD sigma_x(zn+1);
  Double_t mean_x[600];
  Double_t sigma_x[600];
  Double_t sigma_eff[600];
  Double_t stderr[600];
  Double_t mean[600];
  Double_t mean_w[600];
  Double_t somma[600];
  Double_t sum[600];
  Double_t prod[600];
  Double_t sum_pos[600];



  Double_t r_x[600][600];
  int count(0);  

  for (int j = 0; j < zn; j++) {

    count=0;
    for (int i = 0; i < yn; i++) {  

      efficiency_vs_impact_x_vs_Impact_y->SetBinContent(j,i,eff_int[i][j]);
      eff_int_turn[j][i] = eff_int[i][j];
      if (eff_int_turn[j][i] != 0) {
	count++;
      }

      r_x[j][i] = h->GetYaxis()->GetBinCenter(i);  
    }
    r_y[j]= efficiency_vs_impact_x_vs_Impact_y->GetXaxis()->GetBinCenter(j);
    err_y[j]= (efficiency_vs_impact_x_vs_Impact_y->GetXaxis()->GetBinWidth(j))/2.;
    counter[j]= count;
    cout<< counter[j] << endl;

  }

  TCanvas * c5 = new TCanvas();
  efficiency_vs_impact_x_vs_Impact_y->Draw("colz");


  int l(0);


  for (int j = 0; j < zn; j++) {
    for (int i = 0; i < yn; i++) {
      somma[j] += eff_int_turn[j][i];
      prod[j] += r_x[j][i]*eff_int_turn[j][i];
      sum_pos[j] += r_x[j][i];
    }
    mean[j]=prod[j]/somma[j];
    mean_w[j]=prod[j]/sum_pos[j];
  }





  TCanvas * cm = new TCanvas();
  //TGraphErrors * m_ang = new TGraphErrors(zn,r_y,mean,err_y,stderr);
  TGraph * m_ang = new TGraph(zn,r_y,mean);
  Double_t par_lin[2]=0;
  TF1 * flin = new TF1("flin","pol1",-2.,2.);
  flin->SetLineColor(kRed);
  
  m_ang->Fit(flin,"R");
  flin->GetParameters(&par_lin[0]);
  m_ang->Draw("a*"); 
  
  cout<<par_lin[0] <<"  "<< par_lin[1]<< endl;





 
  Double_t **eff_clean;
  Double_t **r_x_clean;
  eff_clean = (double **) malloc(600*sizeof(double *));
  r_x_clean = (double **) malloc(600*sizeof(double *));

  for (int i = 0; i<600; i++) {
    eff_clean[i] = (double *) malloc(counter[i]*sizeof(double*));
    r_x_clean[i] = (double *) malloc(counter[i]*sizeof(double*));
  }


 
 

  for (int j = 0; j < zn; j++) {
    l=0;
    sum[j]=0;
    for (int i = 0; i < yn; i++) {  
      if (eff_int_turn[j][i] != 0) {
	eff_clean[j][l] = eff_int_turn[j][i];
	r_x_clean[j][l] = r_x[j][i];
	sum[j] += eff_clean[j][l];
	//sum[j] += pow(mean[j] - (par_lin[0]-par_lin[1]*r_y[j]),2);
	l++;
      }
      
    }
   
    stderr[j] = sqrt(pow(mean[j] - (par_lin[0]+par_lin[1]*r_y[j]),2));
    if (l !=0 ) { 
      //stderr[j] = sqrt(sum[j]/l)/sqrt(l);
      sigma_eff[j] = TMath::RMS(l,eff_clean[j]);   
      mean_x[j] = TMath::Mean(counter[j],eff_clean[j]);      

      cout<<l<<"  "<<mean[j] <<"  "<<r_y[j]  <<"  "<<mean_x[j] <<"  "<<(par_lin[0]+par_lin[1]*r_y[j]) <<"  "<<stderr[j]<<"  "<<sum[j]<<endl;
    }
    
  }
  
  /*
  for (int j = 0; j < zn; j++) {
    
    mean_x[j] = TMath::Mean(counter[j],eff_clean[j]);
    sigma_x[j] = TMath::RMS(counter[j],eff_clean[j]);
    cout<<mean_x[j]<<"  "<<sigma_x[j]<<endl;
    
  }
  */

  Double_t sum_test[600];
  Double_t err_test[600];

  for (int j = 0; j < zn; j++) {
    for (int i = 0; i < counter[j]; i++) {
      sum_test[j] += pow(r_x_clean[j][i]*eff_clean[j][i] - mean[j]*mean_x[j],2);
    }
    err_test[j] = sqrt(sum_test[j]/counter[j]);
  } 


  TCanvas * cm_new = new TCanvas();
  TGraphErrors * m_ang_new = new TGraphErrors(zn,r_y,mean,err_y,err_test);
  m_ang_new->SetTitle("Mean Impact Angle");
  m_ang_new->GetXaxis()->SetTitle("Impact Position y [mm]");
  m_ang_new->GetYaxis()->SetTitle("Mean Impact Angle x [#murad]");
  //TGraph * m_ang = new TGraph(zn,r_y,mean);
  //m_ang_new->Draw("a*");
  Double_t par_lin_new[2]=0;
  TF1 * flin_new = new TF1("flin_new","pol1",-1.5,1.5);
  flin_new->SetLineColor(kRed);
  
  m_ang_new->Fit(flin_new,"R");
  flin_new->GetParameters(&par_lin_new[0]);
  m_ang_new->Draw("ap"); 
  
  cout<<par_lin_new[0] <<"  "<< par_lin_new[1]<< endl;

  /*
  TCanvas * cprof = new TCanvas();
  tor = new TProfile("tor","Mean Impact Angle",70,-3.,3.,-10.,30.);
  
  for (int j = 0; j < zn; j++) {
    tor->Fill(r_y[j],mean[j]);
  }
  tor->Draw();
  */
 
  TFile *outfile = new TFile(outputfile,"RECREATE");
  efficiency_vs_impact_x_vs_Impact_y->Write();
  m_ang_new->Write();

  
  myfile.close();



}
