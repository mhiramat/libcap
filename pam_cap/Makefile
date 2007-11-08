# simple make file for the pam_cap module

topdir=$(shell pwd)/..
include ../Make.Rules

all: pam_cap.so
	$(MAKE) testcompile

install:
	@echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
	@echo "X                                                X"
	@echo "X Need to figure out where to install pam_cap.so X"
	@echo "X                                                X"
	@echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

pam_cap.so: pam_cap.o
	$(LD) -o pam_cap.so $< $(LIBS)

pam_cap.o: pam_cap.c
	$(CC) $(CFLAGS) -c $< -o $@

testcompile: test.c pam_cap.o
	$(CC) $(CFLAGS) -o $@ $+ -lpam -ldl $(LIBS)

clean:
	rm -f *.o *.so testcompile *~
