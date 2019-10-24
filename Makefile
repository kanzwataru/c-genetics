all:
	gcc *.c -o genetics -g -Og

clean:
	rm -f genetics
	rm -f *.o

