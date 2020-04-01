all:client clean

client:Client_main.o client.o unified.o
	g++ -g -o client Client_main.o client.o unified.o -std=c++11 -pthread
Client_main.o:Client_main.cc
	g++ -g -c Client_main.cc -std=c++11 -pthread
client.o:client.cc
	g++ -g -c client.cc -std=c++11 -pthread
unified.o:unified.cc unified.h
	g++ -g -c unified.cc -std=c++11 -pthread

clean:
	rm client.o
	rm Client_main.o
	rm unified.o