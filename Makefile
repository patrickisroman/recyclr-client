CC     := g++
CFLAGS := -std=gnu++17

RECYCLR_FILES := $(wildcard *.cpp)
RECYCLR_OBJS  := $(RECYCLR_FILES:%.cpp=%.o)

build: recyclr-client.o $(RECYCLR_OBJS)
	$(CC) $(CFLAGS) -o recyclr recyclr-client.o

%.o: %.cpp %.h
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -rf *.o
	rm recyclr
