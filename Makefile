CXX=g++
CFLAGS=-std=c++17 -Wall -Werror

SOURCES=$(wildcard src/*.cpp)

.PHONY: clean

obj/%.o: src/%.cpp headers/%.hpp
	$(CXX) $(CFLAGS) -g -c $< -o $@

obj/main.o: src/main.cpp
	$(CXX) $(CFLAGS) -g -c $< -o $@

test: $(SOURCES:src/%.cpp=obj/%.o) headers/*.hpp
	$(CXX) $(CFLAGS) -g -o epubmaker $(wildcard obj/*.o) -lzippp_static -lzip

clean:
	rm epubmaker obj/*
