//this will be simple root function

#include "Riostream.h"

void plottable(TString filename, TString mask, TString fringe){

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
in.open(filename);

 double pileup; 
 double timestampFirst;
 double timestampLast;
 double nEvents;
 double nTotalTracks;
 double nGoodTracks;

 double accidentalRate;
 double accidentalRateError;

 int nlines = 0;

// TGraph * g[7]; 
// TMultiGraph * mg = new TMultiGraph();


// for(int ii = 0; ii < 7; ++ii)
//   {

// g[ii] = new TGraph();

// }


 TGraphErrors * acc_vs_pu = new TGraphErrors();


while(true)
  {
    
    in >> pileup >> timestampFirst >> timestampLast >> nEvents >> nTotalTracks >> nGoodTracks;
    if(!in.good()) break;

    accidentalRate = (nTotalTracks - nGoodTracks)/nGoodTracks; 
    accidentalRateError = 100.0 * TMath::Sqrt(accidentalRate * (1-accidentalRate)/nTotalTracks);
    std::cout << accidentalRateError << std::endl;
    ++nlines;

    acc_vs_pu->SetPoint(acc_vs_pu->GetN(),pileup,accidentalRate);
    acc_vs_pu->SetPointError(acc_vs_pu->GetN(),0,accidentalRateError);

}
double marksize = 2.5;
//style
acc_vs_pu->SetLineColor(1);
acc_vs_pu->SetMarkerColor(1);
acc_vs_pu->SetMarkerStyle(33);
acc_vs_pu->SetMarkerSize(marksize);
//

acc_vs_pu->SetTitle(" Accidental rates vs pileup;Pileup;Accidental rate");



TCanvas c("c1","c1",2000,1200);
c.cd();
c.SetGrid();
//style
acc_vs_pu->Draw("AP");
//axis
//acc_vs_pu->GetYaxis()->SetLimits(0.0,0.4);
 acc_vs_pu->SetMaximum(0.4);
 acc_vs_pu->SetMinimum(0.0);

//legend
 char legendtitle[256];
 sprintf(legendtitle," Masks");
 TLegend * leg = new TLegend(0.1,0.7,0.40,0.9,legendtitle);
 char legendentry[256];
 sprintf(legendentry,"Mask:%s Fringe:%s",mask.Data(),fringe.Data());
 leg->AddEntry(acc_vs_pu,legendentry,"P");
 leg->Draw();


char outfilename[256];
sprintf(outfilename,"AccRateSim.png");
c.SaveAs(outfilename);


}
