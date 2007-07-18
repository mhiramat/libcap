/*
 * Copyright (c) 1997,2007 Andrew G. Morgan  <morgan@kernel.org>
 *
 * This displays the capabilities of a given file.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

	cap_free(result);
	cap_free(cap_d);
    }

    return 0;
}
