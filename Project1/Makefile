CFLAGS = -c -g
LFLAGS = -g

Project1: fraction.o point.o main.o
	g++ $(LFLAGS) -o Project1 main.o fraction.o point.o

main.o: main.cpp point.h fraction.h
	g++ $(CFLAGS) main.cpp

point.o: point.cpp point.h fraction.h
	g++ $(CFLAGS) point.cpp

fraction.o: fraction.cpp fraction.h
	g++ $(CFLAGS) fraction.cpp
