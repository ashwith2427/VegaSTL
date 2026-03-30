CXX = clang++
CXXFLAGS = -std=c++20 -I./STL -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lfolly -lpthread -ldl
TARGET = program
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean