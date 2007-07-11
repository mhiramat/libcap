/*
 * $Id: getcap.c,v 1.1.1.1 1999/04/17 22:16:31 morgan Exp $
 *
 * Copyright (c) 1997 Andrew G. Morgan  <morgan@parc.power.net>
 *
 * This displays the capabilities of a given file.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/capability.h>

static void usage(void)
{
    fprintf(stderr,
	    "usage: getcap <filename> [<filename> ...]\n"
	    "\n"
	    "\tdisplays the capabilities on the queried file(s).\n"
	);
    exit(1);
}

int main(int argc, char **argv)
{
    char *result=NULL;

    if (argc < 2) {
	usage();
    }

    for ( ++argv; --argc > 0; ++argv ) {
	ssize_t length;
	cap_t cap_d;

	cap_d = cap_get_file(argv[0]);

	if (cap_d == NULL) {
	    fprintf(stderr,
		    "Failed to get capabilities for file `%s'\n"
		    " (%s)\n", argv[0], strerror(errno));
	    continue;
	}

	result = cap_to_text(cap_d, &length);

	fprintf(stderr, "Capabilities for `%s':\n%s\n", *argv, result);
    }

    return 0;
}

/*
 * $Log: getcap.c,v $
 * Revision 1.1.1.1  1999/04/17 22:16:31  morgan
 * release 1.0 of libcap
 *
 * Revision 1.3  1997/05/04 05:34:32  morgan
 * non void main
 *
 * Revision 1.2  1997/04/28 01:01:20  morgan
 * update to allow more than one argument file
 *
 * Revision 1.1  1997/04/21 04:34:04  morgan
 * Initial revision
 *
 */
