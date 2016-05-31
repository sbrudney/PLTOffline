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
  bool withinBorder(std::string const, int, int, int);

  // Mini Structure only used in reading Mask file
  struct MaskAlignmentStruct {
    float GColStart, GColEnd, GRowStart, GRowEnd;
  };
  
  //std::map<std::string const, std::map<pair(int, int)> fMaskMap;
  //std::
  std::map<std::string const, std::map< std::pair<std::string const,int>, MaskAlignmentStruct > > fMaskMap;


 private:
 
  bool fIsGood;



};
 

#endif
