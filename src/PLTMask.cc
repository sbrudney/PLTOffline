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
  if (!Infile.is_open()) {
    fIsGood = false;
    std::cerr<<"ERROR: cannot open mask constants filename: "<<InFileName<<std::endl;
    throw;
  }

  // Read each line in file
  int ColStart0, ColEnd0, RowStart0, RowEnd0, ColStart1, ColEnd1, RowStart1, RowEnd1, ColStart2, ColEnd2, RowStart2, RowEnd2, FringeStart, FringeEnd;
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

    Linestream >> ColStart0 >> ColEnd0 >>  RowStart0 >>  RowEnd0 >>  ColStart1 >> ColEnd1 >> RowStart1 >> RowEnd1 >> ColStart2 >> ColEnd2 >> RowStart2 >> RowEnd2 >> FringeStart >> FringeEnd;
    std::tuple<int, int, int, int, int, int, int, int, int, int, int, int, int, int> MaskNumbers = std::make_tuple(ColStart0, ColEnd0, RowStart0, RowEnd0, ColStart1, ColEnd1, RowStart1, RowEnd1, ColStart2, ColEnd2, RowStart2, RowEnd2, FringeStart, FringeEnd);

  }
   
  InFile.close();
  
  return;
  
}


void PLTMask::WriteMaskFile (std::string const OutFileName)
{
  //Open output file
  FILE* Out = fopen(OutFileNAme.c_str(), "w");
  if (!Out) {
    std::cerr << "ERROR: cannot open file: " << OutFileName << std::endl;
    throw;
  }

  fprintf(Out, "#ColStart0, ColEnd0, RowStart0, RowEnd0, ColStart1, ColEnd1, RowStart1, RowEnd1, ColStart2, ColEnd2, RowStart2, RowEnd2, FringeStart, FringeEnd \n");

