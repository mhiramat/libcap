##
## $Log: Makefile,v $
## Revision 1.1.1.1  1999/04/17 22:16:31  morgan
## release 1.0 of libcap
##
## Revision 1.5  1998/05/24 22:54:09  morgan
## updated for 2.1.104
##
## Revision 1.4  1997/05/14 05:17:13  morgan
## autoconf rearrangement from Zefram
##
## Revision 1.3  1997/05/04 05:34:59  morgan
## cleaner
##
## Revision 1.2  1997/04/28 00:57:11  morgan
## fixes and zefram's patches
##
## Revision 1.1  1997/04/21 04:33:29  morgan
## Initial revision
##
##
##

#
# defines
#
topdir=$(shell pwd)/..
include ../Make.Rules
#
# Library version
#
LIBNAME=libcap.so
#

FILES=cap_alloc cap_proc cap_extint cap_flag cap_text cap_sys

# for later when there is filesystem support for cap's:
#FILES += cap_file 

INCLS=libcap.h cap_names.h $(INCS)
OBJS=$(addsuffix .o, $(FILES))
MAJLIBNAME=$(LIBNAME).$(VERSION)
MINLIBNAME=$(MAJLIBNAME).$(MINOR)

all: $(MINLIBNAME)

_makenames: _makenames.c cap_names.sed
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

cap_names.h: _makenames
	./_makenames > cap_names.h

cap_names.sed: Makefile /usr/include/linux/capability.h
	@echo "=> making cap_names.c from <linux/capability.h>"
	@sed -ne '/^#define[ \t]CAP[_A-Z]\+[ \t]\+[0-9]\+/{s/^#define \([^ \t]*\)[ \t]*\([^ \t]*\)/  \{ \2, \"\1\" \},/;y/ABCDEFGHIJKLMNOPQRSTUVWXYZ/abcdefghijklmnopqrstuvwxyz/;p;}' < /usr/include/linux/capability.h | fgrep -v 0x > cap_names.sed
#	@sed -ne '/^#define[ \t]CAP[_A-Z]\+[ \t]\+[0-9]\+/{s/^#define CAP_\([^ \t]*\)[ \t]*\([^ \t]*\)/  \{ \2, \"\1\" \},/;y/ABCDEFGHIJKLMNOPQRSTUVWXYZ/abcdefghijklmnopqrstuvwxyz/;p;}' < /usr/include/linux/capability.h | fgrep -v 0x > cap_names.sed

$(MINLIBNAME): $(OBJS)
	$(LD) -soname $(MAJLIBNAME) -x -shared -o $@ $(OBJS)
	ln -sf $(MINLIBNAME) $(MAJLIBNAME)
	ln -sf $(MAJLIBNAME) $(LIBNAME)

%.o: %.c $(INCLS)
	$(CC) $(CFLAGS) -c $< -o $@

install: all
	mkdir -p -m 0755 $(INCDIR)/sys
	install -m 0644 include/sys/capability.h $(INCDIR)/sys
	mkdir -p -m 0755 $(LIBDIR)
	install -m 0644 $(MINLIBNAME) $(LIBDIR)/$(MINLIBNAME)
	ln -sf $(MINLIBNAME) $(LIBDIR)/$(MAJLIBNAME)
	ln -sf $(MAJLIBNAME) $(LIBDIR)/$(LIBNAME)
	-/sbin/ldconfig

clean:
	$(LOCALCLEAN)
	rm -f $(OBJS) $(LIBNAME)*
	rm -f cap_names.h cap_names.sed _makenames
	cd include/sys && $(LOCALCLEAN)

