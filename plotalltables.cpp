//this will be simple root function

#include "Riostream.h"
#include <vector>
#include <pair>

void plotalltable(){

//load txt file
//the file has 8 data entries
//1)pileup
//2)first timestamp
//3)last timestamp
//4)nEvents
//5)nTracksAll
//6)nTracksGood
//6)
//7)
//8)

ifstream in;

 double pileup; 
 double timestampFirst;
 double timestampLast;
 double nEvents;
 double nTotalTracks;
 double nGoodTracks;

 double accidentalRate;
 double accidentalRateError;

 int nlines = 0;

 // TString filenames[4] = {filename0,filename1,filename2,filename3}
 // TString masks[4] = {mask0,mask1,mask2,mask3}


 std::vector< std::pair<int, int> > masks;
 std::vector< std::pair<int, int> > fringes;

 masks.push_back(std::make_pair(26,39));
 masks.push_back(std::make_pair(24,36));
 masks.push_back(std::make_pair(22,33));
 masks.push_back(std::make_pair(20,30));



 fringes.push_back(std::make_pair(2,3));
 fringes.push_back(std::make_pair(1,1));


 std::vector< TGraphErrors* > g;
 TMultiGraph * mg = new TMultiGraph();

 std::vector< TGraphErrors* > goodTracks;
 TMultiGraph * goodTracksMulti = new TMultiGraph();

 for(int ii = 0; ii < (masks.size() * fringes.size()); ++ii)
  {

    TGraphErrors * temphist = new TGraphErrors();
    g.push_back(temphist);

    TGraphErrors * temphist2 = new TGraphErrors();
    goodTracks.push_back(temphist2);
  }


 // TGraphErrors * acc_vs_pu = new TGraphErrors();



 double trackNormValue = 0.;

 for(int j = 0; j < fringes.size(); ++j)
   {
     for(int i = 0; i < masks.size(); ++i)
       {
	 
	 //TString filename = filenames[i];
	 
	 char filename[256];
	 sprintf(filename,"AccRateSim_%i_%i_fringe%i_%i.txt",masks[i].first,masks[i].second,fringes[j].first,fringes[j].second);
	 cout << filename << endl;
	 in.open(filename);
	 
	 while(true)
	   {
	     
	     in >> pileup >> timestampFirst >> timestampLast >> nEvents >> nTotalTracks >> nGoodTracks;
	     if(!in.good()) break;
	     
	     accidentalRate = (nTotalTracks - nGoodTracks)/nGoodTracks; 
	     accidentalRateError = TMath::Sqrt(accidentalRate * (1-accidentalRate)/nTotalTracks);
	     ++nlines;
	     
	     g[i+j*masks.size()]->SetPoint(g[i+j*masks.size()]->GetN(),pileup,accidentalRate);
	     g[i+j*masks.size()]->SetPointError(g[i+j*masks.size()]->GetN()-1,0,accidentalRateError);

 
	     if(masks[i].first == 26&&masks[i].second == 39&&fringes[j].first == 2 &&fringes[j].second == 3 && pileup == 1)
	       {
		 trackNormValue = nGoodTracks;
		 cout << "TrackNormValue= " << nGoodTracks << endl;
	       }

	     goodTracks[i+j*masks.size()]->SetPoint(goodTracks[i+j*masks.size()]->GetN(),pileup, 100 * (trackNormValue - nGoodTracks)/trackNormValue / 2);

	     
	   }
	 in.close();
       }
   }

double marksize = 2.5;
//style
g[0]->SetLineColor(1);
g[0]->SetMarkerColor(1);
g[0]->SetMarkerStyle(33);
g[0]->SetMarkerSize(marksize);
//
g[1]->SetLineColor(1);
g[1]->SetMarkerColor(2);
g[1]->SetMarkerStyle(33);
g[1]->SetMarkerSize(marksize);
//
g[2]->SetLineColor(1);
g[2]->SetMarkerColor(8);
g[2]->SetMarkerStyle(33);
g[2]->SetMarkerSize(marksize);
//
g[3]->SetLineColor(1);
g[3]->SetMarkerColor(4);
g[3]->SetMarkerStyle(33);
g[3]->SetMarkerSize(marksize);
//
g[4]->SetLineColor(1);
g[4]->SetMarkerColor(1);
g[4]->SetMarkerStyle(20);
g[4]->SetMarkerSize(marksize/2);
//
g[5]->SetLineColor(1);
g[5]->SetMarkerColor(2);
g[5]->SetMarkerStyle(20);
g[5]->SetMarkerSize(marksize/2);
//
g[6]->SetLineColor(1);
g[6]->SetMarkerColor(8);
g[6]->SetMarkerStyle(20);
g[6]->SetMarkerSize(marksize/2);
//
g[7]->SetLineColor(1);
g[7]->SetMarkerColor(4);
g[7]->SetMarkerStyle(20);
g[7]->SetMarkerSize(marksize/2);

//style good track plot

goodTracks[0]->SetLineColor(1);
goodTracks[0]->SetMarkerColor(1);
goodTracks[0]->SetMarkerStyle(33);
goodTracks[0]->SetMarkerSize(marksize);
//
goodTracks[1]->SetLineColor(1);
goodTracks[1]->SetMarkerColor(2);
goodTracks[1]->SetMarkerStyle(33);
goodTracks[1]->SetMarkerSize(marksize);
//
goodTracks[2]->SetLineColor(1);
goodTracks[2]->SetMarkerColor(8);
goodTracks[2]->SetMarkerStyle(33);
goodTracks[2]->SetMarkerSize(marksize);
//
goodTracks[3]->SetLineColor(1);
goodTracks[3]->SetMarkerColor(4);
goodTracks[3]->SetMarkerStyle(33);
goodTracks[3]->SetMarkerSize(marksize);
//
goodTracks[4]->SetLineColor(1);
goodTracks[4]->SetMarkerColor(1);
goodTracks[4]->SetMarkerStyle(20);
goodTracks[4]->SetMarkerSize(marksize/2);
//
goodTracks[5]->SetLineColor(1);
goodTracks[5]->SetMarkerColor(2);
goodTracks[5]->SetMarkerStyle(20);
goodTracks[5]->SetMarkerSize(marksize/2);
//
goodTracks[6]->SetLineColor(1);
goodTracks[6]->SetMarkerColor(8);
goodTracks[6]->SetMarkerStyle(20);
goodTracks[6]->SetMarkerSize(marksize/2);
//
goodTracks[7]->SetLineColor(1);
goodTracks[7]->SetMarkerColor(4);
goodTracks[7]->SetMarkerStyle(20);
goodTracks[7]->SetMarkerSize(marksize/2);

 mg->SetTitle(" Accidental rates vs pileup;Pileup;Accidental rate");
 goodTracksMulti->SetTitle(" Loss in statistical presicion vs pileup;Pileup;Loss in presicion [%]");

 for(int ii = 0; ii < g.size(); ++ii)
   {
     mg->Add(g[ii]);
     goodTracksMulti->Add(goodTracks[ii]);
   }

 TCanvas c("c1","c1",2000,1200);
 c.cd();
 c.SetGrid();
//style
 mg->Draw("AP");
//axis
 mg->GetXaxis()->SetLimits(0.0,45.);
 mg->SetMaximum(0.23);
 mg->SetMinimum(0.1);

//legend
  char legendtitle[256];
  sprintf(legendtitle," Masks and fringes");
  TLegend * leg = new TLegend(0.1,0.7,0.40,0.9,legendtitle);
  char legendentry[256];

 for(int j = 0; j < fringes.size(); ++j)
   {
     for(int i = 0; i < masks.size(); ++i)
       {

	 sprintf(legendentry,"Mask:%i/%i Fringe:%i/%i",masks[i].first,masks[i].second,fringes[j].first,fringes[j].second);
	 leg->AddEntry(g[i+j*masks.size()],legendentry,"P");
       }
   }

  leg->Draw();


char outfilename[256];
sprintf(outfilename,"AccRateSimTotal.png");
c.SaveAs(outfilename);


 TCanvas c2("c2","c2",2000,1200);
 c2.cd();
 c2.SetGrid();
//style
 goodTracksMulti->Draw("AP");
//axis
 goodTracksMulti->GetXaxis()->SetLimits(0.0,45.);
 goodTracksMulti->SetMaximum(35.);
 goodTracksMulti->SetMinimum(0.0);
 leg->Draw();

 sprintf(outfilename,"GoodTracksTotal.png");
 c2.SaveAs(outfilename);



}
