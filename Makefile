#CXX=clang++
CXX=g++

FLAGS = -g -std=c++14 

all:
	${CXX} ${FLAGS} *.cpp

clean:
	rm -f a.out *.o out.ppm
