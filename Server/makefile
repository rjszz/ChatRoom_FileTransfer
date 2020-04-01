all:server clean

server:Server_main.o server.o unified.o
	g++ -g -o server Server_main.o server.o unified.o  -pthread
Server_main.o:Server_main.cc
	g++ -g -c Server_main.cc  -pthread
server.o:server.cc
	g++ -g -c server.cc	 -pthread
unified.o:unified.cc unified.h
	g++ -g -c unified.cc  -pthread

clean:
	rm  server.o
	rm Server_main.o
	rm unified.o