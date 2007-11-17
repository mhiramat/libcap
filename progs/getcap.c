/*
 * Copyright (c) 1997,2007 Andrew G. Morgan  <morgan@kernel.org>
 *
 * This displays the capabilities of a given file.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/capability.h>

static int verbose = 0;
static int recursive = 0;

static void usage(void)
{
    fprintf(stderr,
	    "usage: getcap [-v] [-r] [-h] <filename> [<filename> ...]\n"
	    "\n"
	    "\tdisplays the capabilities on the queried file(s).\n"
	);
    exit(1);
}

static void do_recursive(const char *fname);

static void do_getcap(const char *fname)
{
    cap_t cap_d;
    char *result;

    cap_d = cap_get_file(fname);
    if (cap_d == NULL) {
	if (errno != ENODATA) {
	    fprintf(stderr, "Failed to get capabilities of file `%s' (%s)\n",
		    fname, strerror(errno));
	} else if (verbose) {
	    printf("%s\n", fname);
	}
	goto out;
    }

    result = cap_to_text(cap_d, NULL);
    if (!result) {
	fprintf(stderr,
		"Failed to get capabilities of human readable format at `%s' (%s)\n",
		fname, strerror(errno));
	cap_free(cap_d);
	return;
    }
    printf("%s %s\n", fname, result);
    cap_free(cap_d);
    cap_free(result);

  out:
    if (recursive) {
	struct stat stbuf;

	if (stat(fname, &stbuf)) {
	    fprintf(stderr, "Failed to get attribute of file `%s' (%s)\n",
		    fname, strerror(errno));
	} else if (S_ISDIR(stbuf.st_mode)) {
	    do_recursive(fname);
	}
    }
}

static void do_recursive(const char *fname)
{
    DIR *dirp;
    struct dirent *dent;
    char buffer[PATH_MAX];

    dirp = opendir(fname);
    if (dirp == NULL) {
	fprintf(stderr, "Failed to open directory `%s' (%s)\n",
		fname, strerror(errno));
	return;
    }

    while ((dent = readdir(dirp)) != NULL) {
	if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, ".."))
	    continue;
	snprintf(buffer, PATH_MAX, "%s/%s", fname, dent->d_name);
	do_getcap(buffer);
    }
    closedir(dirp);
}

int main(int argc, char **argv)
{
    int i, c;

    while ((c = getopt(argc, argv, "rvh")) > 0) {
	switch(c) {
	case 'r':
	    recursive = 1;
	    break;
	case 'v':
	    verbose = 1;
	    break;
	default:
	    usage();
	}
    }

    if (!argv[optind])
	usage();

    for (i=optind; argv[i] != NULL; i++)
	do_getcap(argv[i]);

    return 0;
}
