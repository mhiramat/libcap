#
# Makefile for libcap documentation
#

topdir=$(shell pwd)/..
include $(topdir)/Make.Rules

MAN3S = cap_init.3 cap_free.3 cap_dup.3 \
	cap_clear.3 cap_clear_flag.3 cap_get_flag.3 cap_set_flag.3 \
	cap_get_proc.3 cap_set_proc.3 \
	cap_get_file.3 cap_get_fd.3 cap_set_file.3 cap_set_fd.3 \
	cap_copy_ext.3 cap_size.3 cap_copy_int.3 \
	cap_from_text.3 cap_to_text.3 cap_from_name.3 cap_to_name.3 \
	capsetp.3 capgetp.3
MAN8S = getcap.8 setcap.8

MANS = $(MAN3S) $(MAN8S)

all: $(MANS)

install:
	mkdir -p -m 755 $(MANDIR)/man3 $(MANDIR)/man8
	for man in \
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

