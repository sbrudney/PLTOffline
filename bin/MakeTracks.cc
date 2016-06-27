////////////////////////////////////////////////////////////////////
//
// Dean Andrew Hidas <Dean.Andrew.Hidas@cern.ch>
//
// Created on: Thu Mar  8 18:26:18 UTC 2012
//
////////////////////////////////////////////////////////////////////


#include <iostream>
#include <string>

#include "PLTEvent.h"
#include "PLTU.h"
#include "TFile.h"




// FUNCTION DEFINITIONS HERE
int MakeTracks (std::string const, std::string const, std::string const, bool, std::string const);




int MakeTracks (std::string const DataFileName, std::string const GainCalFileName, std::string const AlignmentFileName, bool IsText, std::string const MaskFileName)
{
  std::cout << "DataFileName:      " << DataFileName << std::endl;
  std::cout << "GainCalFileName:   " << GainCalFileName << std::endl;
  std::cout << "AlignmentFileName: " << AlignmentFileName << std::endl;
  std::cout << "MaskFileName:      " << MaskFileName << std::endl;

  // Set some basic style for plots
  PLTU::SetStyle();
  gStyle->SetOptStat(1111);

  // Grab the plt event reader
  //  PLTEvent Event(DataFileName, GainCalFileName, AlignmentFileName, IsText);
  //PLTEvent Event;
  //  if (MaskFileName != "blank") {
//     PLTEvent Event(DataFileName, GainCalFileName, AlignmentFileName, MaskFileName, IsText);
//     PLTPlane::FiducialRegion FidRegionHits  = PLTPlane::kFiducialRegion_All;
//     PLTPlane::FiducialRegion FidRegionTrack = PLTPlane::kFiducialRegion_All;
//     Event.SetPlaneFiducialRegion(FidRegionHits);
//     Event.SetPlaneClustering(PLTPlane::kClustering_AllTouching, FidRegionHits);
//   }
//   else {
  //PLTMask Mask(MaskFileName);
  PLTEvent Event(DataFileName, GainCalFileName, AlignmentFileName, MaskFileName, IsText);
    
    
  PLTPlane::FiducialRegion FidRegionHits  = PLTPlane::kFiducialRegion_All;
  PLTPlane::FiducialRegion FidRegionTrack = PLTPlane::kFiducialRegion_All;
  Event.SetPlaneFiducialRegion(FidRegionHits);
  Event.SetPlaneClustering(PLTPlane::kClustering_AllTouching, FidRegionHits);
//   }
  PLTAlignment Alignment;
  Alignment.ReadAlignmentFile(AlignmentFileName);

  //mask here
//   if (MaskFileName != "blank") {
//     PLTMask Mask;
//     Mask.ReadMaskFile(MaskFileName);
//   }

  std::map<int, int> NTrkEvMap;

  TH2F* HistBeamSpot[4];
  HistBeamSpot[0] = new TH2F("BeamSpotX", "BeamSpot X=0;Y;Z;NTracks", 25, -25, 25, 25, -540, 540);
  HistBeamSpot[1] = new TH2F("BeamSpotY", "BeamSpot Y=0;X;Z;NTracks", 25, -25, 25, 25, -540, 540);
  HistBeamSpot[2] = new TH2F("BeamSpotZ", "BeamSpot Z=0;X;Y;NTracks", 30, -10, 10, 15, -10, 10);
  HistBeamSpot[3] = new TH2F("BeamSpotZzoom", "BeamSpotZoom Z=0;X;Y;NTracks", 30, -5, 5, 30, -5, 5);

  std::map<int, TH1F*> MapSlopeY;
  std::map<int, TH1F*> MapSlopeX;
  std::map<int, TH2F*> MapSlope2D;
  std::map<int, TH1F*> MapResidualY;
  std::map<int, TH1F*> MapResidualX;

  // Loop over all events in file
  for (int ientry = 0; Event.GetNextEvent() >= 0; ++ientry) {
    if (ientry % 10000 == 0) {
//      std::cout << "Processing entry: " << ientry << std::endl;
    }
    if (ientry>=20000){break;}


    // Loop over all planes with hits in event
    for (size_t it = 0; it != Event.NTelescopes(); ++it) {
      
      // THIS telescope is
      PLTTelescope* Telescope = Event.Telescope(it);

      if (!MapSlopeY[Telescope->Channel()]) {
        TString Name = TString::Format("SlopeY_Ch%i", Telescope->Channel());
        MapSlopeY[Telescope->Channel()] = new TH1F(Name, Name, 40, -0.02, 0.06);
        MapSlopeY[Telescope->Channel()]->SetXTitle("Local Telescope Track-SlopeY #DeltaY/#DeltaZ");
        Name = TString::Format("SlopeX_Ch%i", Telescope->Channel());
        MapSlopeX[Telescope->Channel()] = new TH1F(Name, Name, 40, -0.04, 0.04);
        MapSlopeX[Telescope->Channel()]->SetXTitle("Local Telescope Track-SlopeX #DeltaX/#DeltaZ");
        Name = TString::Format("Slope2D_Ch%i", Telescope->Channel());
        MapSlope2D[Telescope->Channel()] = new TH2F(Name, Name, 100, -0.1, 0.1, 100, -0.1, 0.1);
        MapSlope2D[Telescope->Channel()]->SetXTitle("Local Telescope Track-SlopeX #DeltaX/#DeltaZ");
        MapSlope2D[Telescope->Channel()]->SetYTitle("Local Telescope Track-SlopeY #DeltaY/#DeltaZ");
        Name = TString::Format("ResidualY%i_ROC0", Telescope->Channel());
        MapResidualY[Telescope->Channel()*10+0] = new TH1F(Name, Name, 40, -0.02, 0.06);
        MapResidualY[Telescope->Channel()*10+0]->SetXTitle("Local Telescope Residual-Y (cm)");
        Name = TString::Format("ResidualX%i_ROC0", Telescope->Channel());
        MapResidualX[Telescope->Channel()*10+0] = new TH1F(Name, Name, 40, -0.04, 0.04);
        MapResidualX[Telescope->Channel()*10+0]->SetXTitle("Local Telescope Residual-X (cm)");
        Name = TString::Format("ResidualY%i_ROC1", Telescope->Channel());
        MapResidualY[Telescope->Channel()*10+1] = new TH1F(Name, Name, 40, -0.02, 0.06);
        MapResidualY[Telescope->Channel()*10+1]->SetXTitle("Local Telescope Residual-Y (cm)");
        Name = TString::Format("ResidualX%i_ROC1", Telescope->Channel());
        MapResidualX[Telescope->Channel()*10+1] = new TH1F(Name, Name, 40, -0.04, 0.04);
        MapResidualX[Telescope->Channel()*10+1]->SetXTitle("Local Telescope Residual-X (cm)");
        Name = TString::Format("ResidualY%i_ROC2", Telescope->Channel());
        MapResidualY[Telescope->Channel()*10+2] = new TH1F(Name, Name, 40, -0.02, 0.06);
        MapResidualY[Telescope->Channel()*10+2]->SetXTitle("Local Telescope Residual-Y (cm)");
        Name = TString::Format("ResidualX%i_ROC2", Telescope->Channel());
        MapResidualX[Telescope->Channel()*10+2] = new TH1F(Name, Name, 40, -0.04, 0.04);
        MapResidualX[Telescope->Channel()*10+2]->SetXTitle("Local Telescope Residual-X (cm)");
      }


      if (Telescope->NClusters() > 3) continue;


      if (Telescope->NTracks() > 0 && NTrkEvMap[Telescope->Channel()]++ < 10) {
        Telescope->DrawTracksAndHits( TString::Format("plots/Tracks_Ch%i_Ev%i.gif", Telescope->Channel(), NTrkEvMap[Telescope->Channel()]).Data() );
      }

      for (size_t itrack = 0; itrack != Telescope->NTracks(); ++itrack) {
        PLTTrack* Track = Telescope->Track(itrack);

	/// !!!! Write "IF statement" using variables grabbed from mask file to filter out tracks outside of active region !!!!
	//if(PLTMask::fMaskMap[0].GColStart==""){std::cout<<"DID THIS WORK I HOPE IT DID!!!!!!!!"<<std::endl;}



        HistBeamSpot[0]->Fill( Track->fPlaner[0][1], Track->fPlaner[0][2]);
        HistBeamSpot[1]->Fill( Track->fPlaner[1][0], Track->fPlaner[1][2]);
        HistBeamSpot[2]->Fill( Track->fPlaner[2][0], Track->fPlaner[2][1]);
        HistBeamSpot[3]->Fill( Track->fPlaner[2][0], Track->fPlaner[2][1]);
        std::cout<<Track->fPlaner[2][1]<<std::endl;
        MapSlopeY[Telescope->Channel()]->Fill(Track->fTVY/Track->fTVZ);
        MapSlopeX[Telescope->Channel()]->Fill(Track->fTVX/Track->fTVZ);
        MapSlope2D[Telescope->Channel()]->Fill(Track->fTVX/Track->fTVZ, Track->fTVY/Track->fTVZ);
        MapResidualY[Telescope->Channel()*10+0]->Fill(Track->LResidualY(0));
        MapResidualX[Telescope->Channel()*10+0]->Fill(Track->LResidualX(0));
        MapResidualY[Telescope->Channel()*10+1]->Fill(Track->LResidualY(1));
        MapResidualX[Telescope->Channel()*10+1]->Fill(Track->LResidualX(1));
        MapResidualY[Telescope->Channel()*10+2]->Fill(Track->LResidualY(2));
        MapResidualX[Telescope->Channel()*10+2]->Fill(Track->LResidualX(2));

      }
    }


  }


  TFile *f = new TFile("histo_slopes.root","RECREATE");

  TCanvas Can("BeamSpot", "BeamSpot", 900, 900);
  Can.Divide(3, 3);
  Can.cd(1);
  HistBeamSpot[0]->SetXTitle("X(cm)");
  HistBeamSpot[0]->SetYTitle("Y(cm)");
  HistBeamSpot[0]->Draw("colz");
  Can.cd(2);
  HistBeamSpot[1]->SetXTitle("X(cm)");
  HistBeamSpot[1]->SetYTitle("Y(cm)");
  HistBeamSpot[1]->Draw("colz");
  Can.cd(3);
  HistBeamSpot[2]->SetXTitle("X(cm)");
  HistBeamSpot[2]->SetYTitle("Y(cm)");
  HistBeamSpot[2]->Draw("colz");

  HistBeamSpot[3]->SetXTitle("X (cm)");
  HistBeamSpot[3]->SetYTitle("Y (cm)");
  HistBeamSpot[3]->Draw("colz");

  Can.cd(1+3);
  HistBeamSpot[0]->ProjectionX()->Draw("ep");
  Can.cd(2+3);
  HistBeamSpot[1]->ProjectionX()->Draw("ep");
  Can.cd(3+3);
  HistBeamSpot[2]->ProjectionX()->Draw("ep");

  Can.cd(1+6);
  HistBeamSpot[0]->ProjectionY()->Draw("ep");
  Can.cd(2+6);
  HistBeamSpot[1]->ProjectionY()->Draw("ep");
  Can.cd(3+6);
  HistBeamSpot[2]->ProjectionY()->Draw("ep");
  Can.SaveAs("plots/BeamSpot.gif");
  
  HistBeamSpot[0]->Write();
  HistBeamSpot[1]->Write();
  HistBeamSpot[2]->Write();
  HistBeamSpot[3]->Write();

  for (std::map<int, TH1F*>::iterator it = MapSlopeY.begin(); it != MapSlopeY.end(); ++it) {
    it->second->Write();
    TCanvas Can;
    Can.cd();
    it->second->Draw("hist");
    Can.SaveAs("plots/" + TString(it->second->GetName()) + ".gif");
    delete it->second;
  }

  for (std::map<int, TH1F*>::iterator it = MapSlopeX.begin(); it != MapSlopeX.end(); ++it) {
    it->second->Write();
    TCanvas Can;
    Can.cd();
    it->second->Draw("hist");
    Can.SaveAs("plots/" + TString(it->second->GetName()) + ".gif");
    delete it->second;
  }

  for (std::map<int, TH2F*>::iterator it = MapSlope2D.begin(); it != MapSlope2D.end(); ++it) {
    it->second->Write();
    //TCanvas Can;
    //Can.cd();
    //it->second->Draw("hist");
    //Can.SaveAs("plots/" + TString(it->second->GetName()) + ".gif");
    //delete it->second;
  }
  for (std::map<int, TH1F*>::iterator it = MapResidualY.begin(); it != MapResidualY.end(); ++it) {
    it->second->Write();
    TCanvas Can;
    Can.cd();
    it->second->Draw("hist");
    Can.SaveAs("plots/" + TString(it->second->GetName()) + ".gif");
    delete it->second;
  }
  for (std::map<int, TH1F*>::iterator it = MapResidualX.begin(); it != MapResidualX.end(); ++it) {
    it->second->Write();
    TCanvas Can;
    Can.cd();
    it->second->Draw("hist");
    Can.SaveAs("plots/" + TString(it->second->GetName()) + ".gif");
    delete it->second;
  }

  f->Close();

  return 0;
}


int main (int argc, char* argv[])
{
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " [DataFile.dat] [GainCal.dat] [AlignmentFile.dat] [MaskFile.txt](optional, blank if no mask file)" << std::endl;
    return 1;
  }

  std::string const DataFileName = argv[1];
  std::string const GainCalFileName = argv[2];
  std::string const AlignmentFileName = argv[3];
  std::string const MaskFileName = argv[4];

  //  if (argv[4] == "none"){

  if(DataFileName.substr(DataFileName.find_last_of(".")+1) == "dat")
    {
      MakeTracks(DataFileName, GainCalFileName, AlignmentFileName, false, MaskFileName);
    }
  else if(DataFileName.substr(DataFileName.find_last_of(".")+1) == "txt")
    {
      MakeTracks(DataFileName, GainCalFileName, AlignmentFileName, true, MaskFileName);
    }
  else
    {
    std::cerr << "Usage: please make sure the provided datafile ends on .dat or .txt" << std::endl;
    return 1;
    }
  //  }

  return 0;
}
