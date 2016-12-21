from array import *
import matplotlib.pyplot as plt

class xyPlotter:

    def __init__(self):
        self.FAxis = "+x"
        self.x=array('i',[0])
        self.y=array('i',[0])
        self.i=0
        
    def move(self):
        if (self.FAxis == "+x"):
            self.x.append(self.x[self.i]+2)
            self.y.append(self.y[self.i])
        elif (self.FAxis == "+y"):
            self.y.append(self.y[self.i]+2)
            self.x.append(self.x[self.i])
        elif (self.FAxis == "-x"):
            self.x.append(self.x[self.i]-2)
            self.y.append(self.y[self.i])
        else :                      # For "-y"
            self.y.append(self.y[self.i]-2)
            self.x.append(self.x[self.i])
            
        self.i+=1

    def updateDir(self, direction):
        self.direction = direction
        
        if (self.FAxis == "+x"):
            if(self.direction == 'l'):
                self.FAxis="+y";
            else:
                self.FAxis="-y";
                
        elif (self.FAxis == "+y"):
            if(self.direction == 'l'):
                self.FAxis="-x";
            else:
                self.FAxis="+x";

        elif (self.FAxis == "-x"):
            if(self.direction == 'l'):
                self.FAxis="-y";
            else:
                self.FAxis="+y";
   
        else:                       # For "-y"
            if(self.direction == 'l'):
                self.FAxis="+x";
            else:
                self.FAxis="-x";
   

    def xyPrint(self):
        for xyPlotter.j in range(self.i):
            print ("\n\t" + str(self.x[xyPlotter.j]) + "\t" + str(self.y[xyPlotter.j]))

    def genGraph(self):
        plt.plot(self.x,self.y,'.b-')
        plt.show();
