CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Directory configuration
BUILD_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/jlox

# Source files - adding new files
SRCS = main.cpp Lox.cpp Scanner.cpp Token.cpp Parser.cpp AstPrinter.cpp Interpreter.cpp Environment.cpp
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# Make sure build directories exist
$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR))

# Main target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Pattern rule for object files
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean command
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/* $(BIN_DIR)/*
	find . -name "*.o" -type f -delete

# Run command
.PHONY: run
run: $(TARGET)
	$(TARGET)

# Test runner
.PHONY: test
test:
	$(MAKE) -C tools/testRunner
	tools/testRunner/test_runner

# Dependencies
$(BUILD_DIR)/main.o: main.cpp Lox.h
$(BUILD_DIR)/Lox.o: Lox.cpp Lox.h Scanner.h Token.h TokenType.h
$(BUILD_DIR)/Scanner.o: Scanner.cpp Scanner.h Token.h TokenType.h Lox.h
$(BUILD_DIR)/Token.o: Token.cpp Token.h TokenType.h Literal.h
$(BUILD_DIR)/Parser.o: Parser.cpp Parser.h Token.h TokenType.h Expr.h Lox.h
$(BUILD_DIR)/AstPrinter.o: AstPrinter.cpp AstPrinter.h Expr.h
$(BUILD_DIR)/Interpreter.o: Interpreter.cpp Interpreter.h Expr.h Value.h Lox.h
$(BUILD_DIR)/Environment.o: Environment.cpp Environment.h Token.h Value.h 