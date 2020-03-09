all:server client clean

server:Server.o server.o
	g++ -g -o server Server.o server.o
client:Client.o client.o
	g++ -g -o client Client.o client.o
Server.o:Server.cc
	g++ -g -c Server.cc
Client.o:Client.cc
	g++ -g -c Client.cc
server.o:server.cc
	g++ -g -c server.cc
client.o:client.cc
	g++ -g -c client.cc

clean:
	rm  server.o
	rm Server.o
	rm client.o
	rm Client.o