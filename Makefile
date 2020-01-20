CC := g++

RECYCLR_FILES := $(wildcard *.cpp)
RECYCLR_OBJS  := $(RECYCLR_FILES:%.cpp=%.o)

build: $(RECYCLR_OBJS)

%.o: %.cpp %.h
	$(CC) $< -o $(basename $@)

clean:
	rm -rf *.o
