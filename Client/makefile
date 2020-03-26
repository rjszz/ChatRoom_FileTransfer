all:server client clean

client:Client_main.o client.o
	g++ -g -o client Client_main.o client.o
Client_main.o:Client_main.cc
	g++ -g -c Client_main.cc
client.o:client.cc
	g++ -g -c client.cc

clean:
	rm client.o
	rm Client_main.o