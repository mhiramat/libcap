all: bug

bug: bug.c ../../libcap Makefile
	make -C ../../libcap
	cc --static -o $@ $< -L../../libcap -lcap

clean:
	rm -f bug.o bug
