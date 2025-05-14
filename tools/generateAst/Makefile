CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = generate_ast
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean 