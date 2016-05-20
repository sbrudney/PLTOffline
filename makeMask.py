#!/usr/bin/python

### functions

def countThreeFoldCoin(container):
    ThreeFoldCoin = 0
    channels = {1:[],2:[],4:[],5:[],7:[],8:[],10:[],11:[],13:[],14:[],16:[],17:[],19:[],20:[],22:[],23:[]}
    for event in container:
        channels[int(event[0])].append(int(event[1]))
#    print channels

    for key in channels:
        if len(set(channels[key])) == 3:
           ThreeFoldCoin += 1 

    return ThreeFoldCoin

### actuall script


####

masks = []
#masks.append([[16,36],[25,55]])
#masks.append([[15,37],[23,56]])
#masks.append([[14,38],[21,57]])
#masks.append([[13,39],[19,58]])
masks.append([[12,40],[19,59]])
######

fringes = []
fringes.append([4,5])
#fringes.append([2,3])
#fringes.append([1,1])


##Apply the actuall mask
for fringe in fringes:
    for mask in masks:
        print("Mask : " + "col_limits =" + str(mask[0])+ "row_limits =" + str(mask[1]))

        fileIN = open("simhitplots_digioutput.txt", "r")
        line = fileIN.readline()
        maskedfile = open("SimPuData/MASKEDsimhitplots_digioutput" + str(mask[0][1]-mask[0][0]) + "_" + str(mask[1][1]-mask[1][0]) + "_fringe" + str(fringe[0]) + "_" +str(fringe[1]) + ".txt","w")
        while line:
            cells = [int(i) for i in line.strip().split(' ')]
            #condition for outer ROCs
            if(cells[1] == 0 or cells[1] == 2):
                if(cells[2] > mask[0][0]-fringe[0] and cells[2] <= mask[0][1]+fringe[0]):
                    if(cells[3] > mask[1][0]-fringe[1] and cells[3] <= mask[1][1]+fringe[1]):
                        maskedfile.write(' '.join([str(i) for i in cells]) + "\n")
            else:
                if(cells[2] > mask[0][0] and cells[2] <= mask[0][1]):
                    if(cells[3] > mask[1][0] and cells[3] <= mask[1][1]):
                        maskedfile.write(' '.join([str(i) for i in cells]) + "\n")
            line = fileIN.readline()
        maskedfile.close()
        fileIN.close()


##Creating the different pile up files

print("Now doing all the pileup files")


maxPU = 41
for fringe in fringes:
    for mask in masks: 
        print("Mask : " + "col_limits =" + str(mask[0])+ "row_limits =" + str(mask[1]))

        TFCfile = open("SimPuData/TFCbyPU" + "_" + str(mask[0][1]-mask[0][0]) + "_" + str(mask[1][1]-mask[1][0]) + "_fringe" + str(fringe[0]) + "_" +str(fringe[1]) + ".txt","w")
    
        for PU in range(1,maxPU):
            print("Pile Up : " + str(PU))
            fileIN = open("SimPuData/MASKEDsimhitplots_digioutput" + str(mask[0][1]-mask[0][0]) + "_" + str(mask[1][1]-mask[1][0]) + "_fringe" + str(fringe[0]) + "_" +str(fringe[1]) + ".txt", "r")
            line = fileIN.readline()
            PUfile = open("SimPuData/MASKEDsimhitPU" + str(PU) + "_" + str(mask[0][1]-mask[0][0]) + "_" + str(mask[1][1]-mask[1][0]) + "_fringe" + str(fringe[0]) + "_" +str(fringe[1]) + ".txt","w")
            while line:
                cells = line.strip().split(' ')
                cells[5] = str(int(cells[5])/PU)
                PUfile.write(' '.join(cells) + "\n")
                line = fileIN.readline()
            PUfile.close()
            fileIN.close()

        #now run over PU file again and get the number of coincidences
        
            PUfile = open("SimPuData/MASKEDsimhitPU" + str(PU) + "_" + str(mask[0][1]-mask[0][0]) + "_" + str(mask[1][1]-mask[1][0]) + "_fringe" + str(fringe[0]) + "_" +str(fringe[1]) + ".txt","r")
            line = PUfile.readline()
            eventnumber = int(line.strip().split(' ')[5])
            container = []
            
            ThreeFoldCoin = 0
            events = 0
            eventsWithTFC = 0
            eventsWithoutTFC = 0
        
            while line:
                cells = [int(i) for i in line.strip().split(' ')]
            
                if cells[5] != eventnumber:
                    #        print("new event")
                    #        print container
                
                    TFC = countThreeFoldCoin(container)
                    ThreeFoldCoin += TFC
                    events += (cells[5] - eventnumber)
                    if((cells[5] - eventnumber) > 1):
                        eventsWithoutTFC += cells[5] - eventnumber - 1
                    if(TFC == 0):
                        eventsWithoutTFC+= 1
                    else:
                        eventsWithTFC+=1
                            
                    eventnumber = cells[5]
                    container[:] = []
                    container.append(cells)
                else:
                    container.append(cells)
                    
                line=PUfile.readline()
            print(str(PU) + " " + str(events) + " " + str(ThreeFoldCoin) + " "+ str(eventsWithTFC) + " "+ str(eventsWithoutTFC))
            TFCfile.write(str(PU) + " " + str(events) + " " + str(ThreeFoldCoin) + " "+ str(eventsWithTFC) + " "+ str(eventsWithoutTFC) + "\n") 
        TFCfile.close()

## count fast ORs on each pile up file


