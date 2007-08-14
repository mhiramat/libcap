#
# Makefile for libcap
#
topdir=$(shell pwd)
include Make.Rules

#
# flags
#

all install clean: %: %-here
	$(MAKE) -C libcap $(MAKE_DEFS) $@
	$(MAKE) -C pam_cap $(MAKE_DEFS) $@
	$(MAKE) -C progs $(MAKE_DEFS) $@
	$(MAKE) -C doc $(MAKE_DEFS) $@

all-here:

install-here:

clean-here:
	$(LOCALCLEAN)

distclean: clean
	$(DISTCLEAN)

release: distclean
	cd .. && ln -s libcap libcap-$(VERSION).$(MINOR) && tar cvfz libcap-$(VERSION).$(MINOR).tar.gz libcap-$(VERSION).$(MINOR)/* && rm libcap-$(VERSION).$(MINOR)
