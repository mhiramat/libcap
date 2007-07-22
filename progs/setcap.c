/*
 * Copyright (c) 1997,2007 Andrew G. Morgan  <morgan@kernel.org>
 *
 * This sets the capabilities of a given file.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/capability.h>
#include <unistd.h>

static void usage(void)
{
    fprintf(stderr,
	    "usage: setcap [-q] (-r|-|<caps>) <filename> "
	    "[ ... (-r|-|<capsN>) <filenameN> ]\n"
	);
    exit(1);
}

#define MAXCAP  2048

static int read_caps(int quiet, const char *filename, char *buffer)
{
    int i=MAXCAP;

    if (!quiet) {
	fprintf(stderr,	"Please enter caps for file [empty line to end]:\n");
    }
    while (i > 0) {
	int j = read(STDIN_FILENO, buffer, i);

	if (j < 0) {
	    fprintf(stderr, "\n[Error - aborting]\n");
	    exit(1);
	}

	if (j==0 || buffer[0] == '\n') {
	    /* we're done */
	    break;
	}

	/* move on... */

	i -= j;
	buffer += j;
    }

    /* <NUL> terminate */
    buffer[0] = '\0';

    return (i < MAXCAP ? 0:-1);
}

int main(int argc, char **argv)
{
    char buffer[MAXCAP+1];
    int retval, quiet=0;
    cap_t cap_d;

    if (argc < 3) {
	usage();
    }

    while (--argc > 0) {
	const char *text;

	if (!strcmp(*++argv,"-q")) {
	    quiet = 1;
	    continue;
	}
	if (!strcmp(*argv,"-r")) {
	    cap_d = NULL;
	} else {
	    if (!strcmp(*argv,"-")) {
		retval = read_caps(quiet, *argv, buffer);
		if (retval)
		    usage();
		text = buffer;
	    } else {
		text = *argv;
	    }

	    cap_d = cap_from_text(text);
	    if (cap_d == NULL) {
		perror("fatal error");
		usage();
	    }
#ifdef DEBUG
	    {
		ssize_t length;
		const char *result;

		result = cap_to_text(cap_d, &length);
		fprintf(stderr, "caps set to: [%s]\n", result);
	    }
#endif
	}

	if (--argc <= 0)
	    usage();

	retval = cap_set_file(*++argv, cap_d);
	if (retval != 0) {
	    fprintf(stderr, "Failed to set capabilities on file `%s' (%s)\n",
		    argv[0], strerror(errno));
	    usage();
	}

	if (cap_d) {
	    cap_free(cap_d);
	}
    }

    return 0;
}
