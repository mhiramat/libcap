#
# $Id: Makefile,v 1.1.1.1 1999/04/17 22:16:31 morgan Exp $
#
# Makefile for libcap documentation
#

topdir=$(shell pwd)/..
include $(topdir)/Make.Rules

MAN2S = capget.2 capset.2
MAN3S = cap_init.3 cap_free.3 cap_dup.3 \
	cap_clear.3 cap_get_flag.3 cap_set_flag.3 \
	cap_get_proc.3 cap_set_proc.3 \
	cap_get_file.3 cap_get_fd.3 cap_set_file.3 cap_set_fd.3 \
	cap_copy_ext.3 cap_size.3 cap_copy_int.3 \
	cap_from_text.3 cap_to_text.3 _cap_names.3 \
	capsetp.3 capgetp.3
MAN8S = getcap.8 setcap.8

MANS = $(MAN2S) $(MAN3S) $(MAN8S)

all: $(MANS)

install:
	mkdir -p -m 755 $(MANDIR)/man2 $(MANDIR)/man3
	for man in \
		$(MANDIR)/man2 $(MAN2S) \
		$(MANDIR)/man3 $(MAN3S) \
		$(MANDIR)/man8 $(MAN8S) \
		; \
	do \
		case $$man in \
		/*)	sub=$$man ; continue ;; \
		esac; \
		install -m 644 $$man $$sub ; \
	done

clean:
	$(LOCALCLEAN)

