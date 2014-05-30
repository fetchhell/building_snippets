#!/bin/bash

export LD_LIBRARY_PATH = /opt/PostgreSQL/9.3/lib

CXX = g++
VARNAME = value
INCPATH = /opt/PostgreSQL/9.3/include
LIBPATH = /opt/PostgreSQL/9.3/lib
LIBS    = -L$(LIBPATH)
CXXFLAGS = -Wall -O2 -lm

main: main.o json_parser.o snippet_builder.o
	$(CXX) -I$(INCPATH) $(LIBS) $(CXXFLAGS) -o main main.o json_parser.o snippet_builder.o -lpq -lpgport

main.o: main.cpp json_parser.h snippet_builder.h
	$(CXX) -I$(INCPATH) $(LIBS) $(CXXFLAGS) -o main.o -c main.cpp

json_parser.o: json_parser.h
	$(CXX) $(CXXFLAGS) -o json_parser.o -c json_parser.cpp 

snippet_builder.o: snippet_builder.h
	$(CXX) $(CXXFLAGS) -o snippet_builder.o -c snippet_builder.cpp 

clean:
	rm -rf *.o
