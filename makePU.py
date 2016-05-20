#!/usr/bin/python

maxPU = 41

for PU in range(1,maxPU):

    print("Pile Up : " + str(PU))
#    fileIN = open("simhitplots_digioutput.txt", "r")
    fileIN = open("MASKEDsimhitplots_digioutput.txt", "r")
    line = fileIN.readline()
#    PUfile = open("SimPuData/simhitPU" + str(PU) + ".txt","w")
    PUfile = open("SimPuData/MASKEDsimhitPU" + str(PU) + ".txt","w")
    while line:
        cells = line.strip().split(' ')
        cells[5] = str(int(cells[5])/PU)
        PUfile.write(' '.join(cells) + "\n")
        line = fileIN.readline()
    PUfile.close()
    fileIN.close()







#list_strings = []

#while line:
    
#    for PU in range(maxPU):

    #init     list_Strings.append( line_cells )
#        list_strings[PU] = list_strings[PU] + line_cells


#for PU in range:
#    file = open()
#    file.write( list_strings[PU])
#    file.close()
