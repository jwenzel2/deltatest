import socket

#set target host and port
host = input("Target Host:")
port = input("Target Port:")

target_host = str(host)
target_port = int(port)

data = "GET / HTTP/1.1\r\nHost:" + target_host + "\r\n\r\n"
bdata = data.encode('utf8')

#Create a socket object
client = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

#Connect the client
client.connect((target_host,target_port))

#send some data
client.send(bdata)

#receive some data
response = client.recv(4096)
print(response.decode())
client.close()