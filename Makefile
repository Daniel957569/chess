C := g++
CFLAGS := -Wall -Wextra -std=c++17
LDFLAGS := -lSDL2 -lSDL2_image

SOURCES=$(shell find ./ -name "*.cpp")
OBJECTS := $(SOURCES:.cpp=.o)
EXECUTABLE := chess

all: $(EXECUTABLE) run

run: 
	./chess

$(EXECUTABLE): $(OBJECTS)
	$(C) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(C) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
