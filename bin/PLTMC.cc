////////////////////////////////////////////////////////////////////
//
// Dean Andrew Hidas <Dean.Andrew.Hidas@cern.ch>
//
// Created on: Sun Jun 26 16:16:58 CEST 2011
//
////////////////////////////////////////////////////////////////////


#include <iostream>
#include <fstream>
#include <stdint.h>
#include <math.h>

//// Custom h files used under /interface
#include "PLTHit.h"
#include "PLTAlignment.h"
#include "PLTU.h"

#include "TRandom.h"

void CountHitsInActiveRegionGood(int ACTIVEREGION[], int (&goodtracksarray)[3], int iroc, int (&ghits)[2], int Column, int Row)
{
  
  if (iroc == 0) {
    if (Column >= ACTIVEREGION[0] && Column <= ACTIVEREGION[1] && Column >= 0 && Column <= 51 && Row >= ACTIVEREGION[2] && Row <= ACTIVEREGION[3] && Row >= 0 && Row <= 79) {
      goodtracksarray[0] += 1;
      ghits[0] = 1;
    }
  }
  if (iroc == 1 && ghits[0] == 1) {
    if (Column >= ACTIVEREGION[4] && Column <= ACTIVEREGION[5] && Column >= 0 && Column <= 51 && Row >= ACTIVEREGION[6] && Row <= ACTIVEREGION[7] && Row >= 0 && Row <= 79) {
      goodtracksarray[1] += 1;
      ghits[1] = 1;
    }
  }
  if (iroc == 2 && ghits[1] == 1) {
    if (Column >= ACTIVEREGION[8] && Column <= ACTIVEREGION[9] && Column >= 0 && Column <= 51 && Row >= ACTIVEREGION[10] && Row <= ACTIVEREGION[11] && Row >= 0 && Row <= 79) {
      goodtracksarray[2] += 1;
    }
  }
  return;
}


void CountHitsInActiveRegionBad(int ACTIVEREGION[], int (&badtracksarray)[3], int iroc, int (&hits)[2], int Column, int Row)
{
  
  if (iroc == 0) {
    if (Column >= ACTIVEREGION[0] && Column <= ACTIVEREGION[1] && Column >= 0 && Column <= 51 && Row >= ACTIVEREGION[2] && Row <= ACTIVEREGION[3] && Row >= 0 && Row <= 79) {
      badtracksarray[0] += 1;
      hits[0] = 1;
    }
  }
  if (iroc == 1 && hits[0] == 1) {
    if (Column >= ACTIVEREGION[4] && Column <= ACTIVEREGION[5] && Column >= 0 && Column <= 51 && Row >= ACTIVEREGION[6] && Row <= ACTIVEREGION[7] && Row >= 0 && Row <= 79) {
      badtracksarray[1] += 1;
      hits[1] = 1;
    }
  }
  if (iroc == 2 && hits[1] == 1) {
    if (Column >= ACTIVEREGION[8] && Column <= ACTIVEREGION[9] && Column >= 0 && Column <= 51 && Row >= ACTIVEREGION[10] && Row <= ACTIVEREGION[11] && Row >= 0 && Row <= 79) {
      badtracksarray[2] += 1;
    }
  }
  return;
}


void GetPureNoise (std::vector<PLTHit*>& Hits, PLTAlignment& Alignment, int (&badtracksarray)[3], int ACTIVEREGION[], int (&totalhits)[3], int& totalbadhits)
{
  // Grab list of telescopes
  static std::vector<int> Channels = Alignment.GetListOfChannels();

  int Row, Column;
  for (/*std::vector<int>::iterator*/int ch = 0/*Channels.begin()*/; ch<1 /*!= Channels.end()*/; ++ch) {

    int hits[2] = {0,0};

    for (int iroc = 0; iroc != 3; ++iroc) {
      Column = gRandom->Integer(PLTU::NCOL) + PLTU::FIRSTCOL;
      Row    = gRandom->Integer(PLTU::NROW) + PLTU::FIRSTROW;
      if (Column >= PLTU::FIRSTCOL && Column <= PLTU::LASTCOL && Row >= PLTU::FIRSTROW && Row <= PLTU::LASTROW) {
        Hits.push_back( new PLTHit(/***/ch, iroc, Column, Row, gRandom->Poisson(150)) );
	totalhits[iroc] = 1;
      } else {
        std::cout << "out of range" << std::endl;
      }

      // Add to hits if in range of active pixels
      // loop for each ROC
      CountHitsInActiveRegionBad(ACTIVEREGION,badtracksarray, iroc, hits, Column, Row); 

    }
    if (totalhits[0] == 1 && totalhits[1] == 1 && totalhits[2] == 1) {
      totalbadhits += 1; 
    }
  }

  return;
}


void GetTracksCollisions (std::vector<PLTHit*>& Hits, PLTAlignment& Alignment, int (&goodtracksarray)[3], int ACTIVEREGION[], int (&totalhits)[3], int& totalgoodhits)
{
  // Grab list of telescopes
  // Case 0 for PLTMC() function
  static std::vector<int> Channels = Alignment.GetListOfChannels();


  static int const NTracks = 1;//gRandom->Integer(10);

  for (int itrack = 0; itrack < NTracks; ++itrack) {
    int const Channel = Channels[ gRandom->Integer(Channels.size()) ];
    int const ROC     = gRandom->Integer(3);
    //printf("Channel: %2i  ROC: %i\n", Channel, ROC);

    float const lx = 0.8 * (gRandom->Rndm() - 0.5);
    float const ly = 0.8 * (gRandom->Rndm() - 0.5);
    //float const lx = 0.45 * (gRandom->Gaus(0.5,0.5)-0.5);
    //float const ly = 0.45 * (gRandom->Gaus(0.527,5.0)-0.5);
    //printf(" lx ly: %15E  %15E\n", lx, ly);

    static std::vector<float> TXYZ;
    Alignment.LtoTXYZ(TXYZ, lx, ly, Channel, ROC);
    //printf(" TXYZ: %15E  %15E  %15E\n", TXYZ[0], TXYZ[1], TXYZ[2]);

    static std::vector<float> GXYZ;
    Alignment.TtoGXYZ(GXYZ, TXYZ[0], TXYZ[1], TXYZ[2], Channel, ROC);
    //printf(" GXYZ: %15E  %15E  %15E\n", GXYZ[0], GXYZ[1], GXYZ[2]);

    float const SlopeX = GXYZ[0] / GXYZ[2];
    float const SlopeY = GXYZ[1] / GXYZ[2];
    //float const SlopeX = gRandom->Gaus(0.0,0.005);
    //float const SlopeY = gRandom->Gaus(0.027,0.02);
    //if (Channel == 3){
      //printf(" Slope X Y: %15E  %15E\n", SlopeX, SlopeY);}

    int ghits[2] = {0,0};
    
    for (size_t iroc = 0; iroc != 3; ++iroc) {
      std::vector<float> VP;
      Alignment.LtoGXYZ(VP, 0, 0, Channel, iroc);
      //printf("  VP XYZ: %15E  %15E  %15E\n", VP[0], VP[1], VP[2]);

      float const GZ = VP[2];
      float const GX = SlopeX * GZ;
      float const GY = SlopeY * GZ;
      //printf("ROC %1i  GXYZ: %15E  %15E  %15E\n", iroc, GX, GY, GZ);


      std::vector<float> T;
      Alignment.GtoTXYZ(T, GX, GY, GZ, Channel, iroc);
      //if (Channel == 3) printf("HI %15E\n", GX - T[0]);
      //if (Channel == 3)
      //printf("ROC %1i TX TY TZ  %15E %15E %15E\n", iroc, T[0], T[1], T[2]);

      std::pair<float, float> LXY = Alignment.TtoLXY(T[0], T[1], Channel, iroc);
      std::pair<int, int>     PXY = Alignment.PXYfromLXY(LXY);

      // Add some jitter if you want
      PXY.first  += (int) gRandom->Gaus(0, 1.2);
      PXY.second += (int) gRandom->Gaus(0, 1.2);
      int const PX = PXY.first;
      int const PY = PXY.second;

      // Just some random adc value
      int const adc = gRandom->Poisson(150);

      // Add it as a hit if it's in the range of the diamond
      //printf("ROC %1i LX PX LY PY   %15E %2i  %15E %2i\n", iroc, LXY.first, PX, LXY.second, PY);
      if (PX >= PLTU::FIRSTCOL && PX <= PLTU::LASTCOL && PY >= PLTU::FIRSTROW && PY <= PLTU::LASTROW) {
        Hits.push_back( new PLTHit(Channel, iroc, PX, PY, adc) );
	totalhits[iroc] = 1;
      } else {
        //printf("LX PX LY PY   %2i %6.2f %2i   %2i %6.2f %2i\n", LXY.first, PX, rXY.second, PY);
      }

      // Add to hits if in range of active pixels
      // loop for each ROC
      CountHitsInActiveRegionGood(ACTIVEREGION,goodtracksarray, iroc, ghits, PX, PY); 

    }
    if (totalhits[0] == 1 && totalhits[1] == 1 && totalhits[2] == 1) {
      totalgoodhits += 1; 
    }
  }

  return;
}



void GetTracksCollisions2 (std::vector<PLTHit*>& Hits, PLTAlignment& Alignment, int (&badtracksarray)[3], int ACTIVEREGION[], int (&totalhits)[3], int& totalbadhits)
{
  // Grab list of telescopes
  static std::vector<int> Channels = Alignment.GetListOfChannels();



  static int const NTracks = 1;//gRandom->Integer(4);

  for (int itrack = 0; itrack < NTracks; ++itrack) {
    int const Channel = Channels[ gRandom->Integer(Channels.size()) ];
    int const ROC     = gRandom->Integer(3);
    //printf("Channel: %2i  ROC: %i\n", Channel, ROC);

    // pick a starting point on the first ROC
    int const StartCol = gRandom->Integer(PLTU::NCOL) + PLTU::FIRSTCOL;
    int const StartRow = gRandom->Integer(PLTU::NROW) + PLTU::FIRSTROW;

    //printf(" StartCol: %4i  StartRow: %4i\n", StartCol, StartRow);

    float const lx = Alignment.PXtoLX(StartCol);
    float const ly = Alignment.PYtoLY(StartRow);

    //printf(" lx ly: %15E  %15E\n", lx, ly);

    static std::vector<float> TXYZ;
    Alignment.LtoTXYZ(TXYZ, lx, ly, Channel, ROC);
    //printf(" TXYZ: %15E  %15E  %15E\n", TXYZ[0], TXYZ[1], TXYZ[2]);

    static std::vector<float> GXYZ;
    Alignment.TtoGXYZ(GXYZ, TXYZ[0], TXYZ[1], TXYZ[2], Channel, ROC);
    //printf(" GXYZ: %15E  %15E  %15E\n", GXYZ[0], GXYZ[1], GXYZ[2]);

    float const SlopeX = GXYZ[0] / GXYZ[2];
    float const SlopeY = GXYZ[1] / GXYZ[2];
    //if (Channel == 3)
    //printf(" Slope X Y: %15E  %15E\n", SlopeX, SlopeY);

    int hits[2] = {0,0};

    for (size_t iroc = 0; iroc != 3; ++iroc) {
      std::vector<float> VP;
      Alignment.LtoGXYZ(VP, 0, 0, Channel, iroc);
      //printf("  VP XYZ: %15E  %15E  %15E\n", VP[0], VP[1], VP[2]);

      float const GZ = VP[2];
      float const GX = SlopeX * GZ;
      float const GY = SlopeY * GZ;
      //printf("ROC %1i  GXYZ: %15E  %15E  %15E\n", iroc, GX, GY, GZ);


      std::vector<float> T;
      Alignment.GtoTXYZ(T, GX, GY, GZ, Channel, iroc);
      //if (Channel == 3) printf("HI %15E\n", GX - T[0]);
      //if (Channel == 3)
      //printf("ROC %1i TX TY TZ  %15E %15E %15E\n", iroc, T[0], T[1], T[2]);

      std::pair<float, float> LXY = Alignment.TtoLXY(T[0], T[1], Channel, iroc);
      std::pair<int, int>     PXY = Alignment.PXYfromLXY(LXY);

      // Add some jitter
      PXY.first  += (int) gRandom->Gaus(0, 1.2);
      PXY.second += (int) gRandom->Gaus(0, 1.2);
      int const PX = PXY.first;
      int const PY = PXY.second;

      // Just some random adc value
      int const adc = gRandom->Poisson(150);

      // Add it as a hit if it's in the range of the diamond
      //printf("ROC %1i LX PX LY PY   %15E %2i  %15E %2i\n", iroc, LXY.first, PX, LXY.second, PY);
      if (PX >= PLTU::FIRSTCOL && PX <= PLTU::LASTCOL && PY >= PLTU::FIRSTROW && PY <= PLTU::LASTROW) {
        Hits.push_back( new PLTHit(Channel, iroc, PX, PY, adc) );
	totalhits[iroc] = 1;
      } else {
        //printf("LX PX LY PY   %2i %6.2f %2i   %2i %6.2f %2i\n", LXY.first, PX, rXY.second, PY);
      }

      // Add to hits if in range of active pixels
      // loop for each ROC
      CountHitsInActiveRegionBad(ACTIVEREGION,badtracksarray, iroc, hits, PX, PY); 

    }
    if (totalhits[0] == 1 && totalhits[1] == 1 && totalhits[2] == 1) {
      totalbadhits += 1; 
    }
  }

  return;
}



void GetTracksParallel (std::vector<PLTHit*>& Hits, PLTAlignment& Alignment, int (&badtracksarray)[3], int ACTIVEREGION[], int (&totalhits)[3], int& totalbadhits)
{
  // Grab list of telescopes
  static std::vector<int> Channels = Alignment.GetListOfChannels();


  static int const NTracks = 1;//gRandom->Integer(10);

  for (int itrack = 0; itrack < NTracks; ++itrack) {
    int const Channel = Channels[ gRandom->Integer(Channels.size()) ];
    int const ROC     = gRandom->Integer(3);

    //printf("Channel: %2i  ROC: %i\n", Channel, ROC);

    // pick a starting point on the first ROC
    int const StartCol = gRandom->Integer(PLTU::NCOL) + PLTU::FIRSTCOL;
    int const StartRow = gRandom->Integer(PLTU::NROW) + PLTU::FIRSTROW;

    //printf(" StartCol: %4i  StartRow: %4i\n", StartCol, StartRow);

    float const lx = Alignment.PXtoLX(StartCol);
    float const ly = Alignment.PYtoLY(StartRow);

    //printf(" lx ly: %15E  %15E\n", lx, ly);

    static std::vector<float> TXYZ;
    Alignment.LtoTXYZ(TXYZ, lx, ly, Channel, ROC);
    //printf(" TXYZ: %15E  %15E  %15E\n", TXYZ[0], TXYZ[1], TXYZ[2]);

    static std::vector<float> GXYZ;
    Alignment.TtoGXYZ(GXYZ, TXYZ[0], TXYZ[1], TXYZ[2], Channel, ROC);
    float const GX = GXYZ[0];
    float const GY = GXYZ[1];
    float const GZ = GXYZ[2];
    //printf(" GXYZ: %15E  %15E  %15E\n", GXYZ[0], GXYZ[1], GXYZ[2]);

    int hits[2] = {0,0};

    for (size_t iroc = 0; iroc != 3; ++iroc) {

      std::vector<float> T;
      Alignment.GtoTXYZ(T, GX, GY, GZ, Channel, iroc);

      std::pair<float, float> LXY = Alignment.TtoLXY(T[0], T[1], Channel, iroc);
      std::pair<int, int>     PXY = Alignment.PXYfromLXY(LXY);
      int const PX = PXY.first;
      int const PY = PXY.second;

      // Just some random adc value
      int const adc = gRandom->Poisson(150);

      // Add it as a hit if it's in the range of the diamond
      //printf("iroc StartCol LX PX StartRow LY PY  %2i  %2i %6.2f %2i   %2i %6.2f %2i\n", iroc, StartCol, LXY.first, PX, StartRow, LXY.second, PY);
      if (PX >= PLTU::FIRSTCOL && PX <= PLTU::LASTCOL && PY >= PLTU::FIRSTROW && PY <= PLTU::LASTROW) {
        Hits.push_back( new PLTHit(Channel, iroc, PX, PY, adc) );
	totalhits[iroc] = 1;
      } else {
        //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i\n", StartCol, LXY.first, PX, StartRow, LXY.second, PY);
      }

      // Add to hits if in range of active pixels
      // loop for each ROC
      CountHitsInActiveRegionBad(ACTIVEREGION,badtracksarray, iroc, hits, PX, PY); 

    }
    if (totalhits[0] == 1 && totalhits[1] == 1 && totalhits[2] == 1) {
      totalbadhits += 1; 
    }

  }

  return;
}
void GetTracksRandomSlope (std::vector<PLTHit*>& Hits, PLTAlignment& Alignment, int (&badtracksarray)[3], int ACTIVEREGION[], int (&totalhits)[3], int& totalbadhits)
{
  // This function to generate events hitting telescopes head on

  int const NTelescopes = 1;
  for (int i = 1; i <= NTelescopes; ++i) {

    int const NTracks = 1;//gRandom->Integer(10);

    for (int itrack = 0; itrack < NTracks; ++itrack) {

      // pick a starting point on the first ROC
      int const StartCol = gRandom->Integer(PLTU::NCOL) + PLTU::FIRSTCOL;
      int const StartRow = gRandom->Integer(PLTU::NROW) + PLTU::FIRSTROW;

      float const SlopeX = 9.0 * (gRandom->Rndm() - 0.5);
      float const SlopeY = 9.0 * (gRandom->Rndm() - 0.5);

      int hits[2] = {0,0};

      for (int r = 0; r != 3; ++r) {
        //PLTAlignment::CP* C = Alignment.GetCP(i, r);

        // make straight track, see where that hits a plane if it's shifted..
        // Optionally give it some fuzz..

        // Use L coord system:
        // THINK ABOUT THIS FOR ROTATIONS...
        float const LZ = Alignment.LZ(i, r);
        float const LX = Alignment.PXtoLX(StartCol + SlopeX * LZ);
        float const LY = Alignment.PYtoLY(StartRow + SlopeY * LZ);

        std::pair<float, float> LXY = Alignment.TtoLXY(LX, LY, i, r);

        int const PX = Alignment.PXfromLX(LXY.first);
        int const PY = Alignment.PYfromLY(LXY.second);


        //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i    CLX CLY: %12.3f %12.3f\n", StartCol, LX, PX, StartRow, LY, PY, C->LX, C->LY);

        // Just some random adc value
        int const adc = gRandom->Poisson(150);

        // Add it as a hit if it's in the range of the diamond
        if (PX >= PLTU::FIRSTCOL && PX <= PLTU::LASTCOL && PY >= PLTU::FIRSTROW && PY <= PLTU::LASTROW) {
          Hits.push_back( new PLTHit(i, r, PX, PY, adc) );
	  totalhits[r] = 1;
        } else {
          //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i\n", StartCol, LX, PX, StartRow, LY, PY);
        }

      // Add to hits if in range of active pixels
      // loop for each ROC
	CountHitsInActiveRegionBad(ACTIVEREGION,badtracksarray, r, hits, PX, PY); 

      }
      if (totalhits[0] == 1 && totalhits[1] == 1 && totalhits[2] == 1) {
	totalbadhits += 1; 
      }
    }
  }
  return;
}
void GetTracksHeadOnFirstROCMultiTracks (std::vector<PLTHit*>& Hits, PLTAlignment& Alignment, int (&badtracksarray)[3], int ACTIVEREGION[], int (&totalhits)[3], int& totalbadhits)
{
  // This function to generate events hitting telescopes head on

  int const NTelescopes = 1;
  for (int i = 1; i <= NTelescopes; ++i) {

    int const NTracks = gRandom->Integer(10);

    for (int itrack = 0; itrack < NTracks; ++itrack) {

      // pick a starting point on the first ROC
      int const StartCol = gRandom->Integer(PLTU::NCOL) + PLTU::FIRSTCOL;
      int const StartRow = gRandom->Integer(PLTU::NROW) + PLTU::FIRSTROW;

      float const SlopeX = 9.0 * (gRandom->Rndm() - 0.5);
      float const SlopeY = 9.0 * (gRandom->Rndm() - 0.5);

      int hits[2] = {0,0};

      for (int r = 0; r != 3; ++r) {
        //PLTAlignment::CP* C = Alignment.GetCP(i, r);

        // make straight track, see where that hits a plane if it's shifted..
        // Optionally give it some fuzz..

        // Use L coord system:
        // THINK ABOUT THIS FOR ROTATIONS...
        float const LZ = Alignment.LZ(i, r);
        float const LX = Alignment.PXtoLX(StartCol + SlopeX * LZ);
        float const LY = Alignment.PYtoLY(StartRow + SlopeY * LZ);

        std::pair<float, float> LXY = Alignment.TtoLXY(LX, LY, i, r);

        int const PX = Alignment.PXfromLX(LXY.first);
        int const PY = Alignment.PYfromLY(LXY.second);


        //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i    CLX CLY: %12.3f %12.3f\n", StartCol, LX, PX, StartRow, LY, PY, C->LX, C->LY);

        // Just some random adc value
        int const adc = gRandom->Poisson(150);

        // Add it as a hit if it's in the range of the diamond
        if (PX >= PLTU::FIRSTCOL && PX <= PLTU::LASTCOL && PY >= PLTU::FIRSTROW && PY <= PLTU::LASTROW) {
          Hits.push_back( new PLTHit(i, r, PX, PY, adc) );
	  totalhits[r] = 1;
        } else {
          //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i\n", StartCol, LX, PX, StartRow, LY, PY);
        }

	// Add to hits if in range of active pixels
	// loop for each ROC
	CountHitsInActiveRegionBad(ACTIVEREGION,badtracksarray, r, hits, PX, PY); 
      
      }
      if (totalhits[0] == 1 && totalhits[1] == 1 && totalhits[2] == 1) {
	totalbadhits += 1; 
      }
    }
  }
  return;
}


void GetRandTracksROCEfficiencies (std::vector<PLTHit*>& Hits, PLTAlignment& Alignment, float const Eff0, float const Eff1, float const Eff2, int (&badtracksarray)[3], int ACTIVEREGION[], int (&totalhits)[3], int& totalbadhits)
{
  // This function to generate events hitting telescopes head on

  float const Eff[3] = { Eff0, Eff1, Eff2 };

  int const NTelescopes = 1;//36;
  //std::cout << "Event" << std::endl;
  for (int i = 1; i <= NTelescopes; ++i) {

    // pick a starting point.  +/- 10 should cover shifts in alignment
    int const StartCol = gRandom->Integer(PLTU::NCOL + 20) + PLTU::FIRSTCOL - 10;
    int const StartRow = gRandom->Integer(PLTU::NROW + 20) + PLTU::FIRSTROW - 10;

    float const SlopeX = 2.0 * (gRandom->Rndm() - 0.5);
    float const SlopeY = 2.0 * (gRandom->Rndm() - 0.5);

    int hits[2] = {0,0};

    for (int r = 0; r != 3; ++r) {
      //PLTAlignment::CP* C = Alignment.GetCP(i, r);

      // make straight track, see where that hits a plane if it's shifted..
      // Optionally give it some fuzz..

      // Use L coord system:
      // THINK ABOUT THIS FOR ROTATIONS...
      float const LZ = Alignment.LZ(i, r);
      float const LX = Alignment.PXtoLX(StartCol + SlopeX * LZ);
      float const LY = Alignment.PYtoLY(StartRow + SlopeY * LZ);

      std::pair<float, float> LXY = Alignment.TtoLXY(LX, LY, i, r);

      int const PX = Alignment.PXfromLX(LXY.first);
      int const PY = Alignment.PYfromLY(LXY.second);


      //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i    CLX CLY: %12.3f %12.3f\n", StartCol, LX, PX, StartRow, LY, PY, C->LX, C->LY);

      // Just some random adc value
      int const adc = gRandom->Poisson(150);

      // Add it as a hit if it's in the range of the diamond
      if (PX >= PLTU::FIRSTCOL && PX <= PLTU::LASTCOL && PY >= PLTU::FIRSTROW && PY <= PLTU::LASTROW) {
        //printf("Ch ROC PX PY %2i %1i %3i %3i\n", i, r, PX, PY);
        if (gRandom->Rndm() < Eff[r]) {
          Hits.push_back( new PLTHit(i, r, PX, PY, adc) );
	  totalhits[r] = 1;
        }
      } else {
        //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i\n", StartCol, LX, PX, StartRow, LY, PY);
      }

      // Add to hits if in range of active pixels
      // loop for each ROC
      CountHitsInActiveRegionBad(ACTIVEREGION,badtracksarray, r, hits, PX, PY); 
      
    }
    if (totalhits[0] == 1 && totalhits[1] == 1 && totalhits[2] == 1) {
      totalbadhits += 1; 
    }
  }
  return;
}


void GetTracksROCEfficiencies (std::vector<PLTHit*>& Hits, PLTAlignment& Alignment, float const Eff0, float const Eff1, float const Eff2, int (&badtracksarray)[3], int ACTIVEREGION[], int (&totalhits)[3], int& totalbadhits)
{
  // This function to generate events hitting telescopes head on

  float const Eff[3] = { Eff0, Eff1, Eff2 };

  int const NTelescopes = 1;
  for (int i = 1; i <= NTelescopes; ++i) {

    // pick a starting point.  +/- 10 should cover shifts in alignment
    int const StartCol = gRandom->Integer(PLTU::NCOL - 20) + PLTU::FIRSTCOL + 10;
    int const StartRow = gRandom->Integer(PLTU::NROW - 20) + PLTU::FIRSTROW + 10;

    int hits[2] = {0,0};

    for (int r = 0; r != 3; ++r) {
      //PLTAlignment::CP* C = Alignment.GetCP(i, r);

      // make straight track, see where that hits a plane if it's shifted..
      // Optionally give it some fuzz..

      // Use L coord system:
      // THINK ABOUT THIS FOR ROTATIONS...
      float const LX = Alignment.PXtoLX(StartCol);
      float const LY = Alignment.PYtoLY(StartRow);

      std::pair<float, float> LXY = Alignment.TtoLXY(LX, LY, i, r);

      int const PX = Alignment.PXfromLX(LXY.first);
      int const PY = Alignment.PYfromLY(LXY.second);


      //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i    CLX CLY: %12.3f %12.3f\n", StartCol, LX, PX, StartRow, LY, PY, C->LX, C->LY);

      // Just some random adc value
      int const adc = gRandom->Poisson(150);

      // Add it as a hit if it's in the range of the diamond
      if (PX >= PLTU::FIRSTCOL && PX <= PLTU::LASTCOL && PY >= PLTU::FIRSTROW && PY <= PLTU::LASTROW) {
        if (gRandom->Rndm() < Eff[r]) {
          Hits.push_back( new PLTHit(i, r, PX, PY, adc) );
	  totalhits[r] = 1;
        }
      } else {
        //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i\n", StartCol, LX, PX, StartRow, LY, PY);
      }

      // Add to hits if in range of active pixels
      // loop for each ROC
      CountHitsInActiveRegionBad(ACTIVEREGION, badtracksarray, r, hits, PX, PY); 
      
    }
    if (totalhits[0] == 1 && totalhits[1] == 1 && totalhits[2] == 1) {
      totalbadhits += 1; 
    }
  }
  return;
}


void GetSpecificClusters (std::vector<PLTHit*>& Hits, PLTAlignment& Alignment, int (&badtracksarray)[3], int ACTIVEREGION[], int (&totalhits)[3], int& totalbadhits)
{
  // This function to generate events hitting telescopes head on

  int const NTelescopes = 1;
  for (int i = 1; i <= NTelescopes; ++i) {

    // pick a starting point.  +/- 10 should cover shifts in alignment
    int const StartCol = gRandom->Integer(10) + PLTU::FIRSTCOL + 5;
    int const StartRow = gRandom->Integer(10) + PLTU::FIRSTROW + 5;

    int hits[2] = {0,0};

    for (int r = 0; r != 3; ++r) {
      //PLTAlignment::CP* C = Alignment.GetCP(i, r);

      // make straight track, see where that hits a plane if it's shifted..
      // Optionally give it some fuzz..

      // Use L coord system:
      // THINK ABOUT THIS FOR ROTATIONS...
      float const LX = Alignment.PXtoLX(StartCol);
      float const LY = Alignment.PYtoLY(StartRow);

      std::pair<float, float> LXY = Alignment.TtoLXY(LX, LY, i, r);

      int const PX = Alignment.PXfromLX(LXY.first);
      int const PY = Alignment.PYfromLY(LXY.second);


      //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i    CLX CLY: %12.3f %12.3f\n", StartCol, LX, PX, StartRow, LY, PY, C->LX, C->LY);

      // Just some random adc value
      int const adc = gRandom->Poisson(150);

      // Add it
      Hits.push_back( new PLTHit(i, r, PX, PY, adc) );
      Hits.push_back( new PLTHit(i, r, PX+1, PY, adc) );
      totalhits[r] = 1;

      // Add to hits if in range of active pixels
      // loop for each ROC
      CountHitsInActiveRegionBad(ACTIVEREGION,badtracksarray, r, hits, PX, PY); 
      
    }
    if (totalhits[0] == 1 && totalhits[1] == 1 && totalhits[2] == 1) {
      totalbadhits += 1; 
    }
  }
  return;
}


void GetTracksParallelGaus (std::vector<PLTHit*>& Hits, PLTAlignment& Alignment, int (&badtracksarray)[3], int ACTIVEREGION[], int (&totalhits)[3], int& totalbadhits)
{
  // This function to generate events hitting telescopes head on

  // Define the offset in XY and the widths in XY
  float const ColOffset = 0.0;
  float const RowOffset = 0.0;
  float const ColWidth = 12;
  float const RowWidth = 12;

  int const NTelescopes = 1;
  for (int i = 1; i <= NTelescopes; ++i) {

    // pick a starting point with width and offset
    int const StartCol = gRandom->Gaus( (PLTU::FIRSTCOL + PLTU::LASTCOL + 1.0) / 2.0 + ColOffset, ColWidth);
    int const StartRow = gRandom->Gaus( (PLTU::FIRSTROW + PLTU::LASTROW + 1.0) / 2.0 + RowOffset, RowWidth);

    int hits[2] = {0,0};

    for (int r = 0; r != 3; ++r) {
      // make straight track, see where that hits a plane if it's shifted..
      // Optionally give it some fuzz..

      // Use L coord system:
      // THINK ABOUT THIS FOR ROTATIONS...
      float const LX = Alignment.PXtoLX(StartCol);
      float const LY = Alignment.PYtoLY(StartRow);

      std::pair<float, float> LXY = Alignment.TtoLXY(LX, LY, i, r);

      int const PX = Alignment.PXfromLX(LXY.first);
      int const PY = Alignment.PYfromLY(LXY.second);


      //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i\n", StartCol, LX, PX, StartRow, LY, PY);

      // Just some random adc value
      int const adc = gRandom->Poisson(150);

      // Add it as a hit if it's in the range of the diamond
      if (PX >= PLTU::FIRSTCOL && PX <= PLTU::LASTCOL && PY >= PLTU::FIRSTROW && PY <= PLTU::LASTROW) {
        Hits.push_back( new PLTHit(i, r, PX, PY, adc) );
	totalhits[r] = 1;
      } else {
        //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i\n", StartCol, LX, PX, StartRow, LY, PY);
      }

      // Add to hits if in range of active pixels
      // loop for each ROC
      CountHitsInActiveRegionBad(ACTIVEREGION,badtracksarray, r, hits, PX, PY); 
      
    }
    if (totalhits[0] == 1 && totalhits[1] == 1 && totalhits[2] == 1) {
      totalbadhits += 1; 
    }
  }
  return;
}


void GetTracksHeadOn (std::vector<PLTHit*>& Hits, PLTAlignment& Alignment, int (&badtracksarray)[3],int  ACTIVEREGION[], int (&totalhits)[3], int& totalbadhits)
{
  // This function to generate events hitting telescopes head on

//   int const NTelescopes = 1;
//   for (int i = 1; i <= NTelescopes; ++i) {

//     // pick a starting point.  +/- 10 should cover shifts in alignment
//     int const StartCol = gRandom->Integer(PLTU::NCOL + 20) + PLTU::FIRSTCOL - 10;
//     int const StartRow = gRandom->Integer(PLTU::NROW + 20) + PLTU::FIRSTROW - 10;

//     int hits[2] = {0,0};

//     for (int r = 0; r != 3; ++r) {
//       //PLTAlignment::CP* C = Alignment.GetCP(i, r);

//       // make straight track, see where that hits a plane if it's shifted..
//       // Optionally give it some fuzz..

//       // Use L coord system:
//       // THINK ABOUT THIS FOR ROTATIONS...
//       float const LX = Alignment.PXtoLX(StartCol);
//       float const LY = Alignment.PYtoLY(StartRow);
//       //printf("LY  %.13E\n", LY);

//       std::pair<float, float> LXY = Alignment.TtoLXY(LX, LY, i, r);
//       //printf("LXY %.13E\n", LXY.second);

//       int const PX = Alignment.PXfromLX(LXY.first);
//       int const PY = Alignment.PYfromLY(LXY.second);

//       //std::cout << "PY " << PY << std::endl;

//       //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i    CLX CLY: %12.3f %12.3f\n", StartCol, LX, PX, StartRow, LY, PY, C->LX, C->LY);

  static std::vector<int> Channels = Alignment.GetListOfChannels();


  static int const NTracks = 1;//gRandom->Integer(10);

  for (int itrack = 0; itrack < NTracks; ++itrack) {
    int const Channel = Channels[ gRandom->Integer(Channels.size()) ];
    int const ROC     = gRandom->Integer(3);
    //printf("Channel: %2i  ROC: %i\n", Channel, ROC);

    float const lx = 0.8 * (gRandom->Rndm() - 0.5);
    float const ly = 0.8 * (gRandom->Rndm() - 0.5);
    //float const lx = 0.45 * (gRandom->Gaus(0.5,0.5)-0.5);
    //float const ly = 0.45 * (gRandom->Gaus(0.527,5.0)-0.5);
    //printf(" lx ly: %15E  %15E\n", lx, ly);

    static std::vector<float> TXYZ;
    Alignment.LtoTXYZ(TXYZ, lx, ly, Channel, ROC);
    //printf(" TXYZ: %15E  %15E  %15E\n", TXYZ[0], TXYZ[1], TXYZ[2]);

    static std::vector<float> GXYZ;
    Alignment.TtoGXYZ(GXYZ, TXYZ[0], TXYZ[1], TXYZ[2], Channel, ROC);
    //printf(" GXYZ: %15E  %15E  %15E\n", GXYZ[0], GXYZ[1], GXYZ[2]);

    //float const SlopeX = GXYZ[0] / GXYZ[2];
    //float const SlopeY = GXYZ[1] / GXYZ[2];
    float const SlopeX = gRandom->Rndm();
    printf("%15E\n", SlopeX);
    float const SlopeY = gRandom->Rndm();
    //if (Channel == 3){
      //printf(" Slope X Y: %15E  %15E\n", SlopeX, SlopeY);}

    int ghits[2] = {0,0};
    
    for (size_t r = 0; r != 3; ++r) {
      std::vector<float> VP;
      Alignment.LtoGXYZ(VP, 0, 0, Channel, r);
      //printf("  VP XYZ: %15E  %15E  %15E\n", VP[0], VP[1], VP[2]);

      float const GZ = VP[2];
      float const GX = SlopeX * GZ;
      float const GY = SlopeY * GZ;
      //printf("ROC %1i  GXYZ: %15E  %15E  %15E\n", iroc, GX, GY, GZ);


      std::vector<float> T;
      Alignment.GtoTXYZ(T, GX, GY, GZ, Channel, r);
      //if (Channel == 3) printf("HI %15E\n", GX - T[0]);
      //if (Channel == 3)
      //printf("ROC %1i TX TY TZ  %15E %15E %15E\n", iroc, T[0], T[1], T[2]);

      std::pair<float, float> LXY = Alignment.TtoLXY(T[0], T[1], Channel, r);
      std::pair<int, int>     PXY = Alignment.PXYfromLXY(LXY);

      // Add some jitter if you want
      PXY.first  += (int) gRandom->Gaus(0, 1.2);
      PXY.second += (int) gRandom->Gaus(0, 1.2);
      int const PX = PXY.first;
      int const PY = PXY.second;

      // Just some random adc value
      int const adc = gRandom->Poisson(150);

      // Add it as a hit if it's in the range of the diamond
      if (PX >= PLTU::FIRSTCOL && PX <= PLTU::LASTCOL && PY >= PLTU::FIRSTROW && PY <= PLTU::LASTROW) {
        Hits.push_back( new PLTHit(Channel, r, PX, PY, adc) );
	totalhits[r] = 1;
      } else {
        //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i\n", StartCol, LX, PX, StartRow, LY, PY);
      }

      // Add to hits if in range of active pixels
      // loop for each ROC
      CountHitsInActiveRegionBad(ACTIVEREGION,badtracksarray, r, ghits, PX, PY); 
      
    }
    if (totalhits[0] == 1 && totalhits[1] == 1 && totalhits[2] == 1) {
      totalbadhits += 1; 
    }
  }
  return;
}


void GetTracksHeadOnFirstROC (std::vector<PLTHit*>& Hits, PLTAlignment& Alignment, int (&badtracksarray)[3], int ACTIVEREGION[], int (&totalhits)[3], int& totalbadhits)
{
  // This function to generate events hitting telescopes head on

  for (int i = 15; i <= 15; ++i) {

    // pick a starting point on the first ROC
    int const StartCol = gRandom->Integer(PLTU::NCOL) + PLTU::FIRSTCOL;
    int const StartRow = gRandom->Integer(PLTU::NROW) + PLTU::FIRSTROW;

    int hits[2] = {0,0};

    for (int r = 0; r != 3; ++r) {
      //PLTAlignment::CP* C = Alignment.GetCP(i, r);

      // make straight track, see where that hits a plane if it's shifted..
      // Optionally give it some fuzz..

      // Use L coord system:
      // THINK ABOUT THIS FOR ROTATIONS...
      float const LX = Alignment.PXtoLX(StartCol);
      float const LY = Alignment.PYtoLY(StartRow);

      std::pair<float, float> LXY = Alignment.TtoLXY(LX, LY, i, r);

      int const PX = Alignment.PXfromLX(LXY.first);
      int const PY = Alignment.PYfromLY(LXY.second);


      //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i\n", StartCol, LX, PX, StartRow, LY, PY);

      // Just some random adc value
      int const adc = gRandom->Poisson(150);

      // Add it as a hit if it's in the range of the diamond
      if (PX >= PLTU::FIRSTCOL && PX <= PLTU::LASTCOL && PY >= PLTU::FIRSTROW && PY <= PLTU::LASTROW) {
        Hits.push_back( new PLTHit(i, r, PX, PY, adc) );
	totalhits[r] = 1;
      } else {
        //printf("StartCol LX PX StartRow LY PY   %2i %6.2f %2i   %2i %6.2f %2i\n", StartCol, LX, PX, StartRow, LY, PY);
      }

      // Add to hits if in range of active pixels
      // loop for each ROC
      CountHitsInActiveRegionBad(ACTIVEREGION,badtracksarray, r, hits, PX, PY); 
      
    }
    if (totalhits[0] == 1 && totalhits[1] == 1 && totalhits[2] == 1) {
      totalbadhits += 1; 
    }
  }
  return;
}


void GetGausHitsOneROC (std::vector<PLTHit*>& Hits, PLTAlignment& Alignment, int (&badtracksarray)[3], int ACTIVEREGION[], int (&totalhits)[3], int& totalbadhits)
{
  // This function to generate events hitting telescopes head on

  int const Channel = 1;

  float const ColOffset = 0;
  float const RowOffset = 0;
  float const ColWidth = 20;
  float const RowWidth = 30;

  // pick a starting point on the first ROC
  int const PX = gRandom->Gaus( ((float) (PLTU::FIRSTCOL + PLTU::LASTCOL) + 1.0) / 2.0 + ColOffset, ColWidth);
  int const PY = gRandom->Gaus( ((float) (PLTU::FIRSTROW + PLTU::LASTROW) + 1.0) / 2.0 + RowOffset, RowWidth);


  int const ROC = 0;


  // Just some random adc value
  int const adc = gRandom->Poisson(150);

  // Add it as a hit if it's in the range of the diamond
  if (PX >= PLTU::FIRSTCOL && PX <= PLTU::LASTCOL && PY >= PLTU::FIRSTROW && PY <= PLTU::LASTROW) {
    Hits.push_back( new PLTHit(Channel, ROC, PX, PY, adc) );
  } else {
    //printf("PX PY   %2i  %2i\n", PX, PY);
  }

  // can't use usual method since only does the first ROC
  if (PX >= ACTIVEREGION[0] && PX <= ACTIVEREGION[1] && PY >= ACTIVEREGION[2] && PY <= ACTIVEREGION[3]) {
    // badtracksarray[0] += 1;
  }
      
  return;
}


int PLTMC (int ACTIVEREGION[], bool save_and_write, std::ofstream&  f, std::ofstream& g, float size, float smallest, float (&goodarray)[30], float (&badarray)[30], int (&goodhitsplotted)[30], int (&badhitsplotted)[30], int (&goodtracksplotted)[30], int (&badtracksplotted)[30])
// 2nd function after main()
{
  // Open the output file
  std::ofstream fout("PLTMC.dat", std::ios::binary);
  if (!fout.is_open()) {
    std::cerr << "ERROR: cannot open output file" << std::endl;
    throw;
  }

  uint32_t  n;
  uint32_t  n2;

  PLTAlignment Alignment;
  //// Select which file to get data from and set as variable "Alignment"

  //Alignment.ReadAlignmentFile("ALIGNMENT/Alignment_IdealCastor.dat");
  //Alignment.ReadAlignmentFile("straight");
  //Alignment.ReadAlignmentFile("ALIGNMENT/Alignment_1Telescope.dat");
  //Alignment.ReadAlignmentFile("ALIGNMENT/Alignment_PLTMC.dat");
  //Alignment.ReadAlignmentFile("ALIGNMENT/Alignment_IdealInstall.dat");
  Alignment.ReadAlignmentFile("ALIGNMENT/Alignment_IdealInstall_editted.dat");

  // Vector of hits for each event
  std::vector<PLTHit*> Hits;
  //// ?Gets vectors from file chosen above? and calls them "Hits"
  //// default set at 10000
  int const NEvents = 100000;
  //int const NEvents = 10;

  //// keep track of tracks from interaction point vs. others
  int goodTracks = 0;
  int badTracks = 0;

  // 1st is 1 hit, 2nd is 2, 3rd is 3
  int goodhits[3] = {0, 0, 0};
  int badhits[3] = {0, 0, 0,};

  int totalgoodhits = 0;
  int totalbadhits = 0;

  // define active areas
//   int const ACTIVEFIRSTCOL1 = 49;
//   int const ACTIVELASTCOL1 = 49;
//   int const ACTIVEFIRSTROW1 = 75;
//   int const ACTIVELASTROW1 = 75;
//   int const ACTIVEFIRSTCOL2 = 4;
//   int const ACTIVELASTCOL2 = 49;
//   int const ACTIVEFIRSTROW2 = 5;
//   int const ACTIVELASTROW2 = 75;
//   int const ACTIVEFIRSTCOL3 = 4;
//   int const ACTIVELASTCOL3 = 49;
//   int const ACTIVEFIRSTROW3 = 5;
//   int const ACTIVELASTROW3 = 75;


  for (int ievent = 1; ievent <= NEvents; ++ievent) {
    //// Loop through all set number of events
    if (ievent % 10000 == 0) {
      //printf("ievent = %12i\n", ievent);
    }

    //weight good tracks at about 88% of all tracks
    int random_number = rand() % 100 + 1;
    int choice;
    if (random_number <= 88){choice = 0;}
    else {choice = 1+ rand() % 12;}

    int totalhits[3];
    totalhits[0] = 0;
    totalhits[1] = 0;
    totalhits[2] = 0;

    switch(choice) {
      //// default: switch was at 0 for only good tracks
      //// 13 options available, set currently for case 0
      //// case 0 are "good tracks" from collision point
      case 0:
        goodTracks += 1;
        GetTracksCollisions(Hits, Alignment, goodhits, ACTIVEREGION, totalhits, totalgoodhits);
        break;
      case 100:
	// needs fixing (SlopeY_Ch15)
	badTracks += 1;
        GetTracksHeadOnFirstROC(Hits, Alignment, badhits, ACTIVEREGION, totalhits, totalbadhits);
        break;
      case 2:
	// needs fixing - most problamatic  
	badTracks += 1;
        GetTracksHeadOn(Hits, Alignment, badhits, ACTIVEREGION, totalhits, totalbadhits);
        break;
      case 30:
	// needs fixing 
	badTracks += 1;
        GetTracksParallelGaus(Hits, Alignment, badhits, ACTIVEREGION, totalhits, totalbadhits);
        break;
      case 40:
	// needs fixing 
	badTracks += 1;
        GetSpecificClusters(Hits, Alignment, badhits, ACTIVEREGION, totalhits, totalbadhits);
        break;
      case 5:
	badTracks += 1;
        GetTracksROCEfficiencies(Hits, Alignment, 0.20, 0.80, 0.90, badhits, ACTIVEREGION, totalhits, totalbadhits);
        break;
      case 6:
	badTracks += 1;
        GetRandTracksROCEfficiencies(Hits, Alignment, 0.20, 0.80, 0.90, badhits, ACTIVEREGION, totalhits, totalbadhits);
        break;
      case 7:
	badTracks += 1;
        GetTracksHeadOnFirstROCMultiTracks(Hits, Alignment, badhits, ACTIVEREGION, totalhits, totalbadhits);
        break;
      case 8:
	badTracks += 1;
        GetTracksRandomSlope(Hits, Alignment, badhits, ACTIVEREGION, totalhits, totalbadhits);
        break;
      case 9:
	badTracks += 1;
        GetTracksParallel(Hits, Alignment, badhits, ACTIVEREGION, totalhits, totalbadhits);
        break;
      case 10:
	badTracks += 1;
        GetPureNoise(Hits, Alignment, badhits, ACTIVEREGION, totalhits, totalbadhits);
        break;
      case 11:
	badTracks += 1;
        GetTracksCollisions2(Hits, Alignment, badhits, ACTIVEREGION, totalhits, totalbadhits);
	break;
      case 12:
	badTracks += 1;
        GetGausHitsOneROC(Hits, Alignment, badhits, ACTIVEREGION, totalhits, totalbadhits);
	break;
    }
    //badTracks = NEvents - goodTracks;

    // Delete all hits and clear vector
    n2 = (5 << 8);
    n =  0x50000000;
    n |= ievent;
    fout.write( (char*) &n2, sizeof(uint32_t) );
    fout.write( (char*) &n, sizeof(uint32_t) );


    for (size_t ihit = 0; ihit != Hits.size(); ++ihit) {
      n = 0x00000000;
      PLTHit* Hit = Hits[ihit];
      //printf("Channel ROC Row Col ADC: %2i %1i %2i %2i %4i %12i\n", Hit->Channel(), Hit->ROC(), Hit->Row(), Hit->Column(), Hit->ADC(), ievent);


      n |= (Hit->Channel() << 26);
      n |= ( (Hit->ROC() + 1) << 21);

      if (Hit->Column() % 2 == 0) {
        n |= ( ((80 - Hit->Row()) * 2) << 8 );
      } else {
        // checked, correct
        n |= ( ((80 - Hit->Row()) * 2 + 1) << 8 );
      }

      if (Hit->Column() % 2 == 0) {
        n |= ( ((Hit->Column()) / 2) << 16  );
      } else {
        n |= (( (Hit->Column() - 1) / 2) << 16  );
      }
      n |= Hit->ADC();


      //if (Hit->ROC() == 2) {
      //  printf("WORD: %X\n", (n &  0x3e00000));
      //}

      fout.write( (char*) &n, sizeof(uint32_t) );
      delete Hits[ihit];
    }

    if (Hits.size() % 2 == 0) {
      // even number of hits.. need to fill out the word.. just print the number over two as 2x32  words
      n  = 0xa0000000;
      n2 = 0x00000000;
      n  |= (Hits.size() / 2 + 2);
      fout.write( (char*) &n2, sizeof(uint32_t) );
      fout.write( (char*) &n, sizeof(uint32_t) );
    } else {
      // Print number of hits in 1x32 bit
      n  = 0x00000000;
      fout.write( (char*) &n, sizeof(uint32_t) );
      fout.write( (char*) &n, sizeof(uint32_t) );
      n  = 0xa0000000;
      n  |= (Hits.size() / 2 + 1);
      fout.write( (char*) &n, sizeof(uint32_t) );
    }


    Hits.clear();
  }
  //std::cout<<totalbadhits<<std::endl;
  // print out good tracks vs bad tracks and others
  if (save_and_write) {
    goodarray[int(size*2.0)-int(smallest)] = float(goodhits[2])/float(totalgoodhits);//goodTracks);//goodhits[2];//int(goodTracks);//float(goodhits[2])/float(goodTracks);
    badarray[int(size*2.0)-int(smallest)] = float(badhits[2])/float(totalbadhits);//badhits[2];//int(badTracks);//float(badhits[2])/float(badTracks);
    //std::cout<<float(badhits[2])/float(totalbadhits[0])<<std::endl;
    goodhitsplotted[int(size*2.0)-int(smallest)] = goodhits[2];
    badhitsplotted[int(size*2.0)-int(smallest)] = badhits[2];
    goodtracksplotted[int(size*2.0)-int(smallest)] = goodTracks;
    badtracksplotted[int(size*2.0)-int(smallest)] = badTracks;
    
    
  }
  
  // just prints to screen good vs. bad tracks
  //  else {
  //std::cout<<"Good Tracks:  "<<totalgoodhits<<std::endl;  
    //std::cout<<"Good Tracks Hits:  "<<std::endl;
    //std::cout<<"3 Hits:       "<<goodhits[2]<<std::endl;
    //    std::cout<<"2 Hits:       "<<goodhits[1]<<std::endl;
    //    std::cout<<"1 Hit:        "<<goodhits[0]<<std::endl;  
    //std::cout<<"Bad Tracks:   "<< totalbadhits<<std::endl;
    //std::cout<<"Bad Tracks Hits:   "<<std::endl;
    //std::cout<<"3 Hits:       "<<badhits[2]<<std::endl;
    //    std::cout<<"2 Hits:       "<<badhits[1]<<std::endl;
    //    std::cout<<"1 Hit:        "<<badhits[0]<<std::endl;
    //std::cout<<"Percent Good: "<<100.0 * float(goodTracks)/float(NEvents)<<"%"<<std::endl;
    //std::cout<<"Ratio 3 Hit Bad/Total Bad:   "<<float(badhits[2])/float(totalbadhits)<<std::endl;
    // if (float(badhits[2])/float(totalbadhits) > 1.0){
    //std::cout<<"Ratio 3 Hit Good/Total Good: "<<float(goodhits[2])/float(totalgoodhits)<<std::endl;
  //  for (int j=0; j<12; ++j){
  //      std::cout<<ACTIVEREGION[j]<<std::endl;
  //  }
  //}

     //   }
  fout.close();

  return 0;
}


int main (int argc, char* argv[])
{
  if (argc != 1) {
    // prints what commands to use if not done corretly when executed
    std::cerr << "Usage: " << argv[0] << " " << std::endl;
    return 1;
  }

  // if true, plots a range of sizes for active regions
  bool save_and_write = true;

  // types of geometries
  // 0 = all equal, 1 = cone: 1st largest, 2 = hourglass, 3 = cone: 1st smallest, 10 = test
  int shape = 0;

  // determine sizes of each ROC, numbers refer to length of square sizes in mm
  float sizes = 8.0;
  float size1;
  float size2;
  float size3;
  float smallest = 7.0;

  // files to be created to store values to be plotted in gnuplot
  std::ofstream f,g,h,k;

  // loop through all geometries
  for (int i=0; i<=shape; ++i){
    
    // determine which files to use based on geometries
    if (save_and_write) {
      switch(i) {
      case 0:
	f.open("table_equal_goodhits.txt");
	g.open("table_equal_badhits.txt");
	h.open("track_tables/equal_goodhits.txt");
	k.open("track_tables/equal_badhits.txt");
	//std::cout<<"f & g opened: EQUAL"<<std::endl;
	break;
      case 1:
	f.open("table_cone_goodhits.txt");
	g.open("table_cone_badhits.txt");
	h.open("track_tables/cone_goodhits.txt");
	k.open("track_tables/cone_badhits.txt");
	//std::cout<<"f & g opened: CONE"<<std::endl;
	break;
      case 2:
	f.open("table_hourglass_goodhits.txt");
	g.open("table_hourglass_badhits.txt");
	h.open("track_tables/hourglass_goodhits.txt");
	k.open("track_tables/hourglass_badhits.txt");
	//std::cout<<"f & g opened: HOURGLASS"<<std::endl;
	break;
      case 3:
	f.open("table_cone_inverted_goodhits.txt");
	g.open("table_cone_inverted_badhits.txt");
	h.open("track_tables/cone_inverted_goodhits.txt");
	k.open("track_tables/cone_inverted_badhits.txt");
	//std::cout<<"f & g opened: CONE INVERTED"<<std::endl;
	break;


	//Test case
      case 10:
	f.open("table_test_goodhits.txt");
	g.open("table_test_badhits.txt");
	h.open("track_tables/test_goodhits.txt");
	k.open("track_tables/test_badhits.txt");
      }
    }
    
    for (int si=int(smallest); si<=int(sizes)*2; ++si) {
      
      // must use integers in for loop command, so declare another variable to adjust sizes in half integer intervals
      float s = float(si)/2.0;
      
      // adjustments of ROCs up/down, side-to-side, numbers corresponds to rows or columns
      float up[3] = {0.0,0.0,0.0};//{0.0,-1.0,-0.5};//
      if (i == 3) {
// 	std::cout<<"INVERTED CONE"<<std::endl;
	up[0] = 0.0;//-0.3;//-.2*(7.0-s);
	up[1] = 0.0;//-.1*(7.0-s);
	up[2] = 0.0;
	//	std::cout<<up[0]<<std::endl<<up[1]<<std::endl<<up[2]<<std::endl;
      }
      //test case
      if (i == 10){
	up[0] = -0.3;
	up[1] = 0.0;
	up[2] = 0.0;
      }
      //else {
      //
      //}
      float right[3] = {0.0,0.0,0.0};
      

      // declare active region variables
      int ACTIVEFIRSTCOL1;
      int ACTIVELASTCOL1;
      int ACTIVEFIRSTROW1;
      int ACTIVELASTROW1;
      int ACTIVEFIRSTCOL2;
      int ACTIVELASTCOL2;
      int ACTIVEFIRSTROW2;
      int ACTIVELASTROW2;
      int ACTIVEFIRSTCOL3;
      int ACTIVELASTCOL3;
      int ACTIVEFIRSTROW3;
      int ACTIVELASTROW3;
      
      
      
      // set active regions to full (will change below)
      ACTIVEFIRSTCOL1 = 0;
      ACTIVELASTCOL1 = 51;
      ACTIVEFIRSTROW1 = 0;
      ACTIVELASTROW1 = 79;
      ACTIVEFIRSTCOL2 = 0;
      ACTIVELASTCOL2 = 51;
      ACTIVEFIRSTROW2 = 0;
      ACTIVELASTROW2 = 79;
      ACTIVEFIRSTCOL3 = 0;
      ACTIVELASTCOL3 = 51;
      ACTIVEFIRSTROW3 = 0;
      ACTIVELASTROW3 = 79;
      
      // determine sizes of ROCs' active regions according to geometry case
      switch(i) {
      case(0):
	size1 = s;
	size2 = s;
	size3 = s;
	break;
      case(1):
	size1 = s;
	size2 = s - 1.0;
	size3 = s - 2.0;
	break;
      case(2):
	size1 = s;
	size2 = s - 1.0;
	size3 = s;
	break;
      case(3):
	size1 = s - 1.0;
	size2 = s - 0.5;
	size3 = s;

	//test case
      case(10):
	size1 = s - 2.0;
	size2 = s;
	size3 =s;
	break;
      }
      
      // Adjust active regions according to shape, size, and bumps
      //      std::cout<<up[0]<<std::endl<<up[1]<<std::endl<<up[2]<<std::endl;
      float dcol1 = 51.0 * (float(size1) / 8.0);
      float drow1 = 79.0 * (float(size1) / 8.0);
      ACTIVEFIRSTCOL1 += int(((51.0 - dcol1) / 2.0) + (51.0 * (right[0] / 8.0)));
      if (ACTIVEFIRSTCOL1 < 0) {ACTIVEFIRSTCOL1 = 0;}
      ACTIVELASTCOL1 -= ACTIVEFIRSTCOL1 - int((51.0 * (right[0] / 8.0)));
      if (ACTIVELASTCOL1 > 51) {ACTIVELASTCOL1 = 51;}
      ACTIVEFIRSTROW1 += int(((79.0 - drow1) / 2.0) + (79.0 * (up[0] / 8.0)));
      if (ACTIVEFIRSTROW1 < 0) {ACTIVEFIRSTROW1 = 0;}
      ACTIVELASTROW1 -= ACTIVEFIRSTROW1 - int(79.0 * (up[0] / 8.0));
      if (ACTIVELASTROW1 > 79) {ACTIVELASTROW1 = 79;}
      
      float dcol2 = 51.0 * (float(size2) / 8.0);
      float drow2 = 79.0 * (float(size2) / 8.0);
      ACTIVEFIRSTCOL2 += int(((51.0 - dcol2) / 2.0) + (51.0 * (right[1] / 8.0)));
      if (ACTIVEFIRSTCOL2 < 0) {ACTIVEFIRSTCOL2 = 0;}
      ACTIVELASTCOL2 -= ACTIVEFIRSTCOL2 - int((51.0 * (right[1] / 8.0)));
      if (ACTIVELASTCOL2 > 51) {ACTIVELASTCOL2 = 51;}
      ACTIVEFIRSTROW2 += int(((79.0 - drow2) / 2.0) + (79.0 * (up[1] / 8.0)));
      if (ACTIVEFIRSTROW2 < 0) {ACTIVEFIRSTROW2 = 0;}
      ACTIVELASTROW2 -= ACTIVEFIRSTROW2 - int(79.0 * (up[1] / 8.0));
      if (ACTIVELASTROW2 > 79) {ACTIVELASTROW2 = 79;}
      
      float dcol3 = 51.0 * (float(size3) / 8.0);
      float drow3 = 79.0 * (float(size3) / 8.0);
      ACTIVEFIRSTCOL3 += int(((51.0 - dcol3) / 2.0) + (51.0 * (right[2] / 8.0)));
      if (ACTIVEFIRSTCOL3 < 0) {ACTIVEFIRSTCOL3 = 0;}
      ACTIVELASTCOL3 -= ACTIVEFIRSTCOL3 - int((51.0 * (right[2] / 8.0)));
      if (ACTIVELASTCOL3 > 51) {ACTIVELASTCOL3 = 51;}
      ACTIVEFIRSTROW3 += int(((79.0 - drow3) / 2.0) + (79.0 * (up[2] / 8.0)));
      if (ACTIVEFIRSTROW3 < 0) {ACTIVEFIRSTROW3 = 0;}
      ACTIVELASTROW3 -= ACTIVEFIRSTROW3 - int(79.0 * (up[2] / 8.0));
      if (ACTIVELASTROW3 > 79) {ACTIVELASTROW3 = 79;}
      
      //   for (int i = 1; i<=divide; ++i) {
      //     //sets active regions for the ROCs
      
      //     int ACTIVEFIRSTCOL1;
      //     int ACTIVELASTCOL1;
      //     int ACTIVEFIRSTROW1;
      //     int ACTIVELASTROW1;
      //     int ACTIVEFIRSTCOL2;
      //     int ACTIVELASTCOL2;
      //     int ACTIVEFIRSTROW2;
      //     int ACTIVELASTROW2;
      //     int ACTIVEFIRSTCOL3;
      //     int ACTIVELASTCOL3;
      //     int ACTIVEFIRSTROW3;
      //     int ACTIVELASTROW3;
      
      //     // Max values: Columns: 51, Rows: 79
      
      //     // dividing normal second ROC
      //     ACTIVEFIRSTCOL1 = 0;
      //     ACTIVELASTCOL1 = 51;
      //     ACTIVEFIRSTROW1 = 0;
      //     ACTIVELASTROW1 = 79;
      //     ACTIVEFIRSTCOL2 = int((51.0 - (51.0/sqrt(i)))/2.0);
      //     ACTIVELASTCOL2 = 51 - int((51.0 - (51.0/sqrt(i)))/2.0);
      //     ACTIVEFIRSTROW2 = int((79.0 - (79.0/sqrt(i)))/2.0);
      //     ACTIVELASTROW2 = 79 - int((79.0 - (79.0/sqrt(i)))/2.0);
      //     ACTIVEFIRSTCOL3 = 0;
      //     ACTIVELASTCOL3 = 51;
      //     ACTIVEFIRSTROW3 = 0;
      //     ACTIVELASTROW3 = 79;
      
      //     // dividing by powers of 2 second ROC
      // //     int ACTIVEFIRSTCOL1 = 0;
      // //     int ACTIVELASTCOL1 = 51;
      // //     int ACTIVEFIRSTROW1 = 0;
      // //     int ACTIVELASTROW1 = 79;
      // //     int ACTIVEFIRSTCOL2 = int((51.0 - (51.0/pow(2.0,float(i))))/2.0);
      // //     int ACTIVELASTCOL2 = 51 - int((51.0 - (51.0/pow(2.0,float(i))))/2.0);
      // //     int ACTIVEFIRSTROW2 = int((79.0 - (79.0/pow(2.0,float(i))))/2.0);
      // //     int ACTIVELASTROW2 = 79 - int((79.0 - (79.0/pow(2.0,float(i))))/2.0);
      // //     int ACTIVEFIRSTCOL3 = 0;
      // //     int ACTIVELASTCOL3 = 51;
      // //     int ACTIVEFIRSTROW3 = 0;
      // //     int ACTIVELASTROW3 = 79;
      
      //     // dividing normally first roc
      // //     int ACTIVEFIRSTCOL1 = int((51.0 - (51.0/sqrt(i+3)))/2.0);
      // //     int ACTIVELASTCOL1 = 51 - int((51.0 - (51.0/sqrt(i+3)))/2.0);
      // //     int ACTIVEFIRSTROW1 = int((79.0 - (79.0/sqrt(i+3)))/2.0);
      // //     int ACTIVELASTROW1 = 79 - int((79.0 - (79.0/sqrt(i+3)))/2.0);
      // //     int ACTIVEFIRSTCOL2 =0;
      // //     int ACTIVELASTCOL2 = 51;
      // //     int ACTIVEFIRSTROW2 = 0;
      // //     int ACTIVELASTROW2 = 79;
      // //     int ACTIVEFIRSTCOL3 = 0;
      // //     int ACTIVELASTCOL3 = 51;
      // //     int ACTIVEFIRSTROW3 = 0;
      // //     int ACTIVELASTROW3 = 79;
      
      //     // divide 1st ROC powers of 2
      // //     int ACTIVEFIRSTCOL1 = int((51.0 - (51.0/pow(2.0,float(i))))/2.0);
      // //     int ACTIVELASTCOL1 = 51 - int((51.0 - (51.0/pow(2.0,float(i))))/2.0);
      // //     int ACTIVEFIRSTROW1 = int((79.0 - (79.0/pow(2.0,float(i))))/2.0);
      // //     int ACTIVELASTROW1 = 79 - int((79.0 - (79.0/pow(2.0,float(i))))/2.0);
      // //     int ACTIVEFIRSTCOL2 =0;
      // //     int ACTIVELASTCOL2 = 51;
      // //     int ACTIVEFIRSTROW2 = 0;
      // //     int ACTIVELASTROW2 = 79;
      // //     int ACTIVEFIRSTCOL3 = 0;
      // //     int ACTIVELASTCOL3 = 51;
      // //     int ACTIVEFIRSTROW3 = 0;
      // //     int ACTIVELASTROW3 = 79;

      //     // dividing normally third ROC
      // //     int ACTIVEFIRSTCOL1 = 0;
      // //     int ACTIVELASTCOL1 = 51;
      // //     int ACTIVEFIRSTROW1 = 0;
      // //     int ACTIVELASTROW1 = 79;
      // //     int ACTIVEFIRSTCOL2 =0;
      // //     int ACTIVELASTCOL2 = 51;
      // //     int ACTIVEFIRSTROW2 = 0;
      // //     int ACTIVELASTROW2 = 79;
      // //     int ACTIVEFIRSTCOL3 = int((51.0 - (51.0/sqrt(i+3)))/2.0);
      // //     int ACTIVELASTCOL3 = 51 - int((51.0 - (51.0/sqrt(i+3)))/2.0);
      // //     int ACTIVEFIRSTROW3 = int((79.0 - (79.0/sqrt(i+3)))/2.0);
      // //     int ACTIVELASTROW3 = 79 - int((79.0 - (79.0/sqrt(i+3)))/2.0);

      //     // dividing third ROC powers of 2
      // //     int ACTIVEFIRSTCOL1 = 0;
      // //     int ACTIVELASTCOL1 = 51;
      // //     int ACTIVEFIRSTROW1 = 0;
      // //     int ACTIVELASTROW1 = 79;
      // //     int ACTIVEFIRSTCOL2 =0;
      // //     int ACTIVELASTCOL2 = 51;
      // //     int ACTIVEFIRSTROW2 = 0;
      // //     int ACTIVELASTROW2 = 79;
      // //     int ACTIVEFIRSTCOL3 = int((51.0 - (51.0/pow(2.0,float(i))))/2.0);
      // //     int ACTIVELASTCOL3 = 51 - int((51.0 - (51.0/pow(2.0,float(i))))/2.0);
      // //     int ACTIVEFIRSTROW3 = int((79.0 - (79.0/pow(2.0,float(i))))/2.0);
      // //     int ACTIVELASTROW3 = 79 - int((79.0 - (79.0/pow(2.0,float(i))))/2.0);

      int ACTIVEREGION[12] = 
	{
	  ACTIVEFIRSTCOL1,
	  ACTIVELASTCOL1,
	  ACTIVEFIRSTROW1,
	  ACTIVELASTROW1,
	  ACTIVEFIRSTCOL2,
	  ACTIVELASTCOL2,
	  ACTIVEFIRSTROW2,
	  ACTIVELASTROW2,
	  ACTIVEFIRSTCOL3,
	  ACTIVELASTCOL3,
	  ACTIVEFIRSTROW3,
	  ACTIVELASTROW3
	};

      float goodarray[30];
      float badarray[30];
      int goodhitsplotted[30];
      int badhitsplotted[30];
      int goodtracksplotted[30];
      int badtracksplotted[30];
      
      PLTMC(ACTIVEREGION,save_and_write, f, g, s, smallest, goodarray, badarray, goodhitsplotted, badhitsplotted, goodtracksplotted, badtracksplotted);
      
      
      if (save_and_write) {
	// writes lines of gnuplot file that contains data points to be plotted
	//for (int i = 4; i<=s; ++i){
	f<<s<<"   ";
	g<<s<<"   ";
	h<<s<<"   ";
	k<<s<<"   ";
	
	f<<goodarray[si-int(smallest)]<<""<<std::endl;
	//std::cout<<badarray[si-8]<<std::endl;
	g<<badarray[si-int(smallest)]<<""<<std::endl;
	h<<goodhitsplotted[si-int(smallest)]<<""<<std::endl;
	k<<badhitsplotted[si-int(smallest)]<<""<<std::endl;	
	
      }
    }
    if (save_and_write) {
      f.close();
      g.close();
      h.close();
      k.close();
    }
  }
  return 0;
}
