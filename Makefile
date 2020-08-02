CC=g++
CFLAGS=-std=c++20 -Wall -Wpedantic -Wextra

SOURCES=$(wildcard src/*.cpp)

.PHONY: clean uninstall

build: CFLAGS:=$(CFLAGS) -O3
build: $(SOURCES:src/%.cpp=obj/%.o) headers/*.hpp
	$(CC) $(CFLAGS) -o epubmaker $(wildcard obj/*.o) -lzippp_static -lzip

obj/%.o: src/%.cpp headers/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

obj/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

test: CFLAGS:=$(CFLAGS) -g -Werror
test: $(SOURCES:src/%.cpp=obj/%.o) headers/*.hpp
	$(CC) $(CFLAGS) -o epubmaker $(wildcard obj/*.o) -lzippp_static -lzip

clean:
	rm epubmaker obj/*

uninstall:
	rm /usr/local/bin/epubmaker

install: build
	$(CC) $(CFLAGS) -o /usr/local/bin/epubmaker $(wildcard obj/*.o) -lzippp_static -lzip
