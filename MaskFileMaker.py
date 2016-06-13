"""MaskFileMaker.py
Sean Brudney
13 June 2016
Makes files that say what the mask for PLT
"""

import os

directory = 'MaskFiles'
if not os.path.exists(directory):
    os.makedirs(directory)

## def makeMaskFile(MaskType, Channels, ROCs, ColStart, ColEnd, RowStart, RowEnd):
##     """
##     creates and writes a file with proper syntax to be read by MakeTracks.cc as the Mask file
    
##     MaskType: string, name of type of mask, will be used in file name
##     Channels: int, pos, number of Channels
##     ROCs: int, pos, usually 3
##     ColStart: int, first active column
##     ColEnd: int, last active column (max 51)
##     RowStart: int, first active row
##     RowEnd: int, last active row (max 79)
    
##     """
##     f = open(MaskType+"Mask.txt","w")
##     for CH in range(0,Channels):
##         for ROC in range(0,ROCs):
##             variables = [MaskType,str(CH),str(ROC),str(ColStart),str(ColEnd),str(RowStart),str(RowEnd)]
##             for variable in variables:
##                 f.write(variable+" ")
##             f.write("\n")
##     f.close()

def makeEqualFile(Channels, ROCs, ColStart, ColEnd, RowStart, RowEnd):
    """
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
    """
    MaskType = 'Hourglass'
    f = open("MaskFiles/HourglassMask.txt","w")
    for CH in range(0,Channels):
        for ROC in range(0,ROCs):
            if (ROC==1):
                ColStart += int((difference/8.0)*51.0)
                ColEnd   -= int((difference/8.0)*51.0)
                RowStart += int((difference/8.0)*79.0)
                RowEnd   -= int((difference/8.0)*79.0)
            variables = [MaskType,str(CH),str(ROC),str(ColStart),str(ColEnd),str(RowStart),str(RowEnd)]
            for variable in variables:
                f.write(variable+" ")
            f.write("\n")
            if (ROC==1):
                ColStart -= int((difference/8.0)*51.0)
                ColEnd   += int((difference/8.0)*51.0)
                RowStart -= int((difference/8.0)*79.0)
                RowEnd   += int((difference/8.0)*79.0)
    f.close()
    return


def makeConeFile(Channels, ROCs, ColStart, ColEnd, RowStart, RowEnd, difference):
    """
    """
    MaskType = 'Cone'
    f = open("MaskFiles/ConeMask.txt","w")
    for CH in range(0,Channels):
        for ROC in range(0,ROCs):
            if (ROC==1):
                ColStart += int((difference/8.0)*51.0)
                ColEnd   -= int((difference/8.0)*51.0)
                RowStart += int((difference/8.0)*79.0)
                RowEnd   -= int((difference/8.0)*79.0)
            if (ROC==2):
                ColStart += int((2.0*difference/8.0)*51.0)
                ColEnd   -= int((2.0*difference/8.0)*51.0)
                RowStart += int((2.0*difference/8.0)*79.0)
                RowEnd   -= int((2.0*difference/8.0)*79.0)
            variables = [MaskType,str(CH),str(ROC),str(ColStart),str(ColEnd),str(RowStart),str(RowEnd)]
            for variable in variables:
                f.write(variable+" ")
            f.write("\n")
            if (ROC==1):
                ColStart -= int((difference/8.0)*51.0)
                ColEnd   += int((difference/8.0)*51.0)
                RowStart -= int((difference/8.0)*79.0)
                RowEnd   += int((difference/8.0)*79.0)
            if (ROC==2):
                ColStart -= int((2.0*difference/8.0)*51.0)
                ColEnd   += int((2.0*difference/8.0)*51.0)
                RowStart -= int((2.0*difference/8.0)*79.0)
                RowEnd   += int((2.0*difference/8.0)*79.0)
    f.close()
    return

def makeInvertedConeFile(Channels, ROCs, ColStart, ColEnd, RowStart, RowEnd, difference):
    """
    """
    MaskType = 'InvertedCone'
    f = open("MaskFiles/InvertedConeMask.txt","w")
    for CH in range(0,Channels):
        for ROC in range(0,ROCs):
            if (ROC==1):
                ColStart += int((difference/8.0)*51.0)
                ColEnd   -= int((difference/8.0)*51.0)
                RowStart += int((difference/8.0)*79.0)
                RowEnd   -= int((difference/8.0)*79.0)
            if (ROC==0):
                ColStart += int((2.0*difference/8.0)*51.0)
                ColEnd   -= int((2.0*difference/8.0)*51.0)
                RowStart += int((2.0*difference/8.0)*79.0)
                RowEnd   -= int((2.0*difference/8.0)*79.0)
            variables = [MaskType,str(CH),str(ROC),str(ColStart),str(ColEnd),str(RowStart),str(RowEnd)]
            for variable in variables:
                f.write(variable+" ")
            f.write("\n")
            if (ROC==1):
                ColStart -= int((difference/8.0)*51.0)
                ColEnd   += int((difference/8.0)*51.0)
                RowStart -= int((difference/8.0)*79.0)
                RowEnd   += int((difference/8.0)*79.0)
            if (ROC==0):
                ColStart -= int((2.0*difference/8.0)*51.0)
                ColEnd   += int((2.0*difference/8.0)*51.0)
                RowStart -= int((2.0*difference/8.0)*79.0)
                RowEnd   += int((2.0*difference/8.0)*79.0)
    f.close()
    return

def makeManyMaskFiles(MaskTypes=['Equal','Hourglass','Cone','InvertedCone'], Channels=30, ROCs=3, ColStart=12, ColEnd=38, RowStart=19, RowEnd=59, difference=1.0):
    """
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


