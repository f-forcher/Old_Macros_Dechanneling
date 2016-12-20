
#include "librerie.h"

//analisi su dimensione cristallo vs fascio -> tagli / analisi corona per nucleari elastici
//inserire HiStatCH o HiStatAM


void cutd0x12(){

  ofstream myfile;  

    TString fileName("/run/media/dasukhon/6094557b-7c6a-4bfd-9116-c0c405e03e59/runs_TB/TB_29_04_15_recodata/recoDataSimple_2260");


  TString outputfile = "recoDataSimple_2260.impactcut.histo.root";
  
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
  Double_t Maxd0x = 5.0 ; 
  Double_t Maxd0y = 20. ; 
  Double_t mind0x =-5.0 ; 
  Double_t mind0y = -20.; 
  //Double_t gonioMax = 11.0; 
  //Double_t goniomin = 5.0; 
  //Double_t gonioMax_2 = 5.0; 
  //Double_t goniomin_2 = 0.0; 
  //Double_t gonioMax_3 = 38.0; 
  //Double_t goniomin_3 = 35.0; 
  //Double_t ThetaInMax = 2.0e-6; 
  //Double_t ThetaInmin =-2.0e-6;
  
  double init_scan = 1584010.74;
  //double init_scan = 4712945.0;
  //double init_scan = 4712980.0; 
  int chentries = 4889340;  
  int init_am = 4889341; 

   
   
  //histogram declaration
   
  //histogram declaration

  // TH1D d0_x_h = TH1D("d0_x_h", " Impact point at crystal x ",600,-100.,100.);
  //TH1D d0_y_h = TH1D("d0_y_h", " Impact point at crystal y ",600,-100.,100.);
  /*  
      TH2D * dTheta_x_vs_d0x_chcut = new TH2D("dTheta_x_vs_d0x_chcut", "Defelction x vs d0x chcut" ,120, mind0x, Maxd0x, 600,-200.,400.);
      dTheta_x_vs_d0x_chcut->GetYaxis()->SetTitle("#theta_{x} [#murad]");
      dTheta_x_vs_d0x_chcut->GetXaxis()->SetTitle("x [mm]");
      TH2D * dTheta_y_vs_d0x_chcut = new TH2D("dTheta_y_vs_d0x_chcut", "Defelction y vs d0x chcut" ,120, mind0x, Maxd0x, 600,-200.,400.);
      dTheta_y_vs_d0x_chcut->GetYaxis()->SetTitle("#theta_{y} [#murad]");
      dTheta_y_vs_d0x_chcut->GetXaxis()->SetTitle("x [mm]");
      TH2D * dTheta_x_vs_d0y_chcut = new TH2D("dTheta_x_vs_d0y_chcut", "Defelction x vs d0y chcut", 200., -10., 10, 600,-200.,400.);  
      dTheta_x_vs_d0y_chcut->GetYaxis()->SetTitle("#theta_{x} [#murad]");
      dTheta_x_vs_d0y_chcut->GetXaxis()->SetTitle("y [mm]");
      TH2D * dTheta_y_vs_d0y_chcut = new TH2D("dTheta_y_vs_d0y_chcut", "Defelction y vs d0y chcut", 200., -10., 10, 600,-200.,400.);
      dTheta_y_vs_d0y_chcut->GetYaxis()->SetTitle("#theta_{y} [#murad]");
      dTheta_y_vs_d0y_chcut->GetXaxis()->SetTitle("y [mm]");
  */
  TH2D * dTheta_x_vs_d0x_nocut = new TH2D("dTheta_x_vs_d0x_nocut", "Deflection x vs d0x nocut" ,400, -20., 20., 600,-200.,400.);
  dTheta_x_vs_d0x_nocut->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  dTheta_x_vs_d0x_nocut->GetXaxis()->SetTitle("x [mm]");
  TH2D * dTheta_y_vs_d0x_nocut = new TH2D("dTheta_y_vs_d0x_nocut", "Deflection y vs d0x nocut" ,400, -20., 20., 600,-200.,400.);
  dTheta_y_vs_d0x_nocut->GetYaxis()->SetTitle("#Delta#theta_{y} [#murad]");
  dTheta_y_vs_d0x_nocut->GetXaxis()->SetTitle("x [mm]");
  TH2D * dTheta_x_vs_d0y_nocut = new TH2D("dTheta_x_vs_d0y_nocut", "Deflection x vs d0y nocut", 400., -20., 20., 600,-200.,400.);
  dTheta_x_vs_d0y_nocut->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  dTheta_x_vs_d0y_nocut->GetXaxis()->SetTitle("y [mm]");
  TH2D * dTheta_y_vs_d0y_nocut = new TH2D("dTheta_y_vs_d0y_nocut", "Deflection y vs d0y nocut", 400., -20., 20., 600,-200.,400.);
  dTheta_y_vs_d0y_nocut->GetYaxis()->SetTitle("#Delta#theta_{y} [#murad]");
  dTheta_y_vs_d0y_nocut->GetXaxis()->SetTitle("y [mm]");
  
  
  //TH2D * dTheta_x_vs_dTheta_y_chcut = new TH2D("dTheta_x_vs_dTheta_y_chcut", "Defelction x vs Defelction y xtal cut",600,-200.,400.,600,-200.,400.);
  //dTheta_x_vs_dTheta_y_chcut->GetXaxis()->SetTitle("#theta_{x} [#murad]");
  //dTheta_x_vs_dTheta_y_chcut->GetYaxis()->SetTitle("#theta_{y} [#murad]");
  TH2D * dTheta_y_vs_dTheta_x_nocut = new TH2D("dTheta_x_vs_dTheta_y_nocut", "Deflection x vs Deflection y no cut",600,-200.,400., 600,-200.,400.);
  dTheta_y_vs_dTheta_x_nocut->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  dTheta_y_vs_dTheta_x_nocut->GetYaxis()->SetTitle("#Delta#theta_{y} [#murad]");
  //  TH2D * dTheta_x_vs_dTheta_y_corona = new TH2D("dTheta_x_vs_dTheta_y_corona", "Defelction x vs Defelction y corona",400,-200.,200.,400,-200.,200.);

  TH2D * ThetaOut_x_vs_d0y = new TH2D("ThetaOut_x_vs_d0y", "Theta Out x vs d0y ", 400., -20., 20, 400,-200.,400.);
  ThetaOut_x_vs_d0y->GetYaxis()->SetTitle("#theta_{Out}(x) [#murad]");
  ThetaOut_x_vs_d0y->GetXaxis()->SetTitle("y [mm]");


  TH2D * dTheta_x_vs_d0y_ch = new TH2D("dTheta_x_vs_d0y_ch", "Deflection x vs d0y ch" ,200, -6., 6., 80,20.,100.);
  dTheta_x_vs_d0y_ch->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  dTheta_x_vs_d0y_ch->GetXaxis()->SetTitle("y [mm]");

  TH2D * ThetaOut_x_vs_d0y_ch = new TH2D("ThetaOut_x_vs_d0y_ch", "Theta Out x vs d0y ch",200, -6., 6., 80,20.,100.);
  ThetaOut_x_vs_d0y_ch->GetYaxis()->SetTitle("#theta_{Out}(x) [#murad]");
  ThetaOut_x_vs_d0y_ch->GetXaxis()->SetTitle("y [mm]");


  //TH2D * dTheta_x_vs_d0x_d0ycut  = new TH2D("dTheta_x_vs_d0x_d0ycut", " d0x vs dTheta up " , 1400,-400.,1000.,23., mind0x, Maxd0x);
  //TH2D * dTheta_x_vs_gonio_d0ycut  = new TH2D("dTheta_x_vs_gonio_d0ycut", " gonio vs dTheta up d0ycut" , 1400,-400.,1000.,100., 0., 100.);
  //TH2D * dTheta_x_vs_gonio_d0ycut_d0xcut = new TH2D("dTheta_x_vs_gonio_d0ycut_d0xcut" , " gonio vs dTheta up  d0ycut d0xcut" , 1400,-400.,1000.,100., 0., 100.);
  
  
  //TH2D * dTheta_x_vs_d0x_nocut = new TH2D("dTheta_x_vs_d0x_nocut" , " dTheta_x_vs_d0x_nocut" , 1400,-400.,1000.,100., 0., 100.);
  //TH2D * dTheta_x_vs_gonio_nocut = new TH2D("dTheta_x_vs_gonio_nocut" , " dTheta_x_vs_gonio_nocut" , 1400,-400.,1000.,2000., 4712000., 4714000. );
  //TH2D * dTheta_x_vs_gonio_nocut_zero = new TH2D("dTheta_x_vs_gonio_nocut_zero" , " dTheta_x_vs_gonio_nocut_zero" , 1400,-400.,1000.,500., 0., 100.);
  //TH2D * dTheta_x_vs_gonio_mycut_test = new TH2D("dTheta_x_vs_gonio_mycut_test" , " dTheta_x_vs_gonio_mycut_test" , 1400,-400.,1000.,100., 0., 100.);
  TH2D * dTheta_x_vs_Impact_x_nocut = new TH2D("dTheta_x_vs_Impact_x_nocut" , " dTheta_x_vs_Impact_x_nocut " , 1400,-500.,500.,500., -500., 500.);
  dTheta_x_vs_Impact_x_nocut->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  dTheta_x_vs_Impact_x_nocut->GetXaxis()->SetTitle("#theta_{In}(x) [#murad]");
  //TH2D * Impact_x_vs_d0y = new TH2D("Impact_x_vs_d0y", "Impact_x_vs_d0y",100,0.,100.,30,-3.,3.);
  
  
  //TH1D * dTheta_x_nocut = new TH1D("dTheta_x_nocut","dTheta x no cut",400,-200.,200.);
  //TH1D * dTheta_y_nocut = new TH1D("dTheta_y_nocut","dTheta y no cut",400,-200.,200.);
  //TH1D * dTheta_x_goniocut_d0ycut = new TH1D("dTheta_x_goniocut_d0ycut","dTheta x goniocut d0y cut",1400,-400.,1000.);
  //TH1D * dTheta_x_d0ycut = new TH1D("dTheta_x_d0ycut","dTheta x d0y cut",1400,-400.,1000.);
  //TH1D * dTheta_x_d0ycut_d0xcut = new TH1D("dTheta_x_d0ycut_d0xcut","dTheta x d0y cut d0x cut",1400,-400.,1000.);
 

  int count_1track = 0;
  int count_cut = 0;
  int count_corona = 0;

  Maxd0x = 0.4; 
  mind0x = -0.2; 
  Maxd0y = 6.;
  mind0y = -6.;
  Double_t ThetaInMax = 4.;
  Double_t ThetaInmin =-6.;
  
  Double_t ThetaOutMax = 100.;
  Double_t ThetaOutmin = 20.;
  Double_t dThetaMax = 100.;
  Double_t dThetamin = 20.;

  int cut_d0y(0), cut_d0x(0), angle_cut(0), dTheta_cut(0), thetaOut_cut(0); 
  Double_t radius = 0.;

  //read and fill

  for (Int_t i = 0; i < nentries; i++) {
    if(i%50000==0) cout  << "event " << i << endl;

    Long64_t tentry = tree->LoadTree(i);
   tree->GetEntry(i);
		
    radius = 0.;
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
      //    if (tracks.thetaIn_x[0] < ThetaInMax && tracks.thetaIn_x[0] > ThetaInmin) angle_cut=1;
      //if (tracks.d0_x[0] < Maxd0x && tracks.d0_x[0] > mind0x) cutd0x=1;


		
      //init_position scan 
    // double init_angle = 4.241401e+06;    

    //radius = sqrt(pow((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6,2)+pow((tracks.thetaOut_y[0] - tracks.thetaIn_y[0])*1e6,2));
      
    

    if(singleTrack==1) {
	
      count_1track++;
	
	/*
	int cutd0y(0),goniocut(0),goniocut_2(0),goniocut_3(0);
	//cout << gPos.x-init_scan << endl;
	if(tracks.d0_y[0] < Maxd0y && tracks.d0_y[0] > mind0y) cutd0y=1;
	if((gPos.x-init_scan) < gonioMax && (gPos.x-init_scan) > goniomin) goniocut =1;     
	if((gPos.x-init_scan) < gonioMax_2 && (gPos.x-init_scan) > goniomin_2) goniocut_2 =1;
	if((gPos.x-init_scan) < gonioMax_3 && (gPos.x-init_scan) > goniomin_3) goniocut_3 =1;	
	//cout << cutd0y << " " << goniocut_2 << endl;
	*/

	//fill no cut histo
      dTheta_x_vs_d0x_nocut->Fill(tracks.d0_x,(tracks.thetaOut_x - tracks.thetaIn_x)*1e6); 
      dTheta_y_vs_d0x_nocut->Fill(tracks.d0_x,(tracks.thetaOut_y - tracks.thetaIn_y)*1e6); 
      dTheta_x_vs_d0y_nocut->Fill(tracks.d0_y,(tracks.thetaOut_x - tracks.thetaIn_x)*1e6);
      dTheta_y_vs_d0y_nocut->Fill(tracks.d0_y,(tracks.thetaOut_y - tracks.thetaIn_y)*1e6);
      dTheta_y_vs_dTheta_x_nocut->Fill((tracks.thetaOut_x - tracks.thetaIn_x)*1e6,(tracks.thetaOut_y - tracks.thetaIn_y)*1e6);	
       
      dTheta_x_vs_Impact_x_nocut->Fill(tracks.thetaIn_x*1e6,(tracks.thetaOut_x - tracks.thetaIn_x)*1e6); 


      ThetaOut_x_vs_d0y->Fill(tracks.d0_y,(tracks.thetaOut_x)*1e6);
       
      cut_d0y=0;
      
      if (tracks.d0_y < Maxd0y && tracks.d0_y > mind0y) cut_d0y=1;
      if ((gPos.x+(tracks.thetaIn_x*1e6)-init_scan) < ThetaInMax && (gPos.x+(tracks.thetaIn_x*1e6)-init_scan) > ThetaInmin) angle_cut=1;
      /*
      if (angle_cut) {

      dTheta_x_vs_d0y_anglecut->Fill(tracks.d0_y,(tracks.thetaOut_x - tracks.thetaIn_x)*1e6);
      ThetaOut_x_vs_d0y_anglecut->Fill(tracks.d0_y,tracks.thetaOut_x*1e6);
      if (cut_d0y) {
      
        dTheta_cut = 0;
	  if((tracks.thetaOut_x - tracks.thetaIn_x)*1e6 > dThetamin && (tracks.thetaOut_x - tracks.thetaIn_x)*1e6 < dThetaMax) dTheta_cut = 1;
	    if (dTheta_cut) {
	        dTheta_x_vs_d0y_ch_anglecut->Fill(tracks.d0_y,(tracks.thetaOut_x - tracks.thetaIn_x)*1e6);
		  }
		    
		    thetaOut_cut = 0;
		      if((tracks.thetaOut_x)*1e6 > ThetaOutmin && (tracks.thetaOut_x)*1e6 < ThetaOutMax) thetaOut_cut = 1;
		        if (dTheta_cut) {
			    ThetaOut_x_vs_d0y_ch_anglecut->Fill(tracks.d0_y,(tracks.thetaOut_x)*1e6);
			      } 
			      }
			      }*/
      
      cut_d0x=0;
    
      if (tracks.d0_x < Maxd0x && tracks.d0_x > mind0x) cut_d0x=1;
      
      if (cut_d0y && cut_d0x) {
	count_cut++;
	
	dTheta_cut = 0;
	if((tracks.thetaOut_x - tracks.thetaIn_x)*1e6 > dThetamin && (tracks.thetaOut_x - tracks.thetaIn_x)*1e6 < dThetaMax) dTheta_cut = 1;
	if (dTheta_cut) {
	  dTheta_x_vs_d0y_ch->Fill(tracks.d0_y,(tracks.thetaOut_x - tracks.thetaIn_x)*1e6);
	}

	thetaOut_cut = 0;
	if((tracks.thetaOut_x)*1e6 > ThetaOutmin && (tracks.thetaOut_x)*1e6 < ThetaOutMax) thetaOut_cut = 1;
	if (dTheta_cut) {
	  ThetaOut_x_vs_d0y_ch->Fill(tracks.d0_y,(tracks.thetaOut_x)*1e6);
	}



      }

      /*
      cut_d0x=0;
      cut_d0y=0;
      
      if (tracks.d0_y[0] < Maxd0y && tracks.d0_y[0] > mind0y) cut_d0y=1;
      if (tracks.d0_x[0] < Maxd0x && tracks.d0_x[0] > mind0x) cut_d0x=1;
      
      if (cut_d0y && cut_d0x) {	
	count_cut++;
	dTheta_x_vs_d0x_chcut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6,tracks.d0_x[0]); 
	dTheta_y_vs_d0x_chcut->Fill((tracks.thetaOut_y[0] - tracks.thetaIn_y[0])*1e6,tracks.d0_x[0]); 
	dTheta_x_vs_d0y_chcut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6,tracks.d0_y[0]);
	dTheta_y_vs_d0y_chcut->Fill((tracks.thetaOut_y[0] - tracks.thetaIn_y[0])*1e6,tracks.d0_y[0]);
	dTheta_x_vs_dTheta_y_chcut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6,(tracks.thetaOut_y[0] - tracks.thetaIn_y[0])*1e6);
	
		
	if (radius < 200. && radius >100.) {
	  count_corona++;
	  dTheta_x_vs_dTheta_y_corona->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6,(tracks.thetaOut_y[0] - tracks.thetaIn_y[0])*1e6);
	  }

	

	  }*/


//dTheta_x_vs_gonio_nocut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6 ,gPos.x+tracks.thetaIn_x[0]);
	//dTheta_x_vs_gonio_nocut_zero->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0] )*1e6,gPos.x+(tracks.thetaIn_x[0]*1e6)-init_scan);
	

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
    }//close cycle on nentries

  cout<<"events with 1 track "<< count_1track << endl;
  cout<<"events in cut"<< count_cut <<endl; 
  



    
  TCanvas *c10 = new TCanvas();
  c10->SetTitle("Deflection x vs Impact x chcut");
  dTheta_x_vs_Impact_x_nocut->Draw("colz");
 
  
  TCanvas *c20 = new TCanvas("c20","Deflection x vs Deflection y nocut",1100,1100);
  c20->SetTitle("Deflection x vs Deflection y nocut");
  dTheta_y_vs_dTheta_x_nocut->Draw("colz");
  
  /*
  TCanvas *c3 = new TCanvas();
  c3->SetTitle("Deflection y vs Deflection Y corona");
  dTheta_x_vs_dTheta_y_corona->Draw("colz");
  */
 
  


  //analisi picco

  /*
  TCanvas *c1 = new TCanvas();
  c1->SetTitle("Deflection x vs impact point X chcut");
  dTheta_x_vs_d0x_chcut->Draw("colz");
 
  
  TCanvas *c2 = new TCanvas();
  c2->SetTitle("Deflection x vs impact point y  chcut");
  dTheta_x_vs_d0y_chcut->Draw("colz");
  

  TCanvas *c3 = new TCanvas();
  c3->SetTitle("Deflection y vs impact point Y chcut");
  dTheta_y_vs_d0y_chcut->Draw("colz");


  TCanvas *c4 = new TCanvas();
  c3->SetTitle("Deflection y vs impact point X chcut");
  dTheta_y_vs_d0x_chcut->Draw("colz");
  
  */
  TCanvas *c6 = new TCanvas();
  c6->SetTitle("Deflection x vs impact point X nocut");
  dTheta_x_vs_d0x_nocut->Draw("colz");
 
  
  TCanvas *c7 = new TCanvas();
  c7->SetTitle("Deflection x vs impact point y nocut");
  dTheta_x_vs_d0y_nocut->Draw("colz");
  

  TCanvas *c8 = new TCanvas();
  c8->SetTitle("Deflection y vs impact point Y nocut");
  dTheta_y_vs_d0y_nocut->Draw("colz");


  TCanvas *c9 = new TCanvas();
  c9->SetTitle("Deflection y vs impact point X nocut");
  dTheta_y_vs_d0x_nocut->Draw("colz");
  
 
/*


  TCanvas *c30 = new TCanvas();
  c30->SetTitle("Deflection x vs impact point Y ch");
  dTheta_x_vs_d0y_ch->Draw("colz");

  TCanvas *c31 = new TCanvas();
  c31->SetTitle("Theta Out x vs impact point Y ch");
  ThetaOut_x_vs_d0y_ch->Draw("colz");

  TCanvas *c32 = new TCanvas();
  c32->SetTitle("Deflection x vs impact point Y");
  dTheta_x_vs_d0y_nocut->Draw("colz");

  TCanvas *c33 = new TCanvas();
  c33->SetTitle("Theta Out x vs impact point Y");
  ThetaOut_x_vs_d0y->Draw("colz");

*/

  /*
  TCanvas *c5 = new TCanvas();
  c5->SetTitle("d0y cut deflection y projection");

  
  TAxis *axis = dTheta_y_vs_d0y_chcut->GetXaxis();
  int bmin = axis->FindBin(0);
  int bmax = axis->FindBin(0); 


  TH1D *h1= dTheta_y_vs_d0y_chcut->ProjectionY("",bmin,bmax);

  h1->Draw();

  */

  TFile *outfile = new TFile(outputfile,"RECREATE");
  //dTheta_x_vs_d0x_chcut->Write();
  //dTheta_y_vs_d0x_chcut->Write();
  //dTheta_x_vs_d0y_chcut->Write();
  //dTheta_y_vs_d0y_chcut->Write();
  dTheta_x_vs_d0x_nocut->Write();
  dTheta_y_vs_d0x_nocut->Write();
  dTheta_x_vs_d0y_nocut->Write();
  dTheta_y_vs_d0y_nocut->Write();
  //dTheta_x_vs_dTheta_y_chcut->Write();
 // dTheta_y_vs_dTheta_x_nocut->Write();
  //dTheta_x_vs_dTheta_y_corona->Write();
  //h1->Write();  
 // dTheta_x_vs_d0y_ch->Write();
 // ThetaOut_x_vs_d0y_ch->Write();
  //dTheta_x_vs_d0y_ch_anglecut->Write();
// ThetaOut_x_vs_d0y->Write();

  myfile.close();
  
  
  cout<<"Processing finished!"<<endl;
    

}  //close macro

