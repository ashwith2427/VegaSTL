
CXX = clang++

CXXFLAGS = -std=c++20 -O3 -DNDEBUG \
           -I./vega \
           -fno-rtti \
           -march=native \
           -flto \
           -fsanitize=address \

LDFLAGS = -flto -fsanitize=address
TARGET = program
BUILD_DIR = build

SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
