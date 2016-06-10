#include "PLTMask.h"

#include <map>

PLTMask::PLTMask ()
{
  fIsGood = false;
}

PLTMask::~PLTMask ()
{
}

void PLTMask::ReadMaskFile (std::string const InFileName)
{
  fIsGood = true;

  //Open file
  std::ifstream InFile(InFileName.c_str());
  if (!InFile.is_open()) {
    fIsGood = false;
    std::cerr<<"ERROR: cannot open mask constants filename: "<<InFileName<<std::endl;
    throw;
  }

  // Read each line in file
  std::string MaskType;
  int Channel, ROC;
  for (std::string InLine; std::getline(InFile, InLine); ) {
    if (InLine.size() < 1) {
      continue;
    }
    if (InLine.at(0) == '#') {
      continue;
    }
    
    //std::cout << InLine << std::endl;
    std::istringstream LineStream;
    LineStream.str(InLine);

    LineStream >> MaskType >> Channel  >> ROC;

    std::cout<<"Mask type is: "<<MaskType<<std::endl;

    std::pair<int, int> CHROC = std::make_pair(Channel, ROC);

    int ColStart, ColEnd, RowStart, RowEnd;

    // Only ROC 0,1,2 will work
    if (ROC == 0 || ROC == 1 || ROC == 2) {
      LineStream >> ColStart 
		 >> ColEnd 
		 >> RowStart 
		 >> RowEnd;
      PLTMask::MaskAlignmentStruct MAS = { ColStart, ColEnd, RowStart, RowEnd };
      // save values in map
      fMaskMap[MaskType][CHROC]= MAS;
      std::cout<<MaskType<<" "<<Channel<<" "<<ROC<<" "<<ColStart<<std::endl;
    }
    else {
      std::cerr << "WARNING: Mask file contains things I do not recognize: " << InLine << std::endl;
    }
  }

  //Close input file
  InFile.close();
  
  return;
  
}



bool PLTMask::IsGood ()
{
  return fIsGood;
}

