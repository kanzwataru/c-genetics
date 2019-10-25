all:
	gcc genetics.c -o genetics -g -Og
	gcc parser.c -o parser -g -Og

clean:
	rm -f genetics
	rm -f parser
	rm -f *.o

