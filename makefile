default:
	gcc main.c -o build/bin -lncurses


run:
	gcc main.c -o build/bin -lncurses
	./build/bin
