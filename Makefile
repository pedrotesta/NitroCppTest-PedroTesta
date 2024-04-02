CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
INC_DIR = json/include

all: rectangles

rectangles: rectangles.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< -I$(INC_DIR)

clean:
	rm -f rectangles