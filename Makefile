CXX = g++
CXXFLAGS = -std=c++11

3d: 3d.cpp
	$(CXX) 3d.cpp $(CXXFLAGS) -o 3d -lsfml-graphics -lsfml-window -lsfml-system
