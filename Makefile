sources = src/main.c src/linkedlist.c
headers = src/linkedlist.h

linked: $(sources) $(headers) bin
	$(CC) $(sources) -o bin/linked -O3

debug: $(sources) $(headers) bin
	$(CC) $(sources) -o bin/linked-debug -O0 -g

bin:
	mkdir bin
