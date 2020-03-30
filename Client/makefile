all:server client clean

client:Client_main.o client.o
	g++ -g -o client Client_main.o client.o unified.o
Client_main.o:Client_main.cc
	g++ -g -c Client_main.cc
client.o:client.cc
	g++ -g -c client.cc 
unified.o:unified.cc
	g++ -g -c unified.cc 

clean:
	rm client.o
	rm Client_main.o
	rm unified.o