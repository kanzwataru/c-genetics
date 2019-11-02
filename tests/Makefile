.PHONY: all utils dumper dumped clean make_dumper_header
GENETICS := ../src/genetics.c ../src/parser.c -I../include
CFLAGS	 := -Og -g

all: dumped
	mkdir -p build
	gcc -o build/prog_a prog_a/prog.c $(GENETICS) $(CFLAGS)
	gcc -o build/prog_b prog_b/prog.c $(GENETICS) $(CFLAGS)

utils:
	mkdir -p build
	gcc -o build/genutil $(GENETICS) ../src/genutil.c $(CFLAGS)
	gcc -o build/mkdumper $(GENETICS) ../src/mkdumper.c $(CFLAGS)

build/prog_a_gen.bin: utils
	build/genutil parse prog_a/structs.h $@

build/prog_b_gen.bin: utils
	build/genutil parse prog_b/structs.h $@

dumper: build/prog_a_gen.bin build/prog_b_gen.bin
	build/mkdumper structs.h build/prog_a_gen.bin > build/prog_a_dumper.gen.h
	build/mkdumper structs.h build/prog_b_gen.bin > build/prog_b_dumper.gen.h
	
	cp build/prog_a_dumper.gen.h dumper.gen.h
	gcc -o build/prog_a_dumper dumper.c $(GENETICS) $(CFLAGS) -I prog_a
	rm dumper.gen.h
	
	cp build/prog_b_dumper.gen.h dumper.gen.h
	gcc -o build/prog_b_dumper dumper.c $(GENETICS) $(CFLAGS) -I prog_b
	rm dumper.gen.h

dumped: dumper
	cd build && ./prog_a_dumper prog_a_genetics.bin
	cd build && ./prog_b_dumper prog_b_genetics.bin	

clean:
	rm -Rf build
	rm -f *.bin

