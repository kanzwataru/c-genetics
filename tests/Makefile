GENETICS := ../src/genetics.c ../src/parser.c -I../include
CFLAGS	 := -Og -g

all:
	mkdir -p build
	gcc -o build/prog_a prog_a/prog.c $(GENETICS) $(CFLAGS)
	gcc -o build/prog_b prog_b/prog.c $(GENETICS) $(CFLAGS)

clean:
	rm -Rf build

