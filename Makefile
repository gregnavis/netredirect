CC?=gcc
CFLAGS?=-shared -fPIC -ldl
OUTPUT=libnetredirect.so

.PHONY: clean
.IGNORE: clean

$(OUTPUT): libnetredirect.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm $(OUTPUT)
