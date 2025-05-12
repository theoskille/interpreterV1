CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Directory configuration
BUILD_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/jlox

# Source files
SRCS = main.cpp Lox.cpp Scanner.cpp Token.cpp
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# Make sure build directories exist
$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR))

# Main target
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Pattern rule for object files
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Dependencies
$(BUILD_DIR)/main.o: main.cpp Lox.h
$(BUILD_DIR)/Lox.o: Lox.cpp Lox.h
$(BUILD_DIR)/Scanner.o: Scanner.cpp Scanner.h Token.h TokenType.h
$(BUILD_DIR)/Token.o: Token.cpp Token.h TokenType.h 