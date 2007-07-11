##
## $Log: Makefile,v $
## Revision 1.6  1998/09/20 23:17:32  morgan
## added sucap.c
##
## Revision 1.5  1998/06/07 01:54:43  morgan
## updated for 0.104.  Added execcap.
##
## Revision 1.4  1997/05/14 05:18:23  morgan
## autoconf rearrangement from Zefram
##
## Revision 1.3  1997/05/04 05:34:03  morgan
## took care of case that install cannot handle more than one file
##
## Revision 1.2  1997/04/28 01:01:20  morgan
## update with zefram's patches
##
## Revision 1.1  1997/04/21 04:34:04  morgan
## Initial revision
##
##

topdir=$(shell pwd)/..
include $(topdir)/Make.Rules
#
# Programs: all of the examples that we will compile
#
PROGS=getpcaps setpcaps execcap sucap

# when we have filecaps...
#PROGS+=getcap setcap

all: $(PROGS)

$(PROGS): %: %.o
	$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

%.o: %.c $(INCS)
	$(CC) $(CFLAGS) -c $< -o $@

install: all
	mkdir -p -m 0755 $(SBINDIR)
	for p in $(PROGS) ; do \
		install -s -m 0755 $$p $(SBINDIR) ; \
	done

clean:
	$(LOCALCLEAN)
	rm -f *.o $(PROGS)
