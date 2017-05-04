CC=g++
CFLAGS=-g -Wall -c -std=c++11

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)
EXECUTABLES=$(SOURCES:%.cpp=%)

build: $(EXECUTABLES)

$(EXECUTABLES):%: %.o
	$(CC) $< -o $@

$(OBJECTS):%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

run-p1: permutari
	./permutari

run-p2: pokemoni
	./pokemoni

run-p3: patrula
	./patrula

clean:
	rm -f $(EXECUTABLES) $(OBJECTS)
