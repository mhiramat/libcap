
topdir=$(shell pwd)/..
include $(topdir)/Make.Rules
#
# Programs: all of the examples that we will compile
#
PROGS=getpcaps getcap setcap capsh

LDFLAGS += --static 
LDLIBS += -lcap

all: $(PROGS)

$(PROGS): %: %.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

%.o: %.c $(INCS)
	$(CC) $(CFLAGS) -c $< -o $@

install: all
	mkdir -p -m 0755 $(SBINDIR)
	for p in $(PROGS) ; do \
		install -m 0755 $$p $(SBINDIR) ; \
	done

clean:
	$(LOCALCLEAN)
	rm -f *.o $(PROGS) tcapsh ping
