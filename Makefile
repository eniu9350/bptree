CC=gcc
BINDIR=./bin
LIBDIR=./lib


test: clean
	gcc -o ${BINDIR}/test -g -I/usr/include -I/usr/local/include/gsl -I../include -L. -L/usr/local/lib page.c bptree.c restore.c test.c  -lm -lgsl -lgslcblas

lib: clean
	$(CC) -I/usr/include -I/usr/local/include/gsl -I../include -L. -L/usr/local/lib -c -fPIC page.c bptree.c restore.c
	gcc -shared -o $(LIBDIR)/libbptree.so.1 bptree.o page.o restore.o

install: lib
	sudo cp $(LIBDIR)/libbptree.so.1 /opt/lib
	sudo mkdir -p /usr/local/include/bptree
	sudo cp ./*.h /usr/local/include/bptree/
	sudo ln -sf /opt/lib/libbptree.so.1 /opt/lib/libbptree.so


clean:
	rm test *.o -f
