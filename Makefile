all:
	gcc test.c genetics.c -o genetics -g -Og
	gcc parser.c genetics.c -o parser -g -Og
	gcc make_dumper.c genetics.c -o make_dumper -g -Og
	gcc util.c genetics.c -o util -g -Og

dumper:
	gcc dumper.c genetics.c -o dumper -g -Og

clean:
	rm -f genetics
	rm -f parser
	rm -f make_dumper
	rm -f dumper
	rm -f util
	rm -f *.o
	rm -f *.gen.*
	rm -f *.dat

