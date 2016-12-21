import socket
from plotter import xyPlotter
import thread

borderA=0;
prevIrVal="000";
bot=xyPlotter();

HOST = '192.168.1.38'
PORT = 5006


def nextMove(irVal):
    global borderA;
    global prevIrVal;
    global box;
    returnVal="..";
    if (irVal == "000"):    #when there is no obstacle
        if(borderA==1):     #when the bot comes to a bent
            borderA=0;
            if(prevIrVal=="001"):
                bot.updateDir('r');
                returnVal="r"; 
            else:
                bot.updateDir('l');
                returnVal="l";
        else:
            bot.move();
            returnVal="m";
    elif (irVal == "010"):
        bot.updateDir('l');
        returnVal="l"; #obstacle in the front
    elif (irVal == "001"):
        borderA=1;
        bot.move();
        returnVal="m"; #when bot move in a border
    elif (irVal == "100"):
        borderA=1;
        bot.move();
        returnVal="m"; # "   "   "   "
    elif (irVal == "011"):
        bot.updateDir('l');
        returnVal="l";    #when bot comes to a corner
    elif (irVal == "110"):
        bot.updateDir('r');
        returnVal="r";    # "   "   "   "
    else: i=0;
    prevIrVal=irVal;
    return returnVal;


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
preid=0;
id=0;
s.bind((HOST, PORT))
s.listen(10)
while 1:
    conn,addr = s.accept()
    print ('Connected by', addr)
    while conn:
        d=conn.recv(10)
        dat=d[5:8]
        id=d[1]
        print (id)
        if id!=preid:
            preid=id;
            print ("\nESP: " + dat)
            post=nextMove(dat)
            print ("\nServer: " + post)
            thread.start_new_thread(bot.genGraph,());
        conn.send(post)
    conn.close()
s.close();
