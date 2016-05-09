#!/usr/bin/python

channels = [1,2,4,5,7,8,10,11,13,14,16,17,19,20,22,23]
mfecchannel = [8,8,8,8,8,8,8,8,7,7,7,7,7,7,7,7]
halfshell = [1,1,1,1,2,2,2,2,1,1,1,1,2,2,2,2]
hubaddress = [5,13,21,29,5,13,21,29,5,13,21,29,5,13,21,29]

rocs =3
cols =52
rows =80


#p0 = '%e' % 6.373572E-02
#p1 = '%e' % -1.613902E+01
#p2 = '%e' % 1.081580E+03
#p3 = '%e' % 1.902884E+02
#p4 = '%e' % 4.751614E-01


p0 = '%e' % 9.519752E-02
p1 = '%e' % -2.732470E+01
p2 = '%e' % 2.027245E+03
p3 = '%e' % 1.892437E+02
p4 = '%e' % 6.179999E-01




s = " ";
#print the channel information
for index in range(len(channels)):
    print(" " + str(mfecchannel[index]) + " " + str(halfshell[index]) + " " + str(hubaddress[index]) + " " + str(channels[index]))
#    print(str(mfecchannel[index]) + "\t" + str(halfshell[index]) + "\t" + str(hubaddress[index]) + "\t" + str(channels[index]))
#    print(str(mfecchannel[index]) + " ")

#new line
print

#print gain cal values
# channel roc col row p0 p1 p2 p3

for channel in channels:
    for roc in range(rocs):
        for col in range(cols):
            for row in range(rows):
                print(" " + str(channel) + " " + str(roc) + " " + str(col) + " " + str(row) + " " + p0 + " "+ p1 + " "+ p2 + " "+ p3 + " "+ p4)
