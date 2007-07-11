#
# $Id: Makefile,v 1.1.1.1 1999/04/17 22:16:31 morgan Exp $
#
# Makefile for libcap

topdir=$(shell pwd)
include Make.Rules

#
# flags
#

all install clean: %: %-here
	make -C libcap $(MAKE_DEFS) $@
	make -C progs $(MAKE_DEFS) $@
	make -C doc $(MAKE_DEFS) $@

all-here:

install-here:

clean-here:
	$(LOCALCLEAN)

distclean: clean
	$(DISTCLEAN)

release: distclean
	cd .. ; tar cvfz libcap-$(VERSION).$(MINOR).tar.gz libcap-$(VERSION).$(MINOR)
