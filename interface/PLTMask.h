#ifndef GUARD_PLTMask_h
#define GUARD_PLTMask_h

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <cmath>

#include "PLTHit.h"
#include "PLTCluster.h"
#include "PLTU.h"

class PLTMask
{
 public:
  PLTMask ();
  ~PLTMask ();

  void ReadMaskFile (std::string const);
  void WriteMaskFile (std::string const);
  bool IsGood ();
  //bool withinBorder(std::string, int, int, int);

  // Mini Structure only used in reading Mask file
  struct MaskAlignmentStruct {
    int GColStart, GColEnd, GRowStart, GRowEnd;
  };
  
  //std::map<std::string const, std::map<pair(int, int)> fMaskMap;
  //std::

  // map <MaskType, map < pair< Channel, ROC >, MaskALignmentStruct> > fMaskMap;
  std::map<std::string, std::map< std::pair<int, int>, MaskAlignmentStruct > > fMaskMap;

  static std::string GetMaskType(std::string const);

 private:
 
  bool fIsGood;



};
 

#endif
