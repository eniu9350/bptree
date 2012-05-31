test: clean
	gcc -g page.c bptree.c test.c -o test

clean:
	rm test -f
