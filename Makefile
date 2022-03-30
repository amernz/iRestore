CXX=clang++

all:
	$(CXX) main.cpp -o iRestore -std=c++11 -lusb-1.0
clean:
	rm iRestore
