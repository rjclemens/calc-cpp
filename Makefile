SRCS=$(wildcard *.cpp)
OBJS=calc.out
FLGS=-std=c++17
OPTS=-o2 -g
ARGS="max(23, 5+5*2*2)"

all: build clean

args: compile
	./$(OBJS) $(ARGS)
	
build: compile
	./$(OBJS)

clean:
	rm -f $(OBJS)

compile:
	g++ $(SRCS) $(OPTS) $(FLGS) -o $(OBJS)

run:
	./$(OBJS)
