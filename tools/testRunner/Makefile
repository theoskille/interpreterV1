CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = test_runner
ROOT_DIR = ../..
BUILD_DIR = build

# Source files
SRC_FILES = TestRunner.cpp \
       $(ROOT_DIR)/Token.cpp \
       $(ROOT_DIR)/Scanner.cpp \
       $(ROOT_DIR)/Parser.cpp \
       $(ROOT_DIR)/AstPrinter.cpp \
       $(ROOT_DIR)/Lox.cpp

# Object files in build directory
OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SRC_FILES)))

# Create build directory if it doesn't exist
$(shell mkdir -p $(BUILD_DIR))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule for TestRunner.cpp
$(BUILD_DIR)/TestRunner.o: TestRunner.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for files from root directory
$(BUILD_DIR)/%.o: $(ROOT_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(BUILD_DIR)/*.o

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run 