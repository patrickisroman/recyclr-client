CC     := g++
CFLAGS := -std=gnu++17 -pthread

RECYCLR_FILES := $(wildcard *.cpp)
RECYCLR_OBJS  := $(RECYCLR_FILES:%.cpp=%.o)

build: recyclr-net.o recyclr-client.o
	$(CC) $(CFLAGS) -o recyclr recyclr-client.o

%.o: %.cpp %.h
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -rf *.o
	rm recyclr
