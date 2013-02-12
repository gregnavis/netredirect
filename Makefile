CC?=gcc
OUTPUT=libnetredirect.so

BIN_PATH?=/usr/bin
LIB_PATH?=/usr/lib

.PHONY: clean test install
.IGNORE: clean

all: $(OUTPUT)

$(OUTPUT): libnetredirect.c
	$(CC) -shared -fPIC $^ -ldl -o $@

test:
	./test

install: all
	install -o root -g root -m 0755 $(OUTPUT) $(LIB_PATH)
	install -o root -g root -m 0755 netredirect $(BIN_PATH)

clean:
	rm $(OUTPUT)
