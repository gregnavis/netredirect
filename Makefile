CC?=gcc
CFLAGS?=-shared -fPIC -ldl
OUTPUT=libnetredirect.so

.PHONY: clean test
.IGNORE: clean

all: $(OUTPUT)

$(OUTPUT): libnetredirect.c
	$(CC) $(CFLAGS) -o $@ $^

test:
	./test

clean:
	rm $(OUTPUT)
