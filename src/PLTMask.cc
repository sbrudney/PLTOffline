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


//// Not sure if a write file is necessary

// void PLTMask::WriteMaskFile (std::string const OutFileName)
// {
//   //Open output file
//   FILE* Out = fopen(OutFileName.c_str(), "w");
//   if (!Out) {
//     std::cerr << "ERROR: cannot open file: " << OutFileName << std::endl;
//     throw;
//   }

//   fprintf(Out, "#first line:  MaskType \n");
//   fprintf(Out, "#ColStart, ColEnd, RowStart, RowEnd \n");
//   for (std::map<int, MaskAlignmentStruct>::iterator it = fMaskMap.begin(); it != fMaskMap.end(); ++it) {
//     int const MaskType = it->first;
//     MaskAlignmentStruct& Mask = it->second;
//     fprintf(Out, "\n");
//     fprintf(Out, "%2i\n", MaskType);


//     for (int iroc = 0; iroc !=3; ++iroc) {
//       std::pair<int, int> Nameiroc = std::make_pair(MaskType, iroc);

// //       if (!fMaskMap.count(Nameiroc)) {
// // 	std::cerr << "ERROR: No entry in fMaskMap for Name iroc: " << MaskType << " " << iroc << std::endl;
// // 	continue;
// //       }

      
//       fprintf(Out, "%2i   %1i     %15E                       %15E    %15E    %15E\n", MaskType, iroc, Mask.GColStart, Mask.GColEnd, Mask.GRowStart, Mask.GRowEnd);

//     }
//   }
//   fprintf(Out, "\n");

//   fclose(Out);

//   return;
// }

bool PLTMask::IsGood ()
{
  return fIsGood;
}

