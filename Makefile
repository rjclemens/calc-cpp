SRCS=calc.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
FLGS=-std=c++17

all: calc clean

calc: 
	gcc $(SRCS) $(FLGS) -o $(OBJS)
	./calc.o

clean:
	rm -f $(OBJS)