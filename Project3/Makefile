Pathfinder: main.o output.o Sampler.o
	g++ -o Pathfinder main.o output.o Sampler.o

main.o: main.cpp Sampler.h queue.h pathfinder.h disjointSet.h
	g++ -c main.cpp

output.o: output.cpp
	g++ -c output.cpp
