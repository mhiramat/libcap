# simple make file for the pam_cap module

pam_cap.so: pam_cap.o
	ld -x --shared -o pam_cap.so $< -lcap

pam_cap.o: pam_cap.c
	$(CC) -fPIC -c $<

test: test.c pam_cap.o
	$(CC) -o test test.c pam_cap.o -lpam -ldl -lcap

clean:
	rm -f *.o *.so test
