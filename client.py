import socket

HOST = '192.168.1.38'     # Symbolic name meaning all available interfaces
PORT = 5006           # Arbitrary non-privileged port
conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

conn.connect((HOST, PORT));

print ('Connected to the server')
while conn:    
    p=input("Bot packet id: ");
    conn.send(str(p));
    serData = conn.recv(10)
    print ("\nServer: " + serData)

print('Connection closed..')

conn.close()
