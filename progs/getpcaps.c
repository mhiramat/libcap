/*
 * $Id: getpcaps.c,v 1.2 1999/11/18 06:04:25 morgan Exp $
 *
 * Copyright (c) 1997 Andrew G. Morgan  <morgan@linux.kernel.org>
 *
 * This displays the capabilities of a given process.
 */

#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#undef _POSIX_SOURCE
#include <sys/capability.h>

static void usage(void)
{
    fprintf(stderr,
"usage: getcaps <pid> [<pid> ...]\n\n"
"  This program displays the capabilities on the queried process(es).\n"
"  The capabilities are displayed in the cap_from_text(3) format.\n\n"
"[Copyright (c) 1997-8 Andrew G. Morgan  <morgan@linux.kernel.org>]\n"
	);
    exit(1);
}

int main(int argc, char **argv)
{
    cap_t cap_d;

    if (argc < 2) {
	usage();
    }

    cap_d = cap_init();
    for ( ++argv; --argc > 0; ++argv ) {
	ssize_t length;
	int pid;

	if (cap_d == NULL) {
	    fprintf(stderr, "Failed to make a blank capability set\n"
		    " (%s)\n", strerror(errno));
	    exit(1);
	}

	pid = atoi(argv[0]);
	/* this is a non-POSIX function */
	if (capgetp(pid, cap_d)) {
		fprintf(stderr, "Failed to get cap's for proccess %d:"
			" (%s)\n", pid, strerror(errno));
		continue;
	} else {
	    char *result = cap_to_text(cap_d, &length);
	    fprintf(stderr, "Capabilities for `%s': %s\n", *argv, result);
	    cap_free(result);
	    result = NULL;
	}
    }

    return 0;
}

/*
 * $Log: getpcaps.c,v $
 * Revision 1.2  1999/11/18 06:04:25  morgan
 * use cap_free and not free for string from cap_to_text()
 *
 * Revision 1.1.1.1  1999/04/17 22:16:31  morgan
 * release 1.0 of libcap
 *
 * Revision 1.4  1998/09/20 23:07:08  morgan
 * fixed comment at top
 *
 * Revision 1.3  1998/06/08 00:16:58  morgan
 * change to accommodate alpha (glibc?)
 *
 * Revision 1.2  1998/06/07 01:49:39  morgan
 * added copyright info and some usage info.  Small tidy up.
 *
 * Revision 1.1  1998/04/30 02:53:00  morgan
 * Initial revision
 *
 */
