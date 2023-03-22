import socket

#Server Side Prototype V0

# create a socket object
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# get local machine name
host = socket.gethostname()

port = 9999

# bind the socket to a public host, and a well-known port
serversocket.bind((host, port))

# become a server socket
serversocket.listen(5)

while True:
    # establish a connection
    clientsocket, addr = serversocket.accept()

    print("Got a connection from %s" % str(addr))

    # receive the data from the client
    data = clientsocket.recv(1024).decode()

    #Check data, here we want to check whatever data is being sent
    #If data is a boolean, we check for that
    if data == 'alert':
        print("ALERT: Something important happened!")

    # close the client socket
    clientsocket.close()
