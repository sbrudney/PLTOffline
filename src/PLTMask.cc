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
  int MaskType, ROC;
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

    LineStream >>  MaskType>> ROC;

    std::pair<int, int> NameROC = std::make_pair(MaskType, ROC);

    int ColStart, ColEnd, RowStart, RowEnd, FringeStart, FringeEnd;

    // Only ROC 0,1,2 will work
    if (ROC == 0 || ROC == 1 || ROC == 2) {
      LineStream >> ColStart 
		 >> ColEnd 
		 >> RowStart 
		 >> RowEnd 
		 >> FringeStart 
		 >> FringeEnd;
      fMaskMap[MaskType].GColStart = ColStart;
      fMaskMap[MaskType].GColEnd = ColEnd;
      fMaskMap[MaskType].GRowStart = RowStart;
      fMaskMap[MaskType].GRowEnd = RowEnd;
      fMaskMap[MaskType].GFringeStart = FringeStart;
      fMaskMap[MaskType].GFringeEnd = FringeEnd;
    }
    else {
      std::cerr << "WARNING: Mask file contains things I do not recognize: " << InLine << std::endl;
    }
  }

  //Close input file
  InFile.close();
  
  return;
  
}


void PLTMask::WriteMaskFile (std::string const OutFileName)
{
  //Open output file
  FILE* Out = fopen(OutFileName.c_str(), "w");
  if (!Out) {
    std::cerr << "ERROR: cannot open file: " << OutFileName << std::endl;
    throw;
  }

  fprintf(Out, "#first line:  MaskType \n");
  fprintf(Out, "#ColStart, ColEnd, RowStart, RowEnd, FringeStart, FringeEnd \n");
  for (std::map<int, MaskAlignmentStruct>::iterator it = fMaskMap.begin(); it != fMaskMap.end(); ++it) {
    int const MaskType = it->first;
    MaskAlignmentStruct& Mask = it->second;
    fprintf(Out, "\n");
    fprintf(Out, "%2i\n", MaskType);


    for (int iroc = 0; iroc !=3; ++iroc) {
      std::pair<int, int> Nameiroc = std::make_pair(MaskType, iroc);

//       if (!fMaskMap.count(Nameiroc)) {
// 	std::cerr << "ERROR: No entry in fMaskMap for Name iroc: " << MaskType << " " << iroc << std::endl;
// 	continue;
//       }

      
      fprintf(Out, "%2i   %1i     %15E                       %15E    %15E    %15E    %15E    %15E\n", MaskType, iroc, Mask.GColStart, Mask.GColEnd, Mask.GRowStart, Mask.GRowEnd, Mask.GFringeStart, Mask.GFringeEnd);

    }
  }
  fprintf(Out, "\n");

  fclose(Out);

  return;
}

bool PLTMask::IsGood ()
{
  return fIsGood;
}
