CC = g++
TARGET = id3_algo

SRCDIR = src
BUILDDIR = build
INC = -I include
CFLAGS = -std=c++11 -g -Wall

SOURCES = $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))

$(TARGET): $(OBJECTS)
		$(CC) $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
		mkdir -p $(BUILDDIR)
		$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -r $(BUILDDIR)
	rm $(TARGET)

run: $(TARGET)
	./$(TARGET)
