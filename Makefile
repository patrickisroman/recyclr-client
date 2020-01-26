CXX     := g++
CC      := gcc
LD      := g++
CFLAGS  := -std=gnu++17 -pthread -g
LDFLAGS := $(CFLAGS)

RM      := rm

RECYCLR_SRCS  := $(wildcard *.cpp)
RECYCLR_OBJS  := $(RECYCLR_SRCS:%.cpp=%.o)
RECYCLR_EXEC  := recyclr
RECYCLR_FILES := $(RECYCLR_OBJS) $(RECYCLR_EXEC)

.PHONY: clean all

default: $(RECYCLR_EXEC)
all: $(RECYCLR_SRCS) $(RECYCLR_EXEC)

$(RECYCLR_EXEC): $(RECYCLR_OBJS)
	@echo "(L) $@"
	@$(LD) $(LDFLAGS) $(RECYCLR_OBJS) -o $@

%.o: %.cpp %.h
	@echo "(C) $@"
	@$(CXX) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -rf $(RECYCLR_FILES)
