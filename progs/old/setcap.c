/*
 * $Id: setcap.c,v 1.2 1997/05/04 05:34:32 morgan Exp $
 *
 * Copyright (c) 1997 Andrew G. Morgan  <morgan@parc.power.net>
 *
 * This sets the capabilities of a given file.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/capability.h>
#include <unistd.h>

static void usage(void)
{
    fprintf(stderr,
	    "usage: setcap [-q] (-|<caps>) <filename> "
	    "[ ... (-|<capsN>) <filenameN> ]\n"
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
	if (!strcmp(*argv,"-")) {
	    retval = read_caps(quiet, *argv, buffer);
	    if (retval)
		usage();
	    text = buffer;
	} else
	    text = *argv;

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
	    fprintf(stderr, "[caps set to:\n%s\n]\n", result);
	}
#endif

	if (--argc <= 0)
	    usage();

	retval = cap_set_file(*++argv, cap_d);

	if (retval != 0) {
	    fprintf(stderr,
		    "Failed to set capabilities on file `%s'\n"
		    " (%s)\n", argv[0], strerror(errno));
	    usage();
	}
    }

    return 0;
}

/*
 * $Log: setcap.c,v $
 * Revision 1.2  1997/05/04 05:34:32  morgan
 * non void main
 *
 * Revision 1.1  1997/04/28 01:01:20  morgan
 * Initial revision
 *
 */
