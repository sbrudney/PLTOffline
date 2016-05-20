//this will be simple root function

#include "Riostream.h"
#include <vector>

void plotalltableTFC(){

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
 double nEvents;
 double TFC;
 double nEventsWith;
 double nEventsWithout;

 double withRate;
 double withoutRate;

 int nlines = 0;


std::vector< std::pair<int, int> > masks;
 std::vector< std::pair<int, int> > fringes;

 masks.push_back(std::make_pair(26,39));
 masks.push_back(std::make_pair(24,36));
 masks.push_back(std::make_pair(22,33));
 masks.push_back(std::make_pair(20,30));

 fringes.push_back(std::make_pair(2,3));
 fringes.push_back(std::make_pair(1,1));



 std::vector< TGraph* > with; 
 std::vector< TGraph* > without; 
 std::vector< TGraph* > logs; 
 std::vector< TGraph* > fits;
TMultiGraph * mg = new TMultiGraph();
TMultiGraph * logsMulti = new TMultiGraph();

for(int ii = 0; ii < (masks.size() * fringes.size()); ++ii)
  {

    TGraph * temphist = new TGraph();
    with.push_back(temphist);

    TGraph * temphist2 = new TGraph();
    without.push_back(temphist2);

    TGraph * temphist3 = new TGraph();
    logs.push_back(temphist3);

    TF1 * funk = new TF1();
    fits.push_back(funk);


  }


 // TGraphErrors * acc_vs_pu = new TGraphErrors();


 for(int j = 0; j < fringes.size(); ++j)
   {
     for(int i = 0; i < masks.size(); ++i)
       {
	 
	 char filename[256];
	 sprintf(filename,"SimPuData/TFCbyPU_%i_%i_fringe%i_%i.txt",masks[i].first,masks[i].second,fringes[j].first,fringes[j].second);
	 cout << filename << endl;
	 in.open(filename);
	 
	 while(true)
	   {
	     
	     in >> pileup >> nEvents >> TFC >> nEventsWith >> nEventsWithout;
	     if(!in.good()) break;
	     
	     withRate = nEventsWith/nEvents;
	     withoutRate = nEventsWithout/nEvents;
	     
	     ++nlines;
	     
	     with[i+j*masks.size()]->SetPoint(with[i+j*masks.size()]->GetN(),pileup,withRate);
	     without[i+j*masks.size()]->SetPoint(with[i+j*masks.size()]->GetN(),pileup,withoutRate);
	     logs[i+j*masks.size()]->SetPoint(logs[i+j*masks.size()]->GetN(),pileup, -1 *  TMath::Log(withoutRate));
	     
	   }
	 in.close();
       }
   }

double marksize = 2.5;
//style
with[0]->SetLineColor(1);
with[0]->SetMarkerColor(1);
with[0]->SetMarkerStyle(33);
with[0]->SetMarkerSize(marksize);
//
with[1]->SetLineColor(1);
with[1]->SetMarkerColor(2);
with[1]->SetMarkerStyle(33);
with[1]->SetMarkerSize(marksize);
//
with[2]->SetLineColor(1);
with[2]->SetMarkerColor(8);
with[2]->SetMarkerStyle(33);
with[2]->SetMarkerSize(marksize);
//
with[3]->SetLineColor(1);
with[3]->SetMarkerColor(4);
with[3]->SetMarkerStyle(33);
with[3]->SetMarkerSize(marksize);
//
with[4]->SetLineColor(1);
with[4]->SetMarkerColor(5);
with[4]->SetMarkerStyle(33);
with[4]->SetMarkerSize(marksize);
//
with[5]->SetLineColor(1);
with[5]->SetMarkerColor(6);
with[5]->SetMarkerStyle(33);
with[5]->SetMarkerSize(marksize);
//
with[6]->SetLineColor(1);
with[6]->SetMarkerColor(40);
with[6]->SetMarkerStyle(33);
with[6]->SetMarkerSize(marksize);
//
with[7]->SetLineColor(1);
with[7]->SetMarkerColor(7);
with[7]->SetMarkerStyle(33);
with[7]->SetMarkerSize(marksize);
//


without[0]->SetLineColor(1);
without[0]->SetMarkerColor(1);
without[0]->SetMarkerStyle(23);
without[0]->SetMarkerSize(marksize);
//
without[1]->SetLineColor(1);
without[1]->SetMarkerColor(2);
without[1]->SetMarkerStyle(23);
without[1]->SetMarkerSize(marksize);
//
without[2]->SetLineColor(1);
without[2]->SetMarkerColor(8);
without[2]->SetMarkerStyle(23);
without[2]->SetMarkerSize(marksize);
//
without[3]->SetLineColor(1);
without[3]->SetMarkerColor(4);
without[3]->SetMarkerStyle(23);
without[3]->SetMarkerSize(marksize);
//
without[4]->SetLineColor(1);
without[4]->SetMarkerColor(5);
without[4]->SetMarkerStyle(23);
without[4]->SetMarkerSize(marksize);
//
without[5]->SetLineColor(1);
without[5]->SetMarkerColor(6);
without[5]->SetMarkerStyle(23);
without[5]->SetMarkerSize(marksize);
//
without[6]->SetLineColor(1);
without[6]->SetMarkerColor(40);
without[6]->SetMarkerStyle(23);
without[6]->SetMarkerSize(marksize);
//
without[7]->SetLineColor(1);
without[7]->SetMarkerColor(7);
without[7]->SetMarkerStyle(23);
without[7]->SetMarkerSize(marksize);

//logs
//
logs[0]->SetLineColor(1);
logs[0]->SetMarkerColor(1);
logs[0]->SetMarkerStyle(23);
logs[0]->SetMarkerSize(marksize);
//
logs[1]->SetLineColor(1);
logs[1]->SetMarkerColor(2);
logs[1]->SetMarkerStyle(23);
logs[1]->SetMarkerSize(marksize);
//
logs[2]->SetLineColor(1);
logs[2]->SetMarkerColor(8);
logs[2]->SetMarkerStyle(23);
logs[2]->SetMarkerSize(marksize);
//
logs[3]->SetLineColor(1);
logs[3]->SetMarkerColor(4);
logs[3]->SetMarkerStyle(23);
logs[3]->SetMarkerSize(marksize);
//
logs[4]->SetLineColor(1);
logs[4]->SetMarkerColor(5);
logs[4]->SetMarkerStyle(23);
logs[4]->SetMarkerSize(marksize);
//
logs[5]->SetLineColor(1);
logs[5]->SetMarkerColor(6);
logs[5]->SetMarkerStyle(23);
logs[5]->SetMarkerSize(marksize);
//
logs[6]->SetLineColor(1);
logs[6]->SetMarkerColor(40);
logs[6]->SetMarkerStyle(23);
logs[6]->SetMarkerSize(marksize);
//
logs[7]->SetLineColor(1);
logs[7]->SetMarkerColor(7);
logs[7]->SetMarkerStyle(23);
logs[7]->SetMarkerSize(marksize);
//

 mg->SetTitle(" Rates of Events with TFC and without TFC vs pileup;Pileup");

 for(int ii = 0; ii < with.size(); ++ii)
   {
     mg->Add(with[ii]);
     mg->Add(without[ii]);
   }

 TCanvas c("c1","c1",2000,1200);
 c.cd();
 c.SetGrid();
 //style
 mg->Draw("AP");
 //axis
 mg->GetXaxis()->SetLimits(0.0,45.);
 mg->SetMaximum(1.0);
 mg->SetMinimum(0.0);

 //legend
 char legendtitle[256];
 sprintf(legendtitle,"Masks and Fringes");
 TLegend * leg = new TLegend(0.6,0.4,0.90,0.6,legendtitle);
 leg-> SetNColumns(2);

 char legendentry[256];

 for(int j = 0; j < fringes.size(); ++j)
   {
     for(int i = 0; i < masks.size(); ++i)
       {

	 sprintf(legendentry,"with TFC Mask:%i/%i Fringe:%i/%i",masks[i].first,masks[i].second,fringes[j].first,fringes[j].second);
	 leg->AddEntry(with[i+j*masks.size()],legendentry,"P");
	 sprintf(legendentry,"without TFC Mask:%i/%i Fringe:%i/%i",masks[i].first,masks[i].second,fringes[j].first,fringes[j].second);
	 leg->AddEntry(without[i+j*masks.size()],legendentry,"P");
       }
   }

 leg->Draw();


 char outfilename[256];
 sprintf(outfilename,"TFCTotal.png");
 c.SaveAs(outfilename);


 logsMulti->SetTitle(" -log() of without TFC rate vs pileup;Pileup");

 for(int ii = 0; ii < logs.size(); ++ii)
   {
     logsMulti->Add(logs[ii]);
   }

 TCanvas c2("c2","c2",2000,1200);
 c2.cd();
 c2.SetGrid();
 //style
 logsMulti->Draw("AP");
 //axis
 logsMulti->GetXaxis()->SetLimits(0.0,45.);
 // logsMulti->SetMaximum(1.0);
 // logsMulti->SetMinimum(0.0);

 //legend
 // char legendtitle[256];
 sprintf(legendtitle,"Masks and Fringes");
 TLegend * leg2 = new TLegend(0.1,0.6,0.5,0.9,legendtitle);
 // leg-> SetNColumns(2);

 char legendentry[256];

 for(int j = 0; j < fringes.size(); ++j)
   {
     for(int i = 0; i < masks.size(); ++i)
       {

	 sprintf(legendentry,"with TFC Mask:%i/%i Fringe:%i/%i",masks[i].first,masks[i].second,fringes[j].first,fringes[j].second);
	 leg2->AddEntry(logs[i+j*masks.size()],legendentry,"P");
	 logs[i+j*masks.size()]->Fit("pol1","","",0,20);

       }
   }

 leg2->Draw();

 sprintf(outfilename,"NegLogTFCTotal.png");
 c2.SaveAs(outfilename);





 

}
