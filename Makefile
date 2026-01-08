linked: src/main.c src/linkedlist.c src/linkedlist.h
	@mkdir -p bin
	$(CC) src/main.c src/linkedlist.c -o bin/linked -O3

debug: src/main.c src/linkedlist.c src/linkedlist.h
	@mkdir -p bin
	$(CC) src/main.c src/linkedlist.c -o bin/linked-debug -O0 -g
