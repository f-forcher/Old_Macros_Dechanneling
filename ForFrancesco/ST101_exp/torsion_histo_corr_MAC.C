
#include "librerie.h"

//riempimento istogrammi dopo correzione torsione
void torsion_histo_corr(){

  ofstream myfile;
    
    
  // Input file name
 
    TString fileName("/run/media/fandrisa/6094557b-7c6a-4bfd-9116-c0c405e03e59/TB_29_04_15_recodata/recoDataSimple_2262");
  
  //TString fileName("recoDataSimple_628");
  //TString fileName("recoDataSimple_629");
  
  //TString outputfile = "recoDataSimple_630.histo.root";
  //TString outputfile = "recoDataSimple_629.00-05.test.histo.root"; 
  TString outputfile = "recoDataSimple_2262.torsion.correction.histo.root";
  
  fileName += ".root"; 
      
  // Output file name
  myfile.open("roottext.txt");

  
    // ROOT stored Variables ****************************************************************
    //
    // Variable names are
    //      evt             -> event information
    //                              -> evt.run       run number
    //                              -> evt.evtnum    event number
    //				    -> evt.nuclear   lateral trigger event
    //      date	    -> char array of event date in DDMMYY format
    //
    //      time	    -> char array of event time in HHMMSS format
    //
    //      singleTrack	    -> flag for signle track event
    //
    //      multiHit	    -> flag fo multi track event
    //
    //      gPos            -> goniometer coordinates 
    //                              ->  gPos.x       horizontal rotation [rad]
    //                              ->  gPos.y       tilt [rad]
    //                              ->  gPos.z       lateral translation [mm]
    //     MultiHits        -> set of hit parameters per event where whit more tha one track
    //                              ->  mHits[i].plane[nplane]          plane number 0->4 (0 is upstream)
    //                              ->  mHits[i].thetaIn_x        incoming theta in x [rad]
    //                              ->  mHits[i].thetaIn_y        incoming theta in y [rad]
    //                              ->  mHits[i].thetaInErr_x     incoming theta error in x (nominal) [rad]    
    //                              ->  mHits[i].thetaInErr_y     incoming theta error in y (nominal) [rad] 
    //                              ->  mHits[i].d0_x             impact point on xtal in x [mm]
    //                              ->  mHits[i].d0_y             impact point on xtal in y [mm]
    //                              ->  mHits[i].d0Err_x          impact point error on xtal in x [mm]
    //                              ->  mHits[i].d0Err_y          impact point error on xtal in y [mm]
    //      Tracks	    -> set of track parameters per event where ntracks is number of tracks recorded in the event
    //                              ->  tracks[i].thetaIn_x        incoming theta in x [rad]
    //                              ->  tracks[i].thetaIn_y        incoming theta in y [rad]
    //                              ->  tracks[i].thetaOut_x       outgoing theta in x [rad]
    //                              ->  tracks[i].thetaOut_y       outgoing theta in y [rad]
    //                              ->  tracks[i].thetaInErr_x     incoming theta error in x (nominal) [rad]    
    //                              ->  tracks[i].thetaInErr_y     incoming theta error in y (nominal) [rad] 
    //                              ->  tracks[i].thetaOutErr_x    outgoing theta error in x (nominal) [rad] 
    //                              ->  tracks[i].thetaOutErr_y    outgoing theta error in y (nominal) [rad] 
    //                              ->  tracks[i].d0_x             impact point on xtal in x [mm]
    //                              ->  tracks[i].d0_y             impact point on xtal in y [mm]
    //                              ->  tracks[i].d0Err_x          impact point error on xtal in x [mm]
    //                              ->  tracks[i].d0Err_y          impact point error on xtal in y [mm]
    //                              ->  tracks[i].chi2_x           chi2 of fit in x
    //                              ->  tracks[i].chi2_y           chi2 of fit in y
    
        

  //struct declaration 
  struct Event
    {
       int run;
       int evtnum;
       int nuclear;
    } evt;
    
  struct GonioPos
    {
       double x;
       double y;
       double z;
    } gPos;
    
  struct MultiHit
    {
      int plane[5];
      double thetaIn_x;
      double thetaIn_y;
      double thetaInErr_x;
      double thetaInErr_y;
      double d0_x;
      double d0_y;
      double d0Err_x;
      double d0Err_y;
 } mHits;
    
  struct Track
    {
       double thetaIn_x;
       double thetaIn_y;
       double thetaOut_x;
       double thetaOut_y;
       double thetaInErr_x;
       double thetaInErr_y;
       double thetaOutErr_x;
       double thetaOutErr_y;
       double d0_x;
       double d0_y;
       double d0Err_x;
       double d0Err_y;
       double chi2_x;
       double chi2_y;
    } tracks;


  int multiHit;
  int singleTrack;
    
  char time[80];
  char date[80];
      

    
  TFile * file = new TFile(fileName,"READ");
  TTree * tree = (TTree*)file->Get("simpleEvent");
    
  Int_t nentries = (Int_t)tree->GetEntries();
 

  /*  
  tree->SetBranchAddress("Event",&evt);
  tree->SetBranchAddress("Time",&time);
  tree->SetBranchAddress("Date",&date);
  tree->SetBranchAddress("GonioPos",&gPos);
  tree->SetBranchAddress("nHits",&nhits);
  tree->SetBranchAddress("Hits",&hits);
  tree->SetBranchAddress("nTracks",&ntracks);
  tree->SetBranchAddress("Tracks",&tracks);
  */


  TBranch * bEvent = tree->GetBranch("Event");
  TBranch * bTime = tree->GetBranch("Time");
  TBranch * bDate = tree->GetBranch("Date");
  TBranch * bGonioPos = tree->GetBranch("GonioPos");
  TBranch * bnhits = tree->GetBranch("MultiHit");
  TBranch * bHits = tree->GetBranch("MultiHits");
  TBranch * bntracks = tree->GetBranch("SingleTrack");
  TBranch * bTracks = tree->GetBranch("Tracks");
    
  bEvent->SetAddress(&evt);
  bTime->SetAddress(&time);
  bDate->SetAddress(&date);
  bGonioPos->SetAddress(&gPos);
  bnhits->SetAddress(&multiHit);
  bHits->SetAddress(&mHits);
  bntracks->SetAddress(&singleTrack);
  bTracks->SetAddress(&tracks);
  

  //variable daclaration

  Double_t init_event = 20.;
  Double_t Maxd0x = 0.6 ; 
  Double_t Maxd0y = 20. ; 
  Double_t mind0x =-0.90 ; 
  Double_t mind0y = -20.; 
  Double_t Maxd0y_new = 0. ; 
  Double_t mind0y_new = 0. ; 
  //Double_t gonioMax = 11.0; 
  //Double_t goniomin = 5.0; 
  //Double_t gonioMax_2 = 5.0; 
  //Double_t goniomin_2 = 0.0; 
  //Double_t gonioMax_3 = 38.0; 
  //Double_t goniomin_3 = 35.0; 
  Double_t ThetaInMax = 10.0; 
  Double_t ThetaInmin = -2.0;
  
  double init_scan = 4822988.575;
  //double init_scan = 4712945.0;
  //double init_scan = 4712980.0; 
  int chentries = 4889340;  
  double b = 0.5914;  //p0 = angolo di offset
  double a = -7.158; //p1 = torsione cristallo



   
  //histogram declaration

  // TH1D d0_x_h = TH1D("d0_x_h", " Impact point at crystal x ",600,-100.,100.);
  //TH1D d0_y_h = TH1D("d0_y_h", " Impact point at crystal y ",600,-100.,100.);
  //TH2D * dTheta_x_vs_d0x_d0ycut  = new TH2D("dTheta_x_vs_d0x_d0ycut", " d0x vs dTheta d0y cut" , 400,-200.,200.,23., mind0x, Maxd0x);
  //TH2D * dTheta_x_vs_d0y = new TH2D("dTheta_x_vs_d0y", "dTheta_x_vs_d0y", 400,-200.,200., 100., -5., 5);  
  //TH2D * dTheta_y_vs_d0y = new TH2D("dTheta_y_vs_d0y", "dTheta_y_vs_d0y", 400,-200.,200., 100., -5., 5);


  //TH2D * Impact_x_vs_Impact_y = new TH2D("Impact_x_vs_Impact_y","Impact angle x vs Impact y" , 100.,-5.,5.,600.,-30.,30.);
  TH1D * dTheta_x = new TH1D("dTheta_x","dTheta_x",400,-200.,200.);  
  TH3D * dTheta_x_vs_Impact_x_vs_Impact_y = new TH3D("dTheta_x_vs_Impact_x_vs_Impact_y","dTheta_x_vs_Impact_x_vs_Impact_y", 400,-200.,200.,60,-30., 30.,100,-3.,3.);

  //TH2D * dTheta_x_vs_d0x_d0ycut  = new TH2D("dTheta_x_vs_d0x_d0ycut", " d0x vs dTheta up " , 1400,-400.,1000.,23., mind0x, Maxd0x);
  //TH2D * dTheta_x_vs_gonio_d0ycut  = new TH2D("dTheta_x_vs_gonio_d0ycut", " gonio vs dTheta up d0ycut" , 1400,-400.,1000.,100., 0., 100.);
  //TH2D * dTheta_x_vs_gonio_d0ycut_d0xcut = new TH2D("dTheta_x_vs_gonio_d0ycut_d0xcut" , " gonio vs dTheta up  d0ycut d0xcut" , 1400,-400.,1000.,100., 0., 100.);
  
  
  //TH2D * dTheta_x_vs_d0x_nocut = new TH2D("dTheta_x_vs_d0x_nocut" , " dTheta_x_vs_d0x_nocut" , 1400,-400.,1000.,100., 0., 100.);
  //TH2D * dTheta_x_vs_gonio_nocut = new TH2D("dTheta_x_vs_gonio_nocut" , " dTheta_x_vs_gonio_nocut" , 1400,-400.,1000.,2000., 4712000., 4714000. );
  //TH2D * dTheta_x_vs_gonio_nocut_zero = new TH2D("dTheta_x_vs_gonio_nocut_zero" , " dTheta_x_vs_gonio_nocut_zero" , 1400,-400.,1000.,500., 0., 100.);
  //TH2D * dTheta_x_vs_gonio_mycut_test = new TH2D("dTheta_x_vs_gonio_mycut_test" , " dTheta_x_vs_gonio_mycut_test" , 1400,-400.,1000.,100., 0., 100.);
  // TH2D * dTheta_x_vs_Impact_x_nocut = new TH2D("dTheta_x_vs_Impact_x_nocut" , " dTheta_x_vs_Impact_x_nocut " ,500., -50., 50., 1400,-400.,1000.);
  //TH2D * dTheta_x_vs_Impact_x_d0ycut = new TH2D("dTheta_x_vs_Impact_x_d0ycut" , " dTheta_x_vs_Impact_x_d0ycut " ,500., -50., 50., 1400,-400.,1000.);
  //TH2D * Impact_x_vs_d0y = new TH2D("Impact_x_vs_d0y", "Impact_x_vs_d0y",100,0.,100.,30,-3.,3.);
  
  TH2D * dTheta_x_vs_Impact_x = new TH2D("dTheta_x_vs_Impact_x","dTheta_x vs Impact_x",100,-100.,100., 600,-200.,400.);
  dTheta_x_vs_Impact_x->GetXaxis()->SetTitle("Impact Angle #theta_{x} [#murad]");
  dTheta_x_vs_Impact_x->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]"); 
  TH2D * dTheta_x_vs_Impact_x_cor = new TH2D("dTheta_x_vs_Impact_x_cor","dTheta_x vs Impact_x tor Corr",100,-100.,100., 600,-200.,400.);
  dTheta_x_vs_Impact_x_cor->GetXaxis()->SetTitle("Impact Angle #theta_{x} [#murad]");
  dTheta_x_vs_Impact_x_cor->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");  
  
  //TH1D * dTheta_x_nocut = new TH1D("dTheta_x_nocut","dTheta x no cut",400,-200.,200.);
  //TH1D * dTheta_y_nocut = new TH1D("dTheta_y_nocut","dTheta y no cut",400,-200.,200.);
  //TH1D * dTheta_x_goniocut_d0ycut = new TH1D("dTheta_x_goniocut_d0ycut","dTheta x goniocut d0y cut",1400,-400.,1000.);
  //TH1D * dTheta_x_d0ycut = new TH1D("dTheta_x_d0ycut","dTheta x d0y cut",1400,-400.,1000.);
  //TH1D * dTheta_x_d0ycut_d0xcut = new TH1D("dTheta_x_d0ycut_d0xcut","dTheta x d0y cut d0x cut",1400,-400.,1000.);
 

  int count_1track = 0;
  int count_d0xcut = 0;
  int angle_cut = 0;
  Maxd0x = 0.6;
  mind0x = -0.2;
  Maxd0y = 1.5;
  mind0y = -1.5;    // da mean_impact_angle
 
  int cutd0y(0), cutd0x(0), angle_cut(0), cutd0y_new;


  for (Int_t i = 0; i < nentries; i++) {
    if(i%50000==0) {cout  << "event " << i << endl;}

    Long64_t tentry = tree->LoadTree(i);
    tree->GetEntry(i);
		
    
    /* 
    int count_hit_plane[5];
    for (Int_t ip  = 0; ip < 5; ip++) count_hit_plane[ip] =0; 
    
    for (Int_t ih  = 0; ih < nhits; ih++) {
      
      for (Int_t ip  = 0; ip < 5; ip++)  { if(hits.plane[ih] ==ip )count_hit_plane[ip] = count_hit_plane[ip] +1; }
	    
      }
      int fit_cond(0) , angle_cut(0), cutd0x(0);
      //Maxd0x = 0.2; 
      //mind0x = -0.6; 
      
      */
      
    //if (count_hit_plane[0]==1 && count_hit_plane[1]==1)  fit_cond=1;
    
    //init_position scan 
    //double init_angle = 4.241401e+06;    

    cutd0x = 0;
    cutd0y = 0;
    cutd0y_new = 0;
    angle_cut = 0;
      
    if (tracks.d0_y < Maxd0y && tracks.d0_y > mind0y) cutd0y=1;
    if (tracks.d0_x < Maxd0x && tracks.d0_x > mind0x) cutd0x=1;
 
      
    if(singleTrack==1 && cutd0x && cutd0y) {
      //dTheta_x_vs_Impact_x_nocut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0] )*1e6,gPos.x+(tracks.thetaIn_x[0]*1e6)-init_scan);
      
      count_1track++;
    
   
      //  if (cutd0x && cutd0y) {
      //	count_d0xcut++;
      if ((gPos.x+(tracks.thetaIn_x*1e6)-init_scan-((a*tracks.d0_y)+b)) < ThetaInMax && (gPos.x+(tracks.thetaIn_x*1e6)-init_scan-((a*tracks.d0_y)+b)) > ThetaInmin) angle_cut=1;      
  
      if (angle_cut) {dTheta_x->Fill((tracks.thetaOut_x - tracks.thetaIn_x)*1e6);}

      
      dTheta_x_vs_Impact_x_vs_Impact_y->Fill((tracks.thetaOut_x - tracks.thetaIn_x)*1e6,gPos.x+(tracks.thetaIn_x*1e6)-init_scan-((a*tracks.d0_y)+b),tracks.d0_y);

      //Impact_x_vs_Impact_y->Fill(tracks.d0_y[0],gPos.x+(tracks.thetaIn_x[0]*1e6)-((a*tracks.d0_y[0])-b));

     
      //dTheta_x_vs_Impact_x_nocut->Fill(gPos.x+(tracks.thetaIn_x*1e6)-init_scan-((a*tracks.d0_y)+b),(tracks.thetaOut_x - tracks.thetaIn_x )*1e6);

      dTheta_x_vs_Impact_x->Fill(gPos.x+(tracks.thetaIn_x*1e6)-init_scan,(tracks.thetaOut_x - tracks.thetaIn_x)*1e6);
      dTheta_x_vs_Impact_x_cor->Fill(gPos.x+(tracks.thetaIn_x*1e6)-init_scan-((a*tracks.d0_y)+b),(tracks.thetaOut_x - tracks.thetaIn_x)*1e6); 	


      //dTheta_x_vs_gonio_nocut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6 ,gPos.x+tracks.thetaIn_x[0]);
	//dTheta_x->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6);
	//dTheta_y_vs_d0y->Fill((tracks.thetaOut_y[0] - tracks.thetaIn_y[0])*1e6,tracks.d0_y[0]);
	//Impact_x_vs_Impact_y->Fill(tracks.d0_y[0],gPos.x+(tracks.thetaIn_x[0]*1e6));
	
      //}  

      //dTheta_x_vs_gonio_nocut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6 ,gPos.x+tracks.thetaIn_x[0]);
      //dTheta_x_vs_gonio_nocut_zero->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0] )*1e6,gPos.x+(tracks.thetaIn_x[0]*1e6)-init_scan);
      
    } //close cut 1track
      
  } //close cycle on nentries



  cout<<"events with 1 track "<< count_1track << endl;
  cout<<"events in d0xcut "<< count_d0xcut << endl;

  TFile *outfile = new TFile(outputfile,"RECREATE");
  dTheta_x_vs_Impact_x_vs_Impact_y->Write();
  dTheta_x->Write();
  dTheta_x_vs_Impact_x->Write();
  dTheta_x_vs_Impact_x_cor->Write();
  //dTheta_x_vs_Impact_x_d0ycut->Write();
  //Impact_x_vs_Impact_y->Write();
  
  myfile.close();


 TCanvas *c1 = new TCanvas();
  dTheta_x_vs_Impact_x->Draw("colz");
  
  
 TCanvas *c2 = new TCanvas();
  dTheta_x_vs_Impact_x_cor->Draw("colz");
  
  
}


/*
  TCanvas *c1 = new TCanvas();
  dTheta_x->Draw();
  
  Double_t par_ch[3]=0;
  TF1 *g2    = new TF1("g2","gaus",120,180);
  g2->SetLineColor(kRed); 
  dTheta_x->Fit(g2,"R");
  g2->GetParameters(&par_ch[0]);
  
  cout << "paramteri fit"<<endl;
  cout << par_ch[2] << endl;
  cout << par_ch[1] << endl;
  
  Int_t ch_min = par_ch[1]-(par_ch[2]*3);
  Int_t ch_max = par_ch[1]+(par_ch[2]*3);
  
  cout << "intorno di channeling " << endl;
  cout << "[" << ch_min << "," << ch_max << "]" << endl; 

 
  //TCanvas *c2 = new TCanvas();
  //dTheta_x_vs_Impact_x_nocut.Draw("colz");


 //efficiecy

  int ymax = dTheta_x_vs_Impact_x_vs_Impact_y->GetYaxis()->GetLast(); 
  int ymin = dTheta_x_vs_Impact_x_vs_Impact_y->GetYaxis()->GetFirst();   
  Int_t yn = dTheta_x_vs_Impact_x_vs_Impact_y->GetNbinsY(); 

  cout << ymax << " " << ymin << " " << yn <<  endl;
  
  int zmax = dTheta_x_vs_Impact_x_vs_Impact_y->GetZaxis()->GetLast(); 
  int zmin = dTheta_x_vs_Impact_x_vs_Impact_y->GetZaxis()->GetFirst();   
  Int_t zn = dTheta_x_vs_Impact_x_vs_Impact_y->GetNbinsZ(); 

  cout << zmax << " " << zmin << " " << zn <<  endl;


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
  *
  
  //ciclo per riempire array
  
  int bound(0),mean_bound(0);

  for (int i = 0; i < yn; i++) {
    //xaxis[i]=dTheta_x_vs_impact_x_vs_Impact_y->GetYaxis()->GetBinCenter(i);

    for (int j =0; j < zn; j++) {
      TH1D *h= dTheta_x_vs_Impact_x_vs_Impact_y->ProjectionX("z slice",i,i,j,j);

      //riempo asse y eff
      //yaxis[j]=dTheta_x_vs_impact_x_vs_Impact_y->GetZaxis()->GetBinCenter(j);
      //yerr[i]=(h->GetYaxis()->GetBinWidth(i))/2.;
      //cout << yaxis[i] << "  " << yerr[i] << endl;

      mean_ch[i][j] = 0;
      sigma_ch[i][j] = 0;
      cost_ch[i][j] = 0;
      bound = 0;
      mean_bound = 0;

      cout<<"**************fit*******************"<<endl;
      TAxis *axis = h->GetXaxis();
      int bmin = axis->FindBin(-200);
      int bmax = axis->FindBin(200); 
      int_am[i][j] = h->Integral(bmin,bmax);   
      
      cout << int_am[i][j] << endl;

      if (int_am[i][j] != 0) { 

	TF1 *g2    = new TF1("g2","gaus",ch_min,ch_max);
	g2->SetLineColor(kRed); 
	g2->SetParLimits(1,120.,150.);
	g2->SetParLimits(2,-15.,15.);

	h->Fit(g2,"ONQR");
	g2->GetParameters(&par[0]);
	
	mean_ch[i][j] = par[1];
	sigma_ch[i][j] = par[2];
	cost_ch[i][j] = par[0];
	cout << mean_ch[i][j] <<" "<<sigma_ch[i][j]<<endl;	

	Int_t kick_min = 0;
	Int_t kick_max = 0;
	if (sigma_ch[i][j] < 15. && sigma_ch[i][j] > 5.) bound = 1;
	if (mean_ch[i][j] < 150. && mean_ch[i][j] > 120.) mean_bound = 1;
	
	if(bound && mean_bound) {
	  
	  kick_min = mean_ch[i][j]-(sigma_ch[i][j]*3);
	  kick_max = mean_ch[i][j]+(sigma_ch[i][j]*3);
	  cout << "good "<<kick_min<<" "<<kick_max<< endl;

	} else {
	  kick_max = ch_max;
	  kick_min = ch_min;
	  cout << "warning "<<kick_min<<" "<<kick_max<<endl;
	
	}

	//Cerco bin per Integrali
	
	int bmin_ch = axis->FindBin(kick_min);
	int bmax_ch = axis->FindBin(kick_max); 
	
	//integrali
	
	int_ch[i][j] = h->Integral(bmin_ch,bmax_ch);
	cout<<int_ch[i][j]<<endl;

	eff_int[i][j] = int_ch[i][j]/(int_am[i][j]);

      } else {
	eff_int[i][j] = 0.;
      }
      // cout << "numero di eventi integrali in channeling "<< int_ch[i] <<endl;

     
  
     
      //cout << eff << endl;

    //cout << int_am <<" "<< int_ch << endl;
    //cout << int_ch/int_am << endl;
    /*
      cov02_am[i][j] = covMatrix1(0,2);
      cov00_am[i][j] = covMatrix1(0,0);
      cov22_am[i][j] = covMatrix1(2,2);
      cov02_ch[i][j] = covMatrix2(0,2);
      cov00_ch[i][j] = covMatrix2(0,0);
      cov22_ch[i][j] = covMatrix2(2,2);
      
      varevt_am[i][j] = sqrt(sqrt(2.*TMath::Pi())*(pow(sigma_am[i],2)*pow(cov00_am[i],2)+pow(cost_am[i],2)*pow(cov22_am[i],2)+2*cov02_am[i]*sigma_am[i]*cost_am[i]));

      varevt_ch[i][j] = sqrt(sqrt(2.*TMath::Pi())*(pow(sigma_ch[i],2)*pow(cov00_ch[i],2)+pow(cost_ch[i],2)*pow(cov22_ch[i],2)+2*cov02_ch[i]*sigma_ch[i]*cost_ch[i]));

      xerr[i][j] = sqrt(TMath::Abs(sqrt(2.*TMath::Pi())*(pow(varevt_ch[i],2)/pow(evt_am[i],2)-pow(varevt_am[i],2)*pow(evt_ch[i]/pow(evt_am[i],2),2))));
    *    
   
     
    }
  } 


  // TH1D *h= dTheta_x_vs_Impact_x_vs_Impact_y->ProjectionX("z slice",35,35,50,50);
  //TCanvas * c4 = new TCanvas();
  //h->Draw();
  
  cout << i << " " << j << endl;
  cout << eff_int[60][30] << endl; 

  TH2D * efficiency_vs_impact_x_vs_Impact_y = new TH2D("efficiency_vs_impact_x_vs_Impact_y","efficiency_vs_impact_x_vs_Impact_y",120,-3.,3.,60,-30., 30.);




  
  cout << "intorno di channeling " << endl;
  cout << "[" << ch_min << "," << ch_max << "]" << endl; 

  cout<<"Processing finished!"<<endl;
    

  

  for (int j = 0; j < zn; j++) {
    for (int i = 0; i < yn; i++) {  
      efficiency_vs_impact_x_vs_Impact_y->SetBinContent(j,i,eff_int[i][j]);
    }
  }

  TCanvas * c5 = new TCanvas();
  efficiency_vs_impact_x_vs_Impact_y->Draw("colz");
  
}  //close macro

*/
