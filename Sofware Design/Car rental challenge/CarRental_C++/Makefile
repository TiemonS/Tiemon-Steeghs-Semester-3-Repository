CXX=g++
CXXFLAGS=-Wall -Werror -Wextra -pedantic -ggdb -O0 -std=c++11 -Iproduct
SOURCES=$(wildcard src/*.cpp)
HEADERS=$(wildcard src/*.h)
TARGET=CarRental

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(TARGET)

clean:
	@rm -f $(TARGET)
