CC     := g++
CFLAGS := -std=gnu++17

RECYCLR_FILES := $(wildcard *.cpp)
RECYCLR_OBJS  := $(RECYCLR_FILES:%.cpp=%.o)

build: $(RECYCLR_OBJS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) $< -o $(basename $@)

clean:
	rm -rf *.o
