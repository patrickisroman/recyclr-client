
clean:
	rm -rf *.o

build: recyclr-client.o

recyclr-client.o: recyclr-client.cpp recyclr-client.h
	g++ -o recyclr recyclr-client.cpp

