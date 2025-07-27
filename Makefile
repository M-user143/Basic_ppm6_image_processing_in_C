CC = gcc
CFLAGS = -g -Wall
TARGET = ppm_processor
SRC = main.c ppm.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	cmd /C del $(TARGET).exe