#!/usr/bin/python

from sys import argv

### functions

def countThreeFoldCoin(container):
    ThreeFoldCoin = []
    channels = {1:[],2:[],4:[],5:[],7:[],8:[],10:[],11:[],13:[],14:[],16:[],17:[],19:[],20:[],22:[],23:[]}
    for event in container:
        channels[int(event[0])].append(int(event[1]))
#    print channels

    for key in channels:
        if len(set(channels[key])) == 3:
           ThreeFoldCoin.append(key) 
    return ThreeFoldCoin

### actuall script

masks = []
masks.append([[16,36],[25,55]])
#masks.append([[15,37],[23,56]])
#masks.append([[14,38],[21,57]])
#masks.append([[13,39],[19,58]])
######

fringes = []
fringes.append([2,3])
#fringes.append([1,1])


maxPU = 41
for fringe in fringes:
    for mask in masks: 
        TFCfile = open("SimPuData/TFCperChannelbyPU" + "_" + str(mask[0][1]-mask[0][0]) + "_" + str(mask[1][1]-mask[1][0]) + "_fringe" + str(fringe[0]) + "_" +str(fringe[1]) + ".txt","w")
        for PU in range(1,maxPU):
            fileIN = open("SimPuData/MASKEDsimhitPU" + str(PU) + "_" + str(mask[0][1]-mask[0][0]) + "_" + str(mask[1][1]-mask[1][0]) + "_fringe" + str(fringe[0]) + "_" +str(fringe[1]) + ".txt","r")
            line = fileIN.readline()
            
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

                line=fileIN.readline()
            print(str(PU) + " " + str(events) + " " + str(ThreeFoldCoin) + " "+ str(eventsWithTFC) + " "+ str(eventsWithoutTFC))
            TFCfile.write(str(PU) + " " + str(events) + " " + str(ThreeFoldCoin) + " "+ str(eventsWithTFC) + " "+ str(eventsWithoutTFC) + "\n") 
        TFCfile.close()

