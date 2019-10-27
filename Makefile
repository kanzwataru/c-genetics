all:
	gcc test.c genetics.c -o genetics -g -Og
	gcc parser.c genetics.c -o parser -g -Og

clean:
	rm -f genetics
	rm -f parser
	rm -f *.o
	rm -f *.dat
