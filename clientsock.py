import socket

# Client Side Prototype V0
# TODO: 
# Figure out how to send GPIO Pin Data through socks

# create a socket object
clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# get local machine name
host = socket.gethostname()
#Set correct port
port = 9999

# connection to hostname on the port.
clientsocket.connect((host, port))

# send some data to the server
# here is where we need to figure out what exactly to send
# for now placeholder
data = 'alert'
clientsocket.sendall(data.encode())

# here we receive data back from server, this can be deleted if we are
# not receiving data back, only sending
response = clientsocket.recv(1024).decode()

print("Received: %s" % response)

# close the socket
clientsocket.close()
