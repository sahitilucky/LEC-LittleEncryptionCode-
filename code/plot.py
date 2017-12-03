from mpl_toolkits.mplot3d import *
import matplotlib.pyplot as plt
from matplotlib import cm
import random as rd
import math


f=open("encrypt_times","r")
line=f.readline()

Y=[]
while line:
	Y=Y+[float(line)]	
	line=f.readline()
Y.reverse()
Y=Y[:(len(Y)-1)]
X=[]
for i in range(5,21):
	X=X+[2**i]
#X=X+[2**22]
print len(X)
print len(Y)
plt.xlabel('File size (Ranging from 2^5 to 2^20)')
plt.ylabel('Timetaken')
plt.hold(True)
plt.plot(X,Y,'bo')
plt.plot(X,Y,linewidth=1.0)
#plt.show()
plt.savefig("File size Vs Time")
