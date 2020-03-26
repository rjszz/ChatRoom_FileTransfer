all:server client clean

server:Server_main.o server.o
	g++ -g -o server Server_main.o server.o
Server_main.o:Server_main.cc
	g++ -g -c Server_main.cc
server.o:server.cc
	g++ -g -c server.cc

clean:
	rm  server.o
	rm Server_main.o