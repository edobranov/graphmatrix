# Makefile for GraphMatrix

CXX      = clang++
CXXFLAGS = -g -Wall -Wextra
LDFLAGS  = -g

GraphMatrix: main.cpp GraphMatrix.cpp GraphMatrix.h
	${CXX} ${CXXFLAGS} -o a.out main.cpp GraphMatrix.cpp

clean:
	rm -f a.out