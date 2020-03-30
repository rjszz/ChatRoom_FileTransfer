all:server client clean

server:Server_main.o server.o
	g++ -g -o server Server_main.o server.o -std=c++11 -pthread
Server_main.o:Server_main.cc
	g++ -g -c Server_main.cc
server.o:server.cc
	g++ -g -c server.cc -std=c++11 -pthread
unified.o:unified.cc
	g++ -g -c unified.cc 

clean:
	rm  server.o
	rm Server_main.o
	rm unified.o