
#include "librerie.h"



//analisi run di allineamento -> risoluzione e parametri fascio
void alignment10(){

  ofstream myfile;
    
    
  // Input file name
 

  
  // TString fileName("/Volumes/dfs/Experiments/UA9/Data-Test_beams/2010_09_16\,_H8_re-recodata/recoDataSimple_411_1-12");

  //  TString outputfile = "recoDataSimple_411.alignment.histo.root"; 
  TString fileName("recoDataSimple_406_1-8");

  TString outputfile = "recoDataSimple_406.alignment.histo.root"; 
  
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
    //      tracks	    -> set of track parameters per event where ntracks is number of tracks recorded in the event
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
  Double_t ThetaInMax = 1.0; 
  Double_t ThetaInmin = -1.0;

  int chentries = 4889340;
  int init_am = 4889341;
  double init_scan = 4712668.0;	
  //double init_scan = 4712945.0;
  //double init_scan = 4712980.0; 
  




   
  //histogram declaration


 
  TH1D * d0x_h = new TH1D("d0x_h", " Impact point at crystal x ",1000,-30.,30.);
  d0x_h->GetXaxis()->SetTitle("d_{0x} [mm]");
  TH1D * d0y_h = new TH1D("d0y_h", " Impact point at crystal y ",1000,-30.,30.);
  d0y_h->GetXaxis()->SetTitle("d_{0y} [mm]");
  TH2D * d0x_vs_d0y_h = new TH2D("d0x_vs_d0y_h","beam spot",1000,-30.,30.,1000,-30.,30.);
  d0x_vs_d0y_h->GetXaxis()->SetTitle("d_{0x} [mm]");
  d0x_vs_d0y_h->GetYaxis()->SetTitle("d_{0y} [mm]");

  //TH2D * dTheta_x_vs_d0x_goniocut_d0ycut  = new TH2D("dTheta_x_vs_d0x_goniocut_d0ycut", " d0x vs dTheta up gonio cut" , 1400,-400.,1000.,23., mind0x, Maxd0x);
  //TH2D * dTheta_x_vs_d0x_d0ycut  = new TH2D("dTheta_x_vs_d0x_d0ycut", " d0x vs dTheta up " , 1400,-400.,1000.,23., mind0x, Maxd0x);
  //TH2D * dTheta_x_vs_gonio_d0ycut  = new TH2D("dTheta_x_vs_gonio_d0ycut", " gonio vs dTheta up d0ycut" , 1400,-400.,1000.,100., 0., 100.);
  //TH2D * dTheta_x_vs_gonio_d0ycut_d0xcut = new TH2D("dTheta_x_vs_gonio_d0ycut_d0xcut" , " gonio vs dTheta up  d0ycut d0xcut" , 1400,-400.,1000.,100., 0., 100.);
  
  
  //TH2D * dTheta_x_vs_d0x_nocut = new TH2D("dTheta_x_vs_d0x_nocut" , " dTheta_x_vs_d0x_nocut" , 1400,-400.,1000.,100., 0., 100.);
  //TH2D * dTheta_x_vs_gonio_nocut = new TH2D("dTheta_x_vs_gonio_nocut" , " dTheta_x_vs_gonio_nocut" , 1400,-400.,1000.,2000., 4712000., 4714000. );
  //TH2D * dTheta_x_vs_gonio_nocut_zero = new TH2D("dTheta_x_vs_gonio_nocut_zero" , " dTheta_x_vs_gonio_nocut_zero" , 1400,-400.,1000.,500., 0., 100.);
  //TH2D * dTheta_x_vs_gonio_mycut_test = new TH2D("dTheta_x_vs_gonio_mycut_test" , " dTheta_x_vs_gonio_mycut_test" , 1400,-400.,1000.,100., 0., 100.);
  //TH2D * dTheta_x_vs_Impact_x_nocut = new TH2D("dTheta_x_vs_Impact_x_nocut" , " dTheta_x_vs_Impact_x_nocut " , 1400,-400.,1000.,500., 0., 100.);
  //TH2D * Impact_x_vs_d0y = new TH2D("Impact_x_vs_d0y", "Impact_x_vs_d0y",100,0.,100.,30,-3.,3.);
  
  
  TH1D * dTheta_x_nocut = new TH1D("dTheta_x_nocut","dTheta x no cut",200,-200.,200.);
  dTheta_x_nocut->GetXaxis()->SetTitle("#Delta#theta_{x} [#murad]");
  TH1D * dTheta_y_nocut = new TH1D("dTheta_y_nocut","dTheta y no cut",200,-200.,200.);
  dTheta_y_nocut->GetXaxis()->SetTitle("#Delta#theta_{y} [#murad]");
  //TH1D * dTheta_x_goniocut_d0ycut = new TH1D("dTheta_x_goniocut_d0ycut","dTheta x goniocut d0y cut",1400,-400.,1000.);
  //TH1D * dTheta_x_d0ycut = new TH1D("dTheta_x_d0ycut","dTheta x d0y cut",1400,-400.,1000.);
  //TH1D * dTheta_x_d0ycut_d0xcut = new TH1D("dTheta_x_d0ycut_d0xcut","dTheta x d0y cut d0x cut",1400,-400.,1000.);
  TH1D * ThetaIn_x = new TH1D("ThetaIn_x","ThetaIn_x",100,-100.,100.);
  ThetaIn_x->GetXaxis()->SetTitle("#theta_{x}^{in} [#murad]");
  TH1D * ThetaIn_y = new TH1D("ThetaIn_y","ThetaIn_y",100,-100.,100.);
  ThetaIn_y->GetXaxis()->SetTitle("#theta_{y}^{in} [#murad]");

  TH1D * ThetaOut_x = new TH1D("ThetaOut_x","ThetaOut_x",100,-100.,100.);
  ThetaOut_x->GetXaxis()->SetTitle("#theta_{x}^{out} [#murad]");
  TH1D * ThetaOut_y = new TH1D("ThetaOut_y","ThetaOut_y",100,-100.,100.);
  ThetaOut_y->GetXaxis()->SetTitle("#theta_{y}^{out} [#murad]");
  /*
  TH2D * ThetaIn_x_vs_s = new TH2D("ThetaIn_x_vs_s","ThetaIn_x vs time (s)",1000,76500,77500,200,-100.,100.); 
  ThetaIn_x_vs_s->GetXaxis()->SetTitle("time [s]");
  ThetaIn_x_vs_s->GetYaxis()->SetTitle("#theta_{in}(x) [#murad]");
  TH2D * ThetaIn_y_vs_s = new TH2D("ThetaIn_y_vs_s","ThetaIn_y vs time (s)",1000,76500,77500,200,-100.,100.); 
  ThetaIn_y_vs_s->GetXaxis()->SetTitle("time [s]");
  ThetaIn_y_vs_s->GetYaxis()->SetTitle("#theta_{in}(y) [#murad]");
  */

  int count_1track = 0;
  int h(0), m(0), s(0),d(0), inc_ang(0);
  char hh[2];
  char mm[2];
  char ss[2];
  char dd[2];
  //read and fill

  for (Int_t i = 0; i < nentries; i++) {
    if(i%50000==0) cout  << "event " << i << endl;

    Long64_t tentry = tree->LoadTree(i);
    tree->GetEntry(i);
		
    //h = atoi(time[0]);
    // m = atoi(time[2])*10 + atoi(time[3]);
    //s = atoi(time[5])*10 + atoi(time[6]);
    /*    h=0;
    m=0;
    s=0;
    d=0;

    hh[0] = time[0];
    hh[1] = time[1];
    mm[0] = time[2];
    mm[1] = time[3];
    ss[0] = time[4];
    ss[1] = time[5];

    h = atoi(hh);
    m = atoi(mm);
    s = atoi(ss);

    s += (h*3600)+(m*60);
 
    dd[0] = date[0];
    dd[1] = date[1]; 
    d = atoi(dd);
    */
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
      double init_angle = 4.712668e+06;    

      
      
      if(ntracks==1) {
	//dTheta_x_vs_Impact_x_nocut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0] )*1e6,gPos.x+(tracks.thetaIn_x[0]*1e6)-init_scan);
	
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
	
	dTheta_x_nocut->Fill((tracks.thetaIn_x[2] - tracks.thetaIn_x[0])*1e6);
	dTheta_y_nocut->Fill((tracks.thetaIn_x[3] - tracks.thetaIn_x[1])*1e6);

	//	ThetaIn_x_vs_s->Fill(s,tracks.thetaIn_x[0]*1e6);
	//	ThetaIn_y_vs_s->Fill(s,tracks.thetaIn_x[1]*1e6);

	d0x_h->Fill(tracks.thetaIn_x[8]);
	d0y_h->Fill(tracks.thetaIn_x[9]);

	d0x_vs_d0y_h->Fill(tracks.thetaIn_x[8],tracks.thetaIn_x[9]);

	ThetaIn_x->Fill(tracks.thetaIn_x[0]*1e6);
	ThetaIn_y->Fill(tracks.thetaIn_x[1]*1e6);

	

	if (tracks.thetaIn_x[0]*1e6 < ThetaInMax && tracks.thetaIn_x[0]*1e6 > ThetaInmin){
	  ThetaOut_x->Fill(tracks.thetaIn_x[2]*1e6);
	}

	if (tracks.thetaIn_x[1]*1e6 < ThetaInMax && tracks.thetaIn_x[1]*1e6 > ThetaInmin){
	  ThetaOut_y->Fill(tracks.thetaIn_x[3]*1e6);
	}


	//dTheta_x_vs_d0x_nocut->Fill((tracks.thetaOut_x[0] - tracks.thetaIn_x[0])*1e6,tracks.d0_x[0]);
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
  } //close cycle on nentries

  cout<<"events with 1 track "<< count_1track << endl;




  
 


  //analisi picco

  /*
  TCanvas *c30 = new TCanvas();
  ThetaIn_x_vs_s->Draw("colz");

  TCanvas *c31 = new TCanvas();
  ThetaIn_y_vs_s->Draw("colz");
  

  int ymax1 = ThetaIn_x_vs_s->GetYaxis()->GetLast(); 
  int ymin1 = ThetaIn_x_vs_s->GetYaxis()->GetFirst();   
  Int_t yn1 = ThetaIn_x_vs_s->GetNbinsY(); 

  cout << ymax1 << " " << ymin1 << " " << yn1 <<  endl;
  
  int xmax1 = ThetaIn_x_vs_s->GetXaxis()->GetLast(); 
  int xmin1 = ThetaIn_x_vs_s->GetXaxis()->GetFirst();   
  Int_t xn1 = ThetaIn_x_vs_s->GetNbinsX(); 

  cout << xmax1 << " " << xmin1 << " " << xn1 <<  endl;

  TMatrixD tor1(xn1+1,yn1+1); 
  TMatrixD r_x1(xn1+1,yn1+1); 
  TVectorD r_y1(xn1+1);

  for (int i = 0; i < xn1; i++) {  
    for (int j = 0; j < yn1; j++) {
      tor1[i][j] = ThetaIn_x_vs_s->GetBinContent(i,j);
      r_x1[i][j] = ThetaIn_x_vs_s->GetYaxis()->GetBinCenter(j);  
    }
    r_y1[i]= ThetaIn_x_vs_s->GetXaxis()->GetBinCenter(i);

 }




  Double_t mean1[1500];
  Double_t somma1[1500];
  Double_t prod1[1500];
  Double_t tempo1[1500];
  Double_t media1[1500];
  Double_t err_y1[1500];
  Double_t err_x1[1500];
  Int_t counter1=0;

 for (int i = 0; i < xn1; i++) {
    for (int j = 0; j < yn1; j++) {
      somma1[i]=0;
      prod1[i]=0;
      mean1[i]=0;
      tempo1[i]=0;
      media1[i]=0;
      err_x1[i]=0;
      err_y1[i]=0;
    }
  }

  for (int i = 0; i < xn1; i++) {
    for (int j = 0; j < yn1; j++) {
      somma1[i] += tor1[i][j];
      prod1[i] += r_x1[i][j]*tor1[i][j];
    
    }
    if (somma1[i] != 0. ) {
      mean1[i]=prod1[i]/somma1[i];
      
      //  counter++;
      // tempo[i]=counter;
      // cout << counter << endl;
      //cout << tempo[i] << endl;
    }
    if (mean1[i] != 0. ) {
      counter1++;
      tempo1[counter1]=counter1;
      media1[counter1]=mean1[i];

    err_x1[counter1]=0;
    err_y1[counter1]=TMath::RMS(counter1,media1);
    }
  }
 
  TVectorD mean1(xn1+1);
  TVectorD somma1(xn1+1);
  TVectorD prod1(xn1+1);

  for (int i = 0; i < xn1; i++) {
    for (int j = 0; j < yn1; j++) {
      somma1[i] += tor1[i][j];
      prod1[i] += r_x1[i][j]*tor1[i][j];
    
    }
    if (somma1[i] != 0. ) 
    mean1[i]=prod1[i]/somma1[i];
  
  }
 
  
  TCanvas * cm1 = new TCanvas();
 
  TGraphErrors * torsion1 = new TGraphErrors(counter1,tempo1,media1,err_x1,err_y1);
  torsion1->Draw("ap"); 
  torsion1->SetMarkerColor(4);
  torsion1->SetMarkerSize(1);
  torsion1->SetMarkerStyle(22);
  torsion1->SetTitle("Mean incoming angle x vs time");
  torsion1->GetYaxis()->SetTitle("Mean #theta_{in}(x) [#murad]");
  torsion1->GetXaxis()->SetTitle("time [s]");





  int ymax2 = ThetaIn_y_vs_s->GetYaxis()->GetLast(); 
  int ymin2 = ThetaIn_y_vs_s->GetYaxis()->GetFirst();   
  Int_t yn2 = ThetaIn_y_vs_s->GetNbinsY(); 

  cout << ymax2 << " " << ymin2 << " " << yn2 <<  endl;
  
  int xmax2 = ThetaIn_y_vs_s->GetXaxis()->GetLast(); 
  int xmin2 = ThetaIn_y_vs_s->GetXaxis()->GetFirst();   
  Int_t xn2 = ThetaIn_y_vs_s->GetNbinsX(); 

  cout << xmax2 << " " << xmin2 << " " << xn2 <<  endl;

  TMatrixD tor2(xn2+1,yn2+1); 
  TMatrixD r_x2(xn2+1,yn2+1); 
  TVectorD r_y2(xn2+1);

  for (int i = 0; i < xn2; i++) {  
    for (int j = 0; j < yn2; j++) {
      tor2[i][j] = ThetaIn_y_vs_s->GetBinContent(i,j);
      r_x2[i][j] = ThetaIn_y_vs_s->GetYaxis()->GetBinCenter(j);  
    }
    r_y2[i]= ThetaIn_x_vs_s->GetXaxis()->GetBinCenter(i);

 }





  Double_t mean2[1500];
  Double_t somma2[1500];
  Double_t prod2[1500];
  Double_t tempo2[1500];
  Double_t media2[1500];
  Double_t err_y2[1500];
  Double_t err_x2[1500];
  Int_t counter2=0;


  for (int i = 0; i < xn2; i++) {
    for (int j = 0; j < yn2; j++) {
      somma2[i]=0;
      prod2[i]=0;
      mean2[i]=0;
      tempo2[i]=0;
      media2[i]=0;
      err_x2[i]=0;
      err_y2[i]=0;
    }
  }

  for (int i = 0; i < xn2; i++) {
    for (int j = 0; j < yn2; j++) {
      somma2[i] += tor2[i][j];
      prod2[i] += r_x2[i][j]*tor2[i][j];
    
    }
    if (somma2[i] != 0. ) {
      mean2[i]=prod2[i]/somma2[i];
      
      //  counter++;
      // tempo[i]=counter;
      // cout << counter << endl;
      //cout << tempo[i] << endl;
    }
    if (mean2[i] != 0. ) {
      counter2++;
      tempo2[counter2]=counter2;
      media2[counter2]=mean2[i];

    err_x2[counter2]=0;
    err_y2[counter2]=TMath::RMS(counter2,media2);
    }
  }

 TVectorD mean2(xn2+1);
  TVectorD somma2(xn2+1);
  TVectorD prod2(xn2+1);

  for (int i = 0; i < xn2; i++) {
    for (int j = 0; j < yn2; j++) {
      somma2[i] += tor2[i][j];
      prod2[i] += r_x2[i][j]*tor2[i][j];
    
    }
    if (somma2[i] != 0. ) 
    mean2[i]=prod2[i]/somma2[i];
  
  }
  
 TCanvas * cm2 = new TCanvas();
 
 TGraphErrors * torsion2 = new TGraphErrors(counter2,tempo2,media2,err_x2,err_y2);
  torsion2->Draw("ap"); 
  torsion2->SetMarkerColor(4);
  torsion2->SetMarkerSize(1);
  torsion2->SetMarkerStyle(22);
  torsion2->SetTitle("Mean incoming angle y vs time");
  torsion2->GetYaxis()->SetTitle("Mean #theta_{in}(y) [#murad]");
  torsion2->GetXaxis()->SetTitle("time [s]");

*/


  TCanvas *c1 = new TCanvas("c1","Telescope Resolution",1100,1100);
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
  c2->SetTitle("beam profile");
  gStyle->SetOptFit(1);
    
  c2->Divide(2,1);
  c2->cd(1);
  

  Double_t parx[3]={0};
  TF1 *gx    = new TF1("gx","gaus",-2,2);
 

  gx->SetLineColor(kRed);
  
  d0x_h->Fit(gx,"R");
 
  gx->GetParameters(&parx[0]);
  d0x_h->Draw();

  cout << "fit parameter" << endl;
  cout <<"constant " << parx[0] << endl;
  cout <<"mean " << parx[1] << endl;
  cout <<"sigma " << parx[2] << endl;


  c2->cd(2);

  Double_t pary[3]={0};
  TF1 *gy    = new TF1("gy","gaus",-2,2);
 

  gy->SetLineColor(kRed);
  
  d0y_h->Fit(gy,"R");
 
  gy->GetParameters(&pary[0]);
  d0y_h->Draw();

  cout << "fit parameter" << endl;
  cout <<"constant " << pary[0] << endl;
  cout <<"mean " << pary[1] << endl;
  cout <<"sigma " << pary[2] << endl;

  TCanvas *c3 = new TCanvas("c3","Beam Spot",1100,1100);
  c3->SetTitle("Beam Spot");

  d0x_vs_d0y_h->Draw("contz");
  
  TCanvas *c4 = new TCanvas();
  c4->SetTitle("beam profile");
 
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

 
  TCanvas *c5 = new TCanvas();
  c5->SetTitle("beam profile");
 
  c5->Divide(2,1);
  c5->cd(1);


  Double_t parthx1[3]={0};
  TF1 *gthx1    = new TF1("gthx1","gaus",-100,100);
 

  gthx1->SetLineColor(kRed);
  
  ThetaOut_x->Fit(gthx1,"R");
 
  gthx1->GetParameters(&parthx1[0]);
  ThetaOut_x->Draw();

  cout << "fit parameter" << endl;
  cout <<"constant " << parthx1[0] << endl;
  cout <<"mean " << parthx1[1] << endl;
  cout <<"sigma " << parthx1[2] << endl;

  
  c5->cd(2);

  Double_t parthy1[3]={0};
  TF1 *gthy1    = new TF1("gthy1","gaus",-100,100);
 

  gthy1->SetLineColor(kRed);
  
  ThetaOut_y->Fit(gthy1,"R");
 
  gthy1->GetParameters(&parthy1[0]);
  ThetaOut_y->Draw();

  cout << "fit parameter" << endl;
  cout <<"constant " << parthy1[0] << endl;
  cout <<"mean " << parthy1[1] << endl;
  cout <<"sigma " << parthy1[2] << endl;



  TFile *outfile = new TFile(outputfile,"RECREATE");
  dTheta_x_nocut->Write();
  dTheta_y_nocut->Write();
  d0x_h->Write();
  d0y_h->Write();
  d0x_vs_d0y_h->Write();
  ThetaIn_x->Write();
  ThetaIn_y->Write();
  c1->Write("resolution");
  c2->Write("divergence");
  c3->Write("spot");
  c4->Write("profile");
  // ThetaIn_x_vs_s->Write();
  // ThetaIn_y_vs_s->Write();
  //torsion1->Write("menaImpact_x_vs_time");
  //torsion2->Write("menaImpact_y_vs_time");


  myfile.close();  


  cout<<"Processing finished!"<<endl;
    

}  //close macro

