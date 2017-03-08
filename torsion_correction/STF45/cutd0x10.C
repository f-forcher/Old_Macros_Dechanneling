
#include "librerie.h"

//analisi su dimensione cristallo vs fascio -> tagli / analisi corona per nucleari elastici
void cutd0x10(){

  ofstream myfile;
    
    
  // Input file name
 
   TString fileName("recoDataSimple_410_1-100");
  
  
   TString outputfile = "recoDataSimple_410_1-37.impactcut.histo.root"; 
  
  fileName += ".root"; 
      
  //Output file name
  myfile.open("roottext.txt");


    // ROOT stored Variables ****************************************************************
    //
    // Variable names are
    //      evt             -> event information
    //                              -> evt.run       run number
    //                              -> evt.evtnum    event number
    //                              -> evt.nuclear   lateral trigger event
    //      date            -> char array of event date in DDMMYY format
    //
    //      time            -> char array of event time in HHMMSS format
    //
    //      gPos            -> goniometer coordinates 
    //                              ->  gPos.x       horizontal rotation [rad]
    //                              ->  gPos.y       tilt [rad]
    //                              ->  gPos.z       lateral translation [mm]
    //      hits            -> set of hit parameters per event where nhits is number of hits recorded in the event
    //                              ->  hits[i].plane[nhits]          plane number 0->4 (0 is upstream)
    //                              ->  hits[i].local_x[nhits]        local position in x [mm]
    //                              ->  hits[i].local_y[nhits]        local position in y [mm]
    //                              ->  hits[i].sensor_x[nhits]       sensor position in x [strips]
    //                              ->  hits[i].sensor_y[nhits]       sensor position in y [strips]
    //                              ->  hits[i].charge_x[nhits]       signal charge in x [adcs]
    //                              ->  hits[i].charge_y[nhits]       signal charge in y [adcs]
    //      tracks          -> set of track parameters per event where ntracks is number of tracks recorded in the event
    //                              ->  tracks[i].thetaIn_x[0]        incoming theta in x [rad]
    //                              ->  tracks[i].thetaIn_x[1]        incoming theta in y [rad]
    //                              ->  tracks[i].thetaIn_x[2]        outgoing theta in x [rad]
    //                              ->  tracks[i].thetaIn_x[3]        outgoing theta in y [rad]
    //                              ->  tracks[i].thetaIn_x[4]        incoming theta error in x (nominal) [rad]    
    //                              ->  tracks[i].thetaIn_x[5]        incoming theta error in y (nominal) [rad] 
    //                              ->  tracks[i].thetaIn_x[6]        outgoing theta error in x (nominal) [rad] 
    //                              ->  tracks[i].thetaIn_x[7]        outgoing theta error in y (nominal) [rad] 
    //                              ->  tracks[i].thetaIn_x[8]        impact point on xtal in x [mm]
    //                              ->  tracks[i].thetaIn_x[9]        impact point on xtal in y [mm]
    //                              ->  tracks[i].thetaIn_x[10]       impact point error on xtal in x [mm]
    //                              ->  tracks[i].thetaIn_x[11]       impact point error on xtal in y [mm]
    //                              ->  tracks[i].thetaIn_x[12]       chi2 of fit in x
    //                              ->  tracks[i].thetaIn_x[13]       chi2 of fit in y
    

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
    
  struct Hit
    {
       int plane[100];
       double local_x[100];
       double local_y[100];
       double sensor_x[100];
       double sensor_y[100];
       int charge_x[100];
       int charge_y[100];
    } hits;
    
  struct Track
    {
       double thetaIn_x[100];
       double thetaIn_y[100];
       double thetaOut_x[100];
       double thetaOut_y[100];
       double thetaInErr_x[100];
       double thetaInErr_y[100];
       double thetaOutErr_x[100];
       double thetaOutErr_y[100];
       double d0_x[100];
       double d0_y[100];
       double d0Err_x[100];
       double d0Err_y[100];
       double chi2_x[100];
       double chi2_y[100];
    } tracks;


  int nhits;
  int ntracks;
    
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
  TBranch * bnhits = tree->GetBranch("nHits");
  TBranch * bHits = tree->GetBranch("Hits");
  TBranch * bntracks = tree->GetBranch("nTracks");
  TBranch * bTracks = tree->GetBranch("Tracks");
    
  bEvent->SetAddress(&evt);
  bTime->SetAddress(&time);
  bDate->SetAddress(&date);
  bGonioPos->SetAddress(&gPos);
  bnhits->SetAddress(&nhits);
  bHits->SetAddress(&hits);
  bntracks->SetAddress(&ntracks);
  bTracks->SetAddress(&tracks);
  

  //variable daclaration

  Double_t init_event = 20.;
  Double_t Maxd0x = 3.0 ; 
  Double_t Maxd0y = 20. ; 
  Double_t mind0x =-3.0 ; 
  Double_t mind0y = -20.; 
  //Double_t gonioMax = 11.0; 
  //Double_t goniomin = 5.0; 
  //Double_t gonioMax_2 = 5.0; 
  //Double_t goniomin_2 = 0.0; 
  //Double_t gonioMax_3 = 38.0; 
  //Double_t goniomin_3 = 35.0; 
  //Double_t ThetaInMax = 2.0e-6; 
  //Double_t ThetaInmin =-2.0e-6;
  
  double init_scan = 4712983.0; 
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
  TH2D * dTheta_x_vs_d0x_nocut = new TH2D("dTheta_x_vs_d0x_nocut", "Defelction x vs d0x nocut" ,400, -10., 10., 600,-200.,400.);
  dTheta_x_vs_d0x_nocut->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  dTheta_x_vs_d0x_nocut->GetXaxis()->SetTitle("x [mm]");
  TH2D * dTheta_y_vs_d0x_nocut = new TH2D("dTheta_y_vs_d0x_nocut", "Defelction y vs d0x nocut" ,400, -10., 10., 600,-200.,400.);
  dTheta_y_vs_d0x_nocut->GetYaxis()->SetTitle("#Delta#theta_{y} [#murad]");
  dTheta_y_vs_d0x_nocut->GetXaxis()->SetTitle("x [mm]");
  TH2D * dTheta_x_vs_d0y_nocut = new TH2D("dTheta_x_vs_d0y_nocut", "Defelction x vs d0y nocut", 400., -10., 10., 600,-200.,400.);
  dTheta_x_vs_d0y_nocut->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  dTheta_x_vs_d0y_nocut->GetXaxis()->SetTitle("y [mm]");
  TH2D * dTheta_y_vs_d0y_nocut = new TH2D("dTheta_y_vs_d0y_nocut", "Defelction y vs d0y nocut", 400., -10., 10., 600,-200.,400.);
  dTheta_y_vs_d0y_nocut->GetYaxis()->SetTitle("#Delta#theta_{y} [#murad]");
  dTheta_y_vs_d0y_nocut->GetXaxis()->SetTitle("y [mm]");


  //TH2D * dTheta_x_vs_dTheta_y_chcut = new TH2D("dTheta_x_vs_dTheta_y_chcut", "Defelction x vs Defelction y xtal cut",600,-200.,400.,600,-200.,400.);
  //dTheta_x_vs_dTheta_y_chcut->GetXaxis()->SetTitle("#theta_{x} [#murad]");
  //dTheta_x_vs_dTheta_y_chcut->GetYaxis()->SetTitle("#theta_{y} [#murad]");
  TH2D * dTheta_x_vs_dTheta_y_nocut = new TH2D("dTheta_x_vs_dTheta_y_nocut", "Defelction y vs Defelction x no cut",600,-200.,400., 600,-200.,400.);
  dTheta_x_vs_dTheta_y_nocut->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  dTheta_x_vs_dTheta_y_nocut->GetYaxis()->SetTitle("#Delta#theta_{y} [#murad]");
  //  TH2D * dTheta_x_vs_dTheta_y_corona = new TH2D("dTheta_x_vs_dTheta_y_corona", "Defelction x vs Defelction y corona",400,-200.,200.,400,-200.,200.);
  /*
 TH2D * dTheta_x_vs_d0y_anglecut = new TH2D("dTheta_x_vs_d0y_anglecut", "Defelction x vs d0y anglecut", 800., -10., 10, 600,-200.,400.);
  dTheta_x_vs_d0y_anglecut->GetYaxis()->SetTitle("#theta_{x} [#murad]");
  dTheta_x_vs_d0y_anglecut->GetXaxis()->SetTitle("y [mm]");

  TH2D * ThetaOut_x_vs_d0y_anglecut = new TH2D("ThetaOut_x_vs_d0y_anglecut", "Theta Out x vs d0y anglecut", 800., -10., 10, 400,-200.,200.);
  ThetaOut_x_vs_d0y_anglecut->GetYaxis()->SetTitle("#theta_Out_{x} [#murad]");
  ThetaOut_x_vs_d0y_anglecut->GetXaxis()->SetTitle("y [mm]");
  */
  TH2D * ThetaOut_x_vs_d0y = new TH2D("ThetaOut_x_vs_d0y", "Theta Out x vs d0y ", 800., -10., 10, 400,-200.,200.);
  ThetaOut_x_vs_d0y->GetYaxis()->SetTitle("#theta_{Out}(x) [#murad]");
  ThetaOut_x_vs_d0y->GetXaxis()->SetTitle("y [mm]");


  TH2D * dTheta_x_vs_d0y_ch = new TH2D("dTheta_x_vs_d0y_ch", "Defelction x vs d0y ch" ,240, -3., 3., 55,125.,180.);
  dTheta_x_vs_d0y_ch->GetYaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  dTheta_x_vs_d0y_ch->GetXaxis()->SetTitle("y [mm]");

  TH2D * ThetaOut_x_vs_d0y_ch = new TH2D("ThetaOut_x_vs_d0y_ch", "Theta Out x vs d0y ch",240, -3., 3., 60,120.,180.);
  ThetaOut_x_vs_d0y_ch->GetYaxis()->SetTitle("#theta_{Out}(x) [#murad]");
  ThetaOut_x_vs_d0y_ch->GetXaxis()->SetTitle("y [mm]");
  //  TH2D * dTheta_x_vs_dTheta_y_corona = new TH2D("dTheta_x_vs_dTheta_y_corona", "Defelction x vs Defelction y corona",400,-200.,200.,400,-200.,200.);



  //TH2D * dTheta_x_vs_d0x_d0ycut  = new TH2D("dTheta_x_vs_d0x_d0ycut", " d0x vs dTheta up " , 1400,-400.,1000.,23., mind0x, Maxd0x);
  //TH2D * dTheta_x_vs_gonio_d0ycut  = new TH2D("dTheta_x_vs_gonio_d0ycut", " gonio vs dTheta up d0ycut" , 1400,-400.,1000.,100., 0., 100.);
  //TH2D * dTheta_x_vs_gonio_d0ycut_d0xcut = new TH2D("dTheta_x_vs_gonio_d0ycut_d0xcut" , " gonio vs dTheta up  d0ycut d0xcut" , 1400,-400.,1000.,100., 0., 100.);
  
  
  //TH2D * dTheta_x_vs_d0x_nocut = new TH2D("dTheta_x_vs_d0x_nocut" , " dTheta_x_vs_d0x_nocut" , 1400,-400.,1000.,100., 0., 100.);
  //TH2D * dTheta_x_vs_gonio_nocut = new TH2D("dTheta_x_vs_gonio_nocut" , " dTheta_x_vs_gonio_nocut" , 1400,-400.,1000.,2000., 4712000., 4714000. );
  //TH2D * dTheta_x_vs_gonio_nocut_zero = new TH2D("dTheta_x_vs_gonio_nocut_zero" , " dTheta_x_vs_gonio_nocut_zero" , 1400,-400.,1000.,500., 0., 100.);
  //TH2D * dTheta_x_vs_gonio_mycut_test = new TH2D("dTheta_x_vs_gonio_mycut_test" , " dTheta_x_vs_gonio_mycut_test" , 1400,-400.,1000.,100., 0., 100.);
  //TH2D * dTheta_x_vs_Impact_x_nocut = new TH2D("dTheta_x_vs_Impact_x_nocut" , " dTheta_x_vs_Impact_x_nocut " , 1400,-400.,1000.,500., 0., 100.);
  //TH2D * Impact_x_vs_d0y = new TH2D("Impact_x_vs_d0y", "Impact_x_vs_d0y",100,0.,100.,30,-3.,3.);
  
  
  //TH1D * dTheta_x_nocut = new TH1D("dTheta_x_nocut","dTheta x no cut",400,-200.,200.);
  //TH1D * dTheta_y_nocut = new TH1D("dTheta_y_nocut","dTheta y no cut",400,-200.,200.);
  //TH1D * dTheta_x_goniocut_d0ycut = new TH1D("dTheta_x_goniocut_d0ycut","dTheta x goniocut d0y cut",1400,-400.,1000.);
  //TH1D * dTheta_x_d0ycut = new TH1D("dTheta_x_d0ycut","dTheta x d0y cut",1400,-400.,1000.);
  //TH1D * dTheta_x_d0ycut_d0xcut = new TH1D("dTheta_x_d0ycut_d0xcut","dTheta x d0y cut d0x cut",1400,-400.,1000.);
 

  int count_1track = 0;
  int count_cut = 0;
  int count_corona = 0;

  Maxd0x = 0.3; 
  mind0x = -0.7; 
  Maxd0y = 2.89;
  mind0y = -2.869;
  Double_t ThetaInMax = 4.; 
  Double_t ThetaInmin =-6.;
  
  Double_t ThetaOutMax = 180.; 
  Double_t ThetaOutmin = 125.;
  Double_t dThetaMax = 180.; 
  Double_t dThetamin = 125.;

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
      
    

    if(ntracks==1) {
	
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
      dTheta_x_vs_d0x_nocut->Fill(tracks.thetaIn_x[8],(tracks.thetaIn_x[2] - tracks.thetaIn_x[0])*1e6); 
      dTheta_y_vs_d0x_nocut->Fill(tracks.thetaIn_x[8],(tracks.thetaIn_x[3] - tracks.thetaIn_x[1])*1e6); 
      dTheta_x_vs_d0y_nocut->Fill(tracks.thetaIn_x[9],(tracks.thetaIn_x[2] - tracks.thetaIn_x[0])*1e6);
      dTheta_y_vs_d0y_nocut->Fill(tracks.thetaIn_x[9],(tracks.thetaIn_x[3] - tracks.thetaIn_x[1])*1e6);
      dTheta_x_vs_dTheta_y_nocut->Fill((tracks.thetaIn_x[2] - tracks.thetaIn_x[0])*1e6,(tracks.thetaIn_x[3] - tracks.thetaIn_x[1])*1e6);	

      ThetaOut_x_vs_d0y->Fill(tracks.thetaIn_x[9],tracks.thetaIn_x[2]*1e6);


      angle_cut=0;
      cut_d0y=0;
      
      if (tracks.thetaIn_x[9] < Maxd0y && tracks.thetaIn_x[9] > mind0y) cut_d0y=1;
      //    if ((gPos.x+(tracks.thetaIn_x[0]*1e6)-init_scan) < ThetaInMax && (gPos.x+(tracks.thetaIn_x[0]*1e6)-init_scan) > ThetaInmin) angle_cut=1;


      /*
      if (angle_cut) {
	dTheta_x_vs_d0y_anglecut->Fill(tracks.d0_y[0],(tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6);
	ThetaOut_x_vs_d0y_anglecut->Fill(tracks.d0_y[0],tracks.thetaOut_x[0]*1e6);
	if (cut_d0y) {	
	  count_cut++;
	
	  dTheta_cut = 0;
	  if((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6 > dThetamin && (tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6 < dThetaMax) dTheta_cut = 1;
	  if (dTheta_cut) {
	    dTheta_x_vs_d0y_ch_anglecut->Fill(tracks.d0_y[0],(tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6);
	  }
	  
	  thetaOut_cut = 0;
	  if((tracks.thetaOut_x[0])*1e6 > ThetaOutmin && (tracks.thetaOut_x[0])*1e6 < ThetaOutMax) thetaOut_cut = 1;
	  if (dTheta_cut) {
	    ThetaOut_x_vs_d0y_ch_anglecut->Fill(tracks.d0_y[0],(tracks.thetaOut_x[0])*1e6);
	  }
	}
	}*/

 
      //if (tracks.d0_x[0] < Maxd0x && tracks.d0_x[0] > mind0x) cut_d0x=1;
      
      if (cut_d0y) {	
	count_cut++;
	
	dTheta_cut = 0;
	if((tracks.thetaIn_x[2] - tracks.thetaIn_x[0])*1e6 > dThetamin && (tracks.thetaIn_x[2] - tracks.thetaIn_x[0])*1e6 < dThetaMax) dTheta_cut = 1;
	if (dTheta_cut) {
	  dTheta_x_vs_d0y_ch->Fill(tracks.thetaIn_x[9],(tracks.thetaIn_x[2] - tracks.thetaIn_x[0])*1e6);
	}

	thetaOut_cut = 0;
	if((tracks.thetaIn_x[2])*1e6 > ThetaOutmin && (tracks.thetaIn_x[2])*1e6 < ThetaOutMax) thetaOut_cut = 1;
	if (dTheta_cut) {
	  ThetaOut_x_vs_d0y_ch->Fill(tracks.thetaIn_x[9],(tracks.thetaIn_x[2])*1e6);
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
   
 TCanvas *c20 = new TCanvas("c20","Deflection y vs Deflection x nocut",1100,1100);
  c20->SetTitle("Deflection y vs Deflection x  nocut");
  dTheta_x_vs_dTheta_y_nocut->Draw("colz");
  
  /*
  TCanvas *c3 = new TCanvas();
  c3->SetTitle("Deflection y vs Deflection Y corona");
  dTheta_x_vs_dTheta_y_corona->Draw("colz");
  */
 
  

  /*
  //analisi picco

  TCanvas *c10 = new TCanvas();
  c10->SetTitle("Deflection x vs Deflection y chcut");
  dTheta_x_vs_dTheta_y_chcut->Draw("colz");
 
 
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

 
 TCanvas *c30 = new TCanvas();
  c30->SetTitle("Deflection x vs impact point Y ch");
  dTheta_x_vs_d0y_ch->Draw("colz");

  TCanvas *c31 = new TCanvas();
  c31->SetTitle("Theta Out x vs impact point Y ch");
  ThetaOut_x_vs_d0y_ch->Draw("colz");
  /*
  TCanvas *c32 = new TCanvas();
  c32->SetTitle("Deflection x vs impact point Y ch anglecut");
  dTheta_x_vs_d0y_ch_anglecut->Draw("colz");

  TCanvas *c33 = new TCanvas();
  c33->SetTitle("Theta Out x vs impact point Y ch anglecut");
  ThetaOut_x_vs_d0y_ch_anglecut->Draw("colz");
  
  TCanvas *c34 = new TCanvas();
  c34->SetTitle("Deflection x vs impact point Y");
  dTheta_x_vs_d0y_ch_anglecut->Draw("colz");
  */
  TCanvas *c35 = new TCanvas();
  c35->SetTitle("Theta Out x vs impact point Y");
  ThetaOut_x_vs_d0y->Draw("colz");
 


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
  dTheta_x_vs_dTheta_y_nocut->Write();
  //dTheta_x_vs_dTheta_y_corona->Write();
  //h1->Write();  
  //dTheta_x_vs_d0y_anglecut->Write();
  //ThetaOut_x_vs_d0y_anglecut->Write();
  dTheta_x_vs_d0y_ch->Write();
  ThetaOut_x_vs_d0y_ch->Write();
  //dTheta_x_vs_d0y_ch_anglecut->Write();
  ThetaOut_x_vs_d0y->Write();
  myfile.close();
  
  
  cout<<"Processing finished!"<<endl;
    
}  //close macro

