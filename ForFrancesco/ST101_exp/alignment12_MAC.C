
#include "librerie.h"



//ANALISI RUN ALLINEAMENTO -> Risoluzione e Parametri del Fascio



void alignment12(){

  ofstream myfile;
    
    
  // Input file name
 
    
    // metto run giusta per allineamento Febbraio

  
    TString fileName("/run/media/dasukhon/6094557b-7c6a-4bfd-9116-c0c405e03e59/runs_TB/TB_29_04_15_recodata/recoDataSimple_2256");



  TString outputfile = "recoDataSimple_2256.alignment.histo.root";
  
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
    
  struct MultiHit     // era MultiHit e mHits
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


    int multiHit; int multihit;
  int singleTrack;
  int ntracks;
  //int nhits; //hits;
    
    
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
  bnhits->SetAddress(&multihit);
  bHits->SetAddress(&mHits);
  bntracks->SetAddress(&singleTrack);
  bTracks->SetAddress(&tracks);
 

  //variable daclaration

  Double_t init_event = 20.;
  Double_t Maxd0x = 0.5 ; 
  Double_t Maxd0y = 20. ; 
  Double_t mind0x =-0.90 ; 
  Double_t mind0y = -20.; 
  //Double_t gonioMax = 11.0; 
  //Double_t goniomin = 5.0; 
  //Double_t gonioMax_2 = 5.0; 
  //Double_t goniomin_2 = 0.0; 
  //Double_t gonioMax_3 = 38.0; 
  //Double_t goniomin_3 = 35.0; 
  Double_t ThetaInMax = 2.0e-6;
  Double_t ThetaInmin =-2.0e-6;

  int chentries = 4889340;
  int init_am = 4889341;
  double init_scan = 1584230.8;	
  //double init_scan = 4712945.0;
  //double init_scan = 4712980.0; 
  

   
  //histogram declaration


  TH1D * d0x_h = new TH1D("d0x_h", " Impact point at crystal x ",600,-30.,30.);
  d0x_h->GetXaxis()->SetTitle("x [mm]");
  TH1D * d0y_h = new TH1D("d0y_h", " Impact point at crystal y ",600,-30.,30.);
  d0y_h->GetXaxis()->SetTitle("y [mm]");
 
 TH2D * d0x_vs_d0y_h = new TH2D("d0x_vs_d0y_h","beam spot",600,-30.,30.,600,-30.,30.);
  d0x_vs_d0y_h->GetXaxis()->SetTitle("x [mm]");
  d0x_vs_d0y_h->GetYaxis()->SetTitle("y [mm]");

    
    
  //TH2D * dTheta_x_vs_d0x_goniocut_d0ycut  = new TH2D("dTheta_x_vs_d0x_goniocut_d0ycut", " d0x vs dTheta up gonio cut" , 1400,-400.,1000.,23., mind0x, Maxd0x);
  //TH2D * dTheta_x_vs_d0x_d0ycut  = new TH2D("dTheta_x_vs_d0x_d0ycut", " d0x vs dTheta up " , 1400,-400.,1000.,23., mind0x, Maxd0x);
  //TH2D * dTheta_x_vs_gonio_d0ycut  = new TH2D("dTheta_x_vs_gonio_d0ycut", " gonio vs dTheta up d0ycut" , 1400,-400.,1000.,100., 0., 100.);
  //TH2D * dTheta_x_vs_gonio_d0ycut_d0xcut = new TH2D("dTheta_x_vs_gonio_d0ycut_d0xcut" , " gonio vs dTheta up  d0ycut d0xcut" , 1400,-400.,1000.,100., 0., 100.);
  
  
  TH2D * dTheta_x_vs_d0x_nocut = new TH2D("dTheta_x_vs_d0x_nocut" , " dTheta_x_vs_d0x_nocut" , 1400,-100.,100.,800, -200., 300.);
  TH2D * dTheta_x_vs_d0y_nocut = new TH2D("dTheta_x_vs_d0y_nocut" , " dTheta_x_vs_d0y_nocut" , 1400,-100.,100.,800, 0., 300.);
  TH2D * dTheta_y_vs_d0x_nocut = new TH2D("dTheta_y_vs_d0x_nocut" , " dTheta_y_vs_d0x_nocut" , 1400,-100.,100.,800, -200., 300.);
  TH2D * dTheta_y_vs_d0y_nocut = new TH2D("dTheta_y_vs_d0y_nocut" , " dTheta_y_vs_d0y_nocut" , 1400,-100.,100.,800, -200., 300.);
  //TH2D * dTheta_x_vs_gonio_nocut = new TH2D("dTheta_x_vs_gonio_nocut" , " dTheta_x_vs_gonio_nocut" , 1400,-400.,1000.,2000., 4712000., 4714000. );
  //TH2D * dTheta_x_vs_gonio_nocut_zero = new TH2D("dTheta_x_vs_gonio_nocut_zero" , " dTheta_x_vs_gonio_nocut_zero" , 1400,-400.,1000.,500., 0., 100.);
  //TH2D * dTheta_x_vs_gonio_mycut_test = new TH2D("dTheta_x_vs_gonio_mycut_test" , " dTheta_x_vs_gonio_mycut_test" , 1400,-400.,1000.,100., 0., 100.);
  TH2D * dTheta_x_vs_Impact_x_nocut = new TH2D("dTheta_x_vs_Impact_x_nocut" , " dTheta_x_vs_Impact_x_nocut " , 1400,-100.,200.,500, -100., 100.);
    dTheta_x_vs_Impact_x_nocut->GetXaxis()->SetTitle("#theta_{x} [#murad]");
    dTheta_x_vs_Impact_x_nocut->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  //TH2D * Impact_x_vs_d0y = new TH2D("Impact_x_vs_d0y", "Impact_x_vs_d0y",100,0.,100.,30,-3.,3.);
  
  
  TH1D * dTheta_x_nocut = new TH1D("dTheta_x_nocut","dTheta x no cut",400,-200.,200.);
  dTheta_x_nocut->GetXaxis()->SetTitle("#theta [#murad]");
  TH1D * dTheta_y_nocut = new TH1D("dTheta_y_nocut","dTheta y no cut",400,-200.,200.);
  dTheta_y_nocut->GetXaxis()->SetTitle("#theta [#murad]");
  
   //TH1D * dTheta_x_goniocut_d0ycut = new TH1D("dTheta_x_goniocut_d0ycut","dTheta x goniocut d0y cut",1400,-400.,1000.);
  //TH1D * dTheta_x_d0ycut = new TH1D("dTheta_x_d0ycut","dTheta x d0y cut",1400,-400.,1000.);
  //TH1D * dTheta_x_d0ycut_d0xcut = new TH1D("dTheta_x_d0ycut_d0xcut","dTheta x d0y cut d0x cut",1400,-400.,1000.);
  TH1D * ThetaIn_x = new TH1D("ThetaIn_x","ThetaIn_x",200,-100,100);
  ThetaIn_x->GetXaxis()->SetTitle("#theta [#murad]");
  TH1D * ThetaIn_y = new TH1D("ThetaIn_y","ThetaIn_y",200,-100,100);
  ThetaIn_y->GetXaxis()->SetTitle("#theta [#murad]");
 
  
    
  int count_1track = 0;

  //read and fill

  for (Int_t i = 0; i < nentries; i++) {
    if(i%50000==0) cout  << "event " << i << endl;

    Long64_t tentry = tree->LoadTree(i);
    tree->GetEntry(i);
		
    
    /*
    int count_hit_plane[5];
    for (Int_t ip  = 0; ip < 5; ip++) count_hit_plane[ip] =0; 
  
  
    for (Int_t ih  = 0; ih < nhits; ih++) {
      
      for (Int_t ip  = 0; ip < 5; ip++)  { if(hits.plane[ih] ==ip )count_hit_plane[ip] = count_hit_plane[ip] +1; }
	    


    }
      int fit_cond(0) , angle_cut(0), cutd0x(0), cutd0y(0);
      //Maxd0x = 0.2; 
      //mind0x = -0.6;
      
    */
      
    
      
      //if (count_hit_plane[0]==1 && count_hit_plane[1]==1)  fit_cond=1;
      //    if (tracks.thetaIn_x[0] < ThetaInMax && tracks.thetaIn_x[0] > ThetaInmin) angle_cut=1;
      //if (tracks.d0_x[0] < Maxd0x && tracks.d0_x[0] > mind0x) cutd0x=1;

    //int cutd0y(0),cutd0x(0),goniocut(0),goniocut_2(0),goniocut_3(0);
      
	//if(tracks.d0_y[0] < Maxd0y && tracks.d0_y[0] > mind0y) cutd0y=1;
    //if(tracks.d0_x[0] < Maxd0x && tracks.d0_x[0] > mind0x) cutd0x=1;

      
      
      //init_position scan:
      
      double init_angle = 4.712668e+06;

      
      
      if(singleTrack==1) {
	
  //  dTheta_x_vs_Impact_x_nocut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0] )*1e6,gPos.x+(tracks.thetaIn_x[0]*1e6)-init_scan);
	
	count_1track++;
          
          
     //cout << gPos.x-init_scan << endl;

	
    /*
		if((gPos.x-init_scan) < gonioMax && (gPos.x-init_scan) > goniomin) goniocut =1;     
	if((gPos.x-init_scan) < gonioMax_2 && (gPos.x-init_scan) > goniomin_2) goniocut_2 =1;
	if((gPos.x-init_scan) < gonioMax_3 && (gPos.x-init_scan) > goniomin_3) goniocut_3 =1;	
	//cout << cutd0y << " " << goniocut_2 << endl;
	*/

	//fill no cut histo
	
  dTheta_x_nocut->Fill((tracks.thetaOut_x - tracks.thetaIn_x)*1e6);
	dTheta_y_nocut->Fill((tracks.thetaOut_y - tracks.thetaIn_y)*1e6);

	d0x_h->Fill(tracks.d0_x);
	d0y_h->Fill(tracks.d0_y);

	
 d0x_vs_d0y_h->Fill(tracks.d0_x,tracks.d0_y);

	ThetaIn_x->Fill((tracks.thetaIn_x)*1e6);   // controllo se devo mettere *1e6
	ThetaIn_y->Fill((tracks.thetaIn_y)*1e6);

	
          /*
          if (tracks.thetaIn_x[0]*1e6 < ThetaInMax && tracks.thetaIn_x[0]*1e6 > ThetaInmin){
              ThetaOut_x->Fill(tracks.thetaOut_x[0]*1e6);
          }
          
          if (tracks.thetaIn_y[0]*1e6 < ThetaInMax && tracks.thetaIn_y[0]*1e6 > ThetaInmin){
              ThetaOut_y->Fill(tracks.thetaOut_y[0]*1e6);
          }
          
	dTheta_x_vs_d0x_nocut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6,tracks.d0_x[0]);
    dTheta_x_vs_d0y_nocut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6,tracks.d0_y[0]);
    dTheta_y_vs_d0x_nocut->Fill((tracks.thetaOut_y[0] - tracks.thetaIn_y[0])*1e6,tracks.d0_x[0]);
    dTheta_y_vs_d0y_nocut->Fill((tracks.thetaOut_y[0] - tracks.thetaIn_y[0])*1e6,tracks.d0_y[0]);*/
	//dTheta_x_vs_gonio_nocut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6 ,gPos.x+tracks.thetaIn_x[0]);
	//dTheta_x_vs_gonio_nocut_zero->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0] )*1e6,gPos.x+(tracks.thetaIn_x[0]*1e6)-init_scan);
	
              
  /*   dTheta_x_vs_Impact_x_nocut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0] )*1e6,(tracks.thetaIn_x[0]*1e6));*/


	/*
	//fill cutted histo
	if(cutd0y && goniocut_3) {
	  //events_goniocut++;
	  dTheta_x_vs_d0x_goniocut_d0ycut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6,tracks.d0_x[0]);
	  dTheta_x_goniocut_d0ycut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6);
	  dTheta_x_vs_gonio_mycut_test->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6,gPos.x+(tracks.thetaIn_x[0]*1e6)-init_angle);
	}
	
	if(cutd0y) {
	  dTheta_x_vs_d0x_d0ycut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6,tracks.d0_x[0]);
	  dTheta_x_d0ycut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6);
	  dTheta_x_vs_gonio_d0ycut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6,gPos.x -init_angle);
	  //dTheta_x_vs_gonio_d0ycut_d0xcut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6,gPos.x-init_angle);
	  //dTheta_x_d0ycut_d0xcut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6);
	}
		
*/
      } //close cut 1track
      
      //} //close cycle on nhits 
  } //close cycle on nentries

  cout<<"events with 1 track "<< count_1track << endl;




  
  TFile *outfile = new TFile(outputfile,"RECREATE");
   
  dTheta_x_nocut->Write();
  dTheta_y_nocut->Write();
  d0x_h->Write();
  d0y_h->Write();
  d0x_vs_d0y_h->Write();
  ThetaIn_x->Write();
  ThetaIn_y->Write();
  myfile.close();  
    
    
    
    
    

  //analisi picco


  TCanvas *c1 = new TCanvas();
  c1->SetTitle("resolution");
  gStyle->SetOptFit(1);

  Double_t par[3]={0};
  TF1 *g1 = new TF1("g1","gaus",-50,50);
 
  g1->SetLineColor(kRed);
  dTheta_x_nocut->Fit(g1,"R");
 
  g1->GetParameters(&par[0]);
  dTheta_x_nocut->Draw();

  cout << "fit parameter" << endl;
  cout <<"constant " << par[0] << endl;
  cout <<"mean " << par[1] << endl;
  cout <<"sigma " << par[2] << endl;


  
  TCanvas *c2 = new TCanvas();
  c2->SetTitle("resolution");
  
  Double_t para[3]={0};
  TF1 *g2    = new TF1("g2","gaus",-50,50);
 
  g2->SetLineColor(kBlue);
  dTheta_y_nocut->Fit(g2,"R");
 
  g2->GetParameters(&para[0]);
  dTheta_y_nocut->Draw();

  cout << "fit parameter" << endl;
  cout <<"constant " << para[0] << endl;
  cout <<"mean " << para[1] << endl;
  cout <<"sigma " << para[2] << endl;
  
    

  TCanvas *c22 = new TCanvas();
  c22->SetTitle("beam profile");
  gStyle->SetOptFit(1);
    
  c22->Divide(2,1);
  c22->cd(1);
  

  Double_t parx[3]={0};
  TF1 *gx    = new TF1("gx","gaus",-50,50);
 

  gx->SetLineColor(kRed);
  
  d0x_h->Fit(gx,"R");
 
  gx->GetParameters(&parx[0]);
  d0x_h->Draw();

  cout << "fit parameter" << endl;
  cout <<"constant " << parx[0] << endl;
  cout <<"mean " << parx[1] << endl;
  cout <<"sigma " << parx[2] << endl;


  c22->cd(2);

  Double_t pary[3]={0};
  TF1 *gy    = new TF1("gy","gaus",-50,50);
 

  gy->SetLineColor(kRed);
  
  d0y_h->Fit(gy,"R");
 
  gy->GetParameters(&pary[0]);
  d0y_h->Draw();

  cout << "fit parameter" << endl;
  cout <<"constant " << pary[0] << endl;
  cout <<"mean " << pary[1] << endl;
  cout <<"sigma " << pary[2] << endl;

  
    
  TCanvas *c3 = new TCanvas();
  c3->SetTitle("beam spot");

  d0x_vs_d0y_h->Draw("colz");
  
    
    
    
  TCanvas *c4 = new TCanvas();
  c4->SetTitle("beam spot");
 
  c4->Divide(2,1);
  c4->cd(1);

  Double_t parthx[3]={0};
  TF1 *gthx    = new TF1("gthx","gaus",-100,100);
 

  gthx->SetLineColor(kRed);
  
  ThetaIn_x->Fit(gthx,"R");
 
  gthx->GetParameters(&parthx[0]);
  ThetaIn_x->Draw();

  cout << "fit parameter" << endl;
  cout <<"constant " << parthx[0] << endl;
  cout <<"mean " << parthx[1] << endl;
  cout <<"sigma " << parthx[2] << endl;

  
  c4->cd(2);

  Double_t parthy[3]={0};
  TF1 *gthy    = new TF1("gthy","gaus",-100,100);
 

  gthy->SetLineColor(kRed);
  
  ThetaIn_y->Fit(gthy,"R");
 
  gthy->GetParameters(&parthy[0]);
  ThetaIn_y->Draw();

  cout << "fit parameter" << endl;
  cout <<"constant " << parthy[0] << endl;
  cout <<"mean " << parthy[1] << endl;
  cout <<"sigma " << parthy[2] << endl;

    
    // Grafici nuovi:
    /*
 
    TCanvas *c8 = new TCanvas("c8","Grafico_prova2",1100,1100);
    c8->SetTitle("Grafico_prova2");
    
    dTheta_x_vs_Impact_x_nocut->Draw("colz");
    
    
    TCanvas *c101 = new TCanvas("c101","Grafico_prova4",1100,1100);
    c101->SetTitle("Grafico_prova4");
    
    dTheta_x_vs_d0y_nocut->Draw("colz");
    
    TCanvas *c102 = new TCanvas("c102","Grafico_prova4",1100,1100);
    c102->SetTitle("Grafico_prova4");
    
    dTheta_x_vs_d0y_nocut->Draw("colz");
    
    TCanvas *c103 = new TCanvas("c103","Grafico_prova4",1100,1100);
    c103->SetTitle("Grafico_prova4");
    
    dTheta_x_vs_d0y_nocut->Draw("colz");
    
    TCanvas *c104 = new TCanvas("c104","Grafico_prova4",1100,1100);
    c104->SetTitle("Grafico_prova4");
    
    dTheta_x_vs_d0y_nocut->Draw("colz");


*/

  cout<<"Processing finished!"<<endl;
    
  
}  //close macro

