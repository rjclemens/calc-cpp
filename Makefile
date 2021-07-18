SRCS=calc.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
FLGS=-std=c++17
ARGS="max(23, 5+5*2*2)"
all: build clean
rall: run clean

build: 
	g++ $(SRCS) $(FLGS) -o $(OBJS)
	./calc.o 

clean:
	rm -f $(OBJS)

run:
	g++ $(SRCS) $(FLGS) -o $(OBJS)
	./calc.o $(ARGS)