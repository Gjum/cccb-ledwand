cccbDisplay: main.c ledwand.c
	@gcc --std=gnu99 -o $@ $^ -lz -lm

.PHONY: clean
clean:
	@@rm -f cccbDisplay main.o ledwand.o

