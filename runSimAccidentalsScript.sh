#!/bin/bash

helperfile=".tempfile"


#declare -a masks=("_20_30" "_22_33" "_24_36" "_26_39")
#declare -a fringes=("1_1" "2_3")

declare -a masks=("_28_40")
declare -a fringes=("4_5")





for fringe in "${fringes[@]}"
do
  for mask in "${masks[@]}"
    do
    touch $helperfile
    echo "Mask is " "$mask"
    for ii in {1..40}
#    for ii in {1..2}
      do
      echo "Running PU " ${ii}
    
      #  ./MeasureAccidentals SimPuData/simhitPU${ii}.txt GainCal/fake2.dat ALIGNMENT/Alignment_IdealInstall.dat TrackDistributions/TrackDistributions_MagnetOn.txt
      ./MeasureAccidentals SimPuData/MASKEDsimhitPU${ii}${mask}_fringe${fringe}.txt GainCal/fake2.dat ALIGNMENT/Alignment_IdealInstall.dat TrackDistributions/TrackDistributions_MagnetOn.txt
 # ./countTFC.py ${ii}
      file=SimPuData/MASKEDAccidentalRatesPU${ii}${mask}_fringe${fringe}.txt
      cp AccidentalRates.txt $file
      output=`cat $file | head -2 | tail -1`
      echo ${ii} $output >> $helperfile
    done
    
    root -q -b "plottable.cpp(\"$helperfile\",\"${mask}\",\"${fringe}\")"

    cp $helperfile AccRateSim${mask}_fringe${fringe}.txt
    cp AccRateSim.png AccRateSim${mask}_fringe${fringe}.png
    rm $helperfile

  done
done