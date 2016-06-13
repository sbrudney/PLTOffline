"""MaskFileMaker.py
Sean Brudney
13 June 2016
Makes files that say what the mask for PLT will be, to be read by MakeTracks as the last input
"""

import os
# Make sure directory exists to store mask files, if not makes it
directory = 'MaskFiles'
if not os.path.exists(directory):
    os.makedirs(directory)

def makeEqualFile(Channels, ROCs, ColStart, ColEnd, RowStart, RowEnd):
    """
    makes equal mask file

    Channels: int, number of channels
    ROCs: int, number of ROCs (3)
    ColStart: int, first active column, min:0
    ColEnd: int, last active column, max:51
    RowStart: int, first active row, min:0
    RowEnd: int, last active row, max:79

    """
    MaskType = 'Equal'
    f = open("MaskFiles/EqualMask.txt","w")
    for CH in range(0,Channels):
        for ROC in range(0,ROCs):
            variables = [MaskType,str(CH),str(ROC),str(ColStart),str(ColEnd),str(RowStart),str(RowEnd)]
            for variable in variables:
                f.write(variable+" ")
            f.write("\n")
    f.close()
    return

def makeHourglassFile(Channels, ROCs, ColStart, ColEnd, RowStart, RowEnd, difference):
    """
    makes hourglass mask file

    Channels: int, number of channels
    ROCs: int, number of ROCs (3)
    ColStart: int, first active column of larger ROCs, min:0
    ColEnd: int, last active column of larger ROCs, max:51
    RowStart: int, first active row of larger ROCs, min:0
    RowEnd: int, last active row of larger ROCs, max:79
    difference: float (mm), how much shorter side length of middle ROC should be, min:0.0, max:8.0

    """
    MaskType = 'Hourglass'
    f = open("MaskFiles/HourglassMask.txt","w")
    for CH in range(0,Channels):
        for ROC in range(0,ROCs):
            if (ROC==1):
                ColStart += int((0.5*difference/8.0)*51.0)
                ColEnd   -= int((0.5*difference/8.0)*51.0)
                RowStart += int((0.5*difference/8.0)*79.0)
                RowEnd   -= int((0.5*difference/8.0)*79.0)
            variables = [MaskType,str(CH),str(ROC),str(ColStart),str(ColEnd),str(RowStart),str(RowEnd)]
            for variable in variables:
                f.write(variable+" ")
            f.write("\n")
            if (ROC==1):
                ColStart -= int((0.5*difference/8.0)*51.0)
                ColEnd   += int((0.5*difference/8.0)*51.0)
                RowStart -= int((0.5*difference/8.0)*79.0)
                RowEnd   += int((0.5*difference/8.0)*79.0)
    f.close()
    return


def makeConeFile(Channels, ROCs, ColStart, ColEnd, RowStart, RowEnd, difference):
    """
    makes cone mask file (first ROC largest)

    Channels: int, number of channels
    ROCs: int, number of ROCs (3)
    ColStart: int, first active column of largest ROCs, min:0
    ColEnd: int, last active column of largest ROCs, max:51
    RowStart: int, first active row of largest ROCs, min:0
    RowEnd: int, last active row of largest ROCs, max:79
    difference: float (mm), how much shorter side length of middle ROC should be (last ROC will be twice as short), min:0.0, max:8.0 

    """
    MaskType = 'Cone'
    f = open("MaskFiles/ConeMask.txt","w")
    for CH in range(0,Channels):
        for ROC in range(0,ROCs):
            if (ROC==1):
                ColStart += int((0.5*difference/8.0)*51.0)
                ColEnd   -= int((0.5*difference/8.0)*51.0)
                RowStart += int((0.5*difference/8.0)*79.0)
                RowEnd   -= int((0.5*difference/8.0)*79.0)
            if (ROC==2):
                ColStart += int((0.5*2.0*difference/8.0)*51.0)
                ColEnd   -= int((0.5*2.0*difference/8.0)*51.0)
                RowStart += int((0.5*2.0*difference/8.0)*79.0)
                RowEnd   -= int((0.5*2.0*difference/8.0)*79.0)
            variables = [MaskType,str(CH),str(ROC),str(ColStart),str(ColEnd),str(RowStart),str(RowEnd)]
            for variable in variables:
                f.write(variable+" ")
            f.write("\n")
            if (ROC==1):
                ColStart -= int((0.5*difference/8.0)*51.0)
                ColEnd   += int((0.5*difference/8.0)*51.0)
                RowStart -= int((0.5*difference/8.0)*79.0)
                RowEnd   += int((0.5*difference/8.0)*79.0)
            if (ROC==2):
                ColStart -= int((0.5*2.0*difference/8.0)*51.0)
                ColEnd   += int((0.5*2.0*difference/8.0)*51.0)
                RowStart -= int((0.5*2.0*difference/8.0)*79.0)
                RowEnd   += int((0.5*2.0*difference/8.0)*79.0)
    f.close()
    return

def makeInvertedConeFile(Channels, ROCs, ColStart, ColEnd, RowStart, RowEnd, difference):
    """
    makes cone mask file (first ROC largest)

    Channels: int, number of channels
    ROCs: int, number of ROCs (3)
    ColStart: int, first active column of largest ROCs, min:0
    ColEnd: int, last active column of largest ROCs, max:51
    RowStart: int, first active row of largest ROCs, min:0
    RowEnd: int, last active row of largest ROCs, max:79
    difference: float (mm), how much shorter side length of middle ROC should be (first ROC will be twice as short), min:0.0, max:8.0 

    """
    MaskType = 'InvertedCone'
    f = open("MaskFiles/InvertedConeMask.txt","w")
    for CH in range(0,Channels):
        for ROC in range(0,ROCs):
            if (ROC==1):
                ColStart += int((0.5*difference/8.0)*51.0)
                ColEnd   -= int((0.5*difference/8.0)*51.0)
                RowStart += int((0.5*difference/8.0)*79.0)
                RowEnd   -= int((0.5*difference/8.0)*79.0)
            if (ROC==0):
                ColStart += int((0.5*2.0*difference/8.0)*51.0)
                ColEnd   -= int((0.5*2.0*difference/8.0)*51.0)
                RowStart += int((0.5*2.0*difference/8.0)*79.0)
                RowEnd   -= int((0.5*2.0*difference/8.0)*79.0)
            variables = [MaskType,str(CH),str(ROC),str(ColStart),str(ColEnd),str(RowStart),str(RowEnd)]
            for variable in variables:
                f.write(variable+" ")
            f.write("\n")
            if (ROC==1):
                ColStart -= int((0.5*difference/8.0)*51.0)
                ColEnd   += int((0.5*difference/8.0)*51.0)
                RowStart -= int((0.5*difference/8.0)*79.0)
                RowEnd   += int((0.5*difference/8.0)*79.0)
            if (ROC==0):
                ColStart -= int((0.5*2.0*difference/8.0)*51.0)
                ColEnd   += int((0.5*2.0*difference/8.0)*51.0)
                RowStart -= int((0.5*2.0*difference/8.0)*79.0)
                RowEnd   += int((0.5*2.0*difference/8.0)*79.0)
    f.close()
    return

def makeManyMaskFiles(MaskTypes=['Equal','Hourglass','Cone','InvertedCone'], Channels=30, ROCs=3, ColStart=0, ColEnd=0, RowStart=0, RowEnd=0, difference=1.0):
    """
    loops through MaskTypes to make several mask files to be used by MakeTracks

    MaskTypes: list of strings, names of the different types of masks

    Channels: int, number of channels
    ROCs: int, number of ROCs (3)
    ColStart: int, first active column of largest ROC, min:0
    ColEnd: int, last active column of largest ROC, max:51
    RowStart: int, first active row of largest ROC, min:0
    RowEnd: int, last active row of largest ROC, max:79
    difference: float (mm), how much shorter the next shortest ROC side length will be, min:0.0, max:8.0

    """
    for MaskType in MaskTypes:
        if (MaskType=='Equal'):
            makeEqualFile(Channels, ROCs, ColStart, ColEnd, RowStart, RowEnd)
        elif (MaskType=='Hourglass'):
            makeHourglassFile(Channels, ROCs, ColStart, ColEnd, RowStart, RowEnd, difference)
        elif (MaskType=='Cone'):
            makeConeFile(Channels, ROCs, ColStart, ColEnd, RowStart, RowEnd, difference)
        elif (MaskType=='InvertedCone'):
            makeInvertedConeFile(Channels, ROCs, ColStart, ColEnd, RowStart, RowEnd, difference)
        else:
            print("ERROR: I don't know how to make a "+MaskType+" file! Please write a function for it.")

    return


