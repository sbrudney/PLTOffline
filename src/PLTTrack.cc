#include "PLTTrack.h"
#include "PLTTelescope.h"


PLTTrack::PLTTrack ()
{
}



PLTTrack::~PLTTrack ()
{
}



void PLTTrack::AddCluster (PLTCluster* in)
{
  fClusters.push_back(in);
  return;
}



int PLTTrack::MakeTrack (PLTAlignment& Alignment)
{

  if (DEBUG)
    std::cout << "Entering PLTTrack::MakeTrack. fClusters.size()= " << fClusters.size() << std::endl;

  // Check we have enough clusters
  if (NClusters() < 2) {
    std::cerr << "WARNING in PLTTrack::MakeTrack: Not enough clusters to make a track" << std::endl;
    return 0;
  }

  // Points in telescope coords where line passes each plane
  float XT[nPlanes];
  float YT[nPlanes];
  float ZT[nPlanes];
  // Initialize the arrays
  for (int iROC=0; iROC < nPlanes; iROC++){
      XT[iROC] =1.;
      YT[iROC] =1.;
      ZT[iROC] =1.;
  }

  // Set default for residuals
  for (int i = 0; i != 6; ++i) {
    fLResidualX[i] = -999;
    fLResidualY[i] = -999;
  }

  float VX, VY, VZ;

  int const Channel = fClusters[0]->Channel();

  // For ==2 clusters: Just use the direct line connecting them as a track
  if (NClusters() == 2) {

    // Vector components
    VX = fClusters[1]->TX() - fClusters[0]->TX();
    VY = fClusters[1]->TY() - fClusters[0]->TY();
    VZ = fClusters[1]->TZ() - fClusters[0]->TZ();

    float const SlopeX = VX / VZ;
    float const SlopeY = VY / VZ;

    // Length
    float const Mod = sqrt(VX*VX + VY*VY + VZ*VZ);

    // Normalize vectors
    VX = VX / Mod;
    VY = VY / Mod;
    VZ = VZ / Mod;

    if (DEBUG) {
      printf("2P VXVYVZ %12.3f %12.3f %12.3f\n", VX, VY, VZ);
    }

    // Compute the points in telescope coords where line passes each plane
    for (int iPlane = 0; iPlane < nPlanes; ++iPlane) {

      PLTAlignment::CP* C = Alignment.GetCP(Channel, iPlane);

      XT[iPlane] = (C->LZ - fClusters[0]->TZ()) * SlopeX + fClusters[0]->TX();
      YT[iPlane] = (C->LZ - fClusters[0]->TZ()) * SlopeY + fClusters[0]->TY();
      ZT[iPlane] =  C->LZ;
    }
  }
  else if (NClusters() == 3) {

    float const SumX = fClusters[0]->TX() + fClusters[1]->TX() + fClusters[2]->TX();
    float const SumY = fClusters[0]->TY() + fClusters[1]->TY() + fClusters[2]->TY();
    float const SumZ = fClusters[0]->TZ() + fClusters[1]->TZ() + fClusters[2]->TZ();

    float const SumZ2 = fClusters[0]->TZ() * fClusters[0]->TZ()
                      + fClusters[1]->TZ() * fClusters[1]->TZ()
                      + fClusters[2]->TZ() * fClusters[2]->TZ();

    float const SumXZ = fClusters[0]->TX() * fClusters[0]->TZ()
                      + fClusters[1]->TX() * fClusters[1]->TZ()
                      + fClusters[2]->TX() * fClusters[2]->TZ();

    float const SumYZ = fClusters[0]->TY() * fClusters[0]->TZ()
                      + fClusters[1]->TY() * fClusters[1]->TZ()
                      + fClusters[2]->TY() * fClusters[2]->TZ();

    float const MySlopeX = (3 * SumXZ - SumX * SumZ) / (3 * SumZ2 - SumZ * SumZ);
    float const MySlopeY = (3 * SumYZ - SumY * SumZ) / (3 * SumZ2 - SumZ * SumZ);

    float const SlopeX = (fClusters[2]->TX() - fClusters[0]->TX()) / (fClusters[2]->TZ() - fClusters[0]->TZ());
    float const SlopeY = (fClusters[2]->TY() - fClusters[0]->TY()) / (fClusters[2]->TZ() - fClusters[0]->TZ());

    VX = SlopeX;
    VY = SlopeY;
    VZ = 1;

    // Length
    float const Mod = sqrt(VX*VX + VY*VY + VZ*VZ);

    // Normalize vectors
    VX = VX / Mod;
    VY = VY / Mod;
    VZ = VZ / Mod;

    if (DEBUG) {
      printf("3P VXVYVZ %12.3f %12.3f %12.3f %12.3f\n", VX, VY, VZ, Mod);
    }

    float const AvgX = (fClusters[0]->TX() + fClusters[1]->TX() + fClusters[2]->TX()) / 3.0;
    float const AvgY = (fClusters[0]->TY() + fClusters[1]->TY() + fClusters[2]->TY()) / 3.0;
    float const AvgZ = (fClusters[0]->TZ() + fClusters[1]->TZ() + fClusters[2]->TZ()) / 3.0;

    // Compute the points in telescope coords where line passes each plane
    for (int ip = 0; ip < nPlanes ; ++ip) {

      PLTAlignment::CP* C = Alignment.GetCP(Channel, ip);

      XT[ip] = (C->LZ - AvgZ) * SlopeX + AvgX;
      YT[ip] = (C->LZ - AvgZ) * SlopeY + AvgY;
      ZT[ip] = C->LZ;
    }
  }
  // >3 clusters
  else{

    // Use Tgraphs to fit the x/y-coordinates
    // Graph: 1st coord / 2nd coord:
    // gX: Z / X
    // gY: Z / Y
    TGraph gX( NClusters() );
    TGraph gY( NClusters() );

    // Fill the graph with telescope coordinates
    for (int iCl=0; iCl < NClusters(); iCl++){
      gX.SetPoint( iCl, fClusters[iCl]->TZ(), fClusters[iCl]->TX() );
      gY.SetPoint( iCl, fClusters[iCl]->TZ(), fClusters[iCl]->TY() );
    }

    TF1 funX("funX","[0]*x+[1]");
    TF1 funY("funY","[0]*x+[1]");

    gX.Fit( &funX );
    gY.Fit( &funY );

    VX = funX.GetParameter(0);
    VY = funY.GetParameter(0);
    VZ = 1;

    // Length
    float const Mod = sqrt(VX*VX + VY*VY + VZ*VZ);

    // Normalize vectors
    VX = VX / Mod;
    VY = VY / Mod;
    VZ = VZ / Mod;

    // Compute the points in telescope coords where line passes each plane
    for (int ip = 0; ip < nPlanes ; ++ip) {

      PLTAlignment::CP* C = Alignment.GetCP(Channel, ip);
      
        XT[ip] = (C->LZ ) * funX.GetParameter(0) + funX.GetParameter(1);
        YT[ip] = (C->LZ ) * funY.GetParameter(0) + funY.GetParameter(1);
        ZT[ip] = C->LZ;
      }

  }

  // Set the vector and origin of track in telescope
  fTVX = VX;
  fTVY = VY;
  fTVZ = VZ;
  fTOX = XT[0];
  fTOY = YT[0];
  fTOZ = ZT[0];

  // These "G" quantities are defined to be point on ROC0
  std::vector<float> GV;
  // Rotate vector only..
  Alignment.VTtoVGXYZ(GV, fTVX, fTVY, fTVZ, Channel, 0);
  fGVX = GV[0];
  fGVY = GV[1];
  fGVZ = GV[2];
  std::vector<float> GO;
  Alignment.TtoGXYZ(GO, fTOX, fTOY, fTOZ, Channel, 0);
  fGOX = GO[0];
  fGOY = GO[1];
  fGOZ = GO[2];

  // Comput this track passes through each X=0, Y=0, Z=0 planes
  fPlaner[0][0] = fGOX - fGOX / fGVX * fGVX;
  fPlaner[0][1] = fGOY - fGOX / fGVX * fGVY;
  fPlaner[0][2] = fGOZ - fGOX / fGVX * fGVZ;

  fPlaner[1][0] = fGOX - fGOY / fGVY * fGVX;
  fPlaner[1][1] = fGOY - fGOY / fGVY * fGVY;
  fPlaner[1][2] = fGOZ - fGOY / fGVY * fGVZ;

  fPlaner[2][0] = fGOX - fGOZ / fGVZ * fGVX;
  fPlaner[2][1] = fGOY - fGOZ / fGVZ * fGVY;
  fPlaner[2][2] = fGOZ - fGOZ / fGVZ * fGVZ;
  //for (int ii = 0; ii != 3; ++ii) {
  //  printf("%15E %15E %15E\n", fPlaner[ii][0], fPlaner[ii][1], fPlaner[ii][2]);
  //}
  //printf("TEST: %f %f %f %f\n", fTOX, fTOY, fTOZ, fGOZ);

  // Compute where the line passes in each planes coords
  float XL[3];
  float YL[3];

  for (size_t ic = 0; ic != NClusters(); ++ic) {
    PLTCluster* Cluster = fClusters[ic];

    //int const Channel = Cluster->SeedHit()->Channel();
    int const ROC     = Cluster->SeedHit()->ROC();

    XL[ROC] = Alignment.TtoLX(XT[ROC], YT[ROC], Channel, ROC);
    YL[ROC] = Alignment.TtoLY(XT[ROC], YT[ROC], Channel, ROC);

    fLResidualX[ROC] = XL[ROC] - Cluster->LX();
    fLResidualY[ROC] = YL[ROC] - Cluster->LY();

    if (DEBUG) {
      printf("TESTLT: TX TY  LX LY: %1i  %1i  %12.3f %12.3f  %12.3f %12.3f\n", (int) NClusters(), ROC, XT[ROC], YT[ROC], XL[ROC], YL[ROC]);
    }
  }

  if (NClusters() < 3) {
    fD2 = 0;
  } else {
    fD2 = 0;
    for (size_t i = 0; i != NClusters(); ++i) {
      fD2 += fLResidualX[i]*fLResidualX[i] + fLResidualY[i]*fLResidualY[i];
    }
  }

  if (DEBUG)
    std::cout << "Reached end of PLTTrack::MakeTrack" << std::endl;

  return 0;
}




std::pair<float, float> PLTTrack::LResiduals (PLTCluster& Cluster, PLTAlignment& Alignment)
{

  float TZ = Alignment.GetCP(Cluster.Channel(), Cluster.ROC())->LZ;
  float TX = fTVX * TZ + fTOX;
  float TY = fTVY * TZ + fTOY;
  std::pair<float, float> LPXY = Alignment.TtoLXY(TX, TY, Cluster.Channel(), Cluster.ROC());
  //printf("LResiduals Cluster.LX %12.3E   LPXY.first %12.3E\n", Cluster.LX(), LPXY.first);
  return std::make_pair( LPXY.first - Cluster.LX(), LPXY.second - Cluster.LY() );

}


bool PLTTrack::IsFiducial (PLTPlane* Plane, PLTAlignment& Alignment, PLTPlane::FiducialRegion FidRegion)
{
  return IsFiducial (Plane->Channel(), Plane->ROC(), Alignment, FidRegion);
}


bool PLTTrack::IsFiducial (int const Channel, int const ROC, PLTAlignment& Alignment, PLTPlane::FiducialRegion FidRegion)
{
  // Check if a track passes through the diamond on a en plane

  float const TZ = Alignment.GetTZ(Channel, ROC);
  // Get telescope coords at the location of the plane
  //float const TX = fTOX + fTVX * Plane->TZ();
  //float const TY = fTOY + fTVY * Plane->TZ();
  float const TX = fTOX + fTVX * TZ;
  float const TY = fTOY + fTVY * TZ;

  //printf("IsFiducial: PlaneTY fTOY TX TY TZ  %2i %12.3f %12.3f %12.3f\n", Plane->Cluster(0)->SeedHit()->Row(), Plane->Cluster(0)->SeedHit()->TY(), fTOY, TX, TY, Plane->TZ());

  // Convert telescope coords to local coords
  std::pair<float, float> LXY = Alignment.TtoLXY(TX, TY, Channel, ROC);

  // Convert local coords to Pixel numbers
  int const PX = Alignment.PXfromLX(LXY.first);
  int const PY = Alignment.PYfromLY(LXY.second);

  //printf("IsFiducial: ROC  LX LY  PX PY  %1i  %12.3f %12.3f  %2i %2i\n", ROC, LXY.first, LXY.second, PX, PY);

  // If either row or col are outside of the diamond return false
  return PLTPlane::IsFiducial(FidRegion, PX, PY);
  if (PX > PLTU::LASTCOL || PX < PLTU::FIRSTCOL || PY > PLTU::LASTROW || PY < PLTU::FIRSTROW) {
    return false;
  }

  // else we must be inside the diamond...
  return true;
}


size_t PLTTrack::NClusters ()
{
  return fClusters.size();
}

size_t PLTTrack::NHits ()
{
  size_t Sum = 0;
  for (size_t i = 0; i != fClusters.size(); ++i) {
    Sum += fClusters[i]->NHits();
  }

  return Sum;
}


PLTCluster* PLTTrack::Cluster (size_t const i)
{
  return fClusters[i];
}


float PLTTrack::LResidualX (size_t const i)
{
  return fLResidualX[i];
}

float PLTTrack::LResidualY (size_t const i)
{
  return fLResidualY[i];
}

float PLTTrack::TX (float const Z)
{
  return fTVX * (Z / fTVZ) + fTOX;
}

float PLTTrack::TY (float const Z)
{
  return fTVY * (Z / fTVZ) + fTOY;
}


std::pair<float, float> PLTTrack::GXYatGZ (float const GZ, PLTAlignment& Alignment)
{
  std::vector<float> T;
  Alignment.GtoTXYZ(T, GZ, 0, 0, fClusters[0]->Channel(), 0);

  std::vector<float> G;
  Alignment.TtoGXYZ(G, TX(T[2]), TY(T[2]), T[2], fClusters[0]->Channel(), 0);
  return std::make_pair<float, float>(G[0], G[1]);
}


float PLTTrack::D2 ()
{
  return fD2;
}
