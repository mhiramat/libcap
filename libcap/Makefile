#
# defines
#
topdir=$(shell pwd)/..
include ../Make.Rules
#
# Library version
#
LIBNAME=$(LIBTITLE).so
STALIBNAME=$(LIBTITLE).a
#

FILES=cap_alloc cap_proc cap_extint cap_flag cap_text cap_file

INCLS=libcap.h cap_names.h $(INCS)
OBJS=$(addsuffix .o, $(FILES))
MAJLIBNAME=$(LIBNAME).$(VERSION)
MINLIBNAME=$(MAJLIBNAME).$(MINOR)
LDFLAGS+=-lattr

all: $(MINLIBNAME) $(STALIBNAME)

_makenames: _makenames.c cap_names.sed
	$(CC) $(CFLAGS) $< -o $@

cap_names.h: _makenames
	./_makenames > cap_names.h

cap_names.sed: Makefile $(KERNEL_HEADERS)/linux/capability.h
	@echo "=> making cap_names.c from <linux/capability.h>"
	@sed -ne '/^#define[ \t]CAP[_A-Z]\+[ \t]\+[0-9]\+/{s/^#define \([^ \t]*\)[ \t]*\([^ \t]*\)/  \{ \2, \"\1\" \},/;y/ABCDEFGHIJKLMNOPQRSTUVWXYZ/abcdefghijklmnopqrstuvwxyz/;p;}' < $(KERNEL_HEADERS)/linux/capability.h | fgrep -v 0x > cap_names.sed
#	@sed -ne '/^#define[ \t]CAP[_A-Z]\+[ \t]\+[0-9]\+/{s/^#define CAP_\([^ \t]*\)[ \t]*\([^ \t]*\)/  \{ \2, \"\1\" \},/;y/ABCDEFGHIJKLMNOPQRSTUVWXYZ/abcdefghijklmnopqrstuvwxyz/;p;}' < /usr/include/linux/capability.h | fgrep -v 0x > cap_names.sed

$(STALIBNAME): $(OBJS)
	ar rcs $(STALIBNAME) $(OBJS)
	ranlib $(STALIBNAME)

$(MINLIBNAME): $(OBJS)
	$(LD) $(LDFLAGS) $(COPTFLAG) -Wl,-soname,$(MAJLIBNAME) -o $@ $(OBJS)
	ln -sf $(MINLIBNAME) $(MAJLIBNAME)
	ln -sf $(MAJLIBNAME) $(LIBNAME)

%.o: %.c $(INCLS)
	$(CC) $(CFLAGS) -c $< -o $@

install: all
	mkdir -p -m 0755 $(INCDIR)/sys
	install -m 0644 include/sys/capability.h $(INCDIR)/sys
	mkdir -p -m 0755 $(LIBDIR)
	install -m 0644 $(STALIBNAME) $(LIBDIR)/$(MINLIBNAME)
	install -m 0644 $(MINLIBNAME) $(LIBDIR)/$(MINLIBNAME)
	ln -sf $(MINLIBNAME) $(LIBDIR)/$(MAJLIBNAME)
	ln -sf $(MAJLIBNAME) $(LIBDIR)/$(LIBNAME)
	-/sbin/ldconfig

clean:
	$(LOCALCLEAN)
	rm -f $(OBJS) $(LIBNAME)* $(STALIBNAME)
	rm -f cap_names.h cap_names.sed _makenames
	cd include/sys && $(LOCALCLEAN)
