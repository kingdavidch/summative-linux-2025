CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGET = factory_simulation
SOURCES = main.c assembly_line.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean
