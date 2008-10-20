/*
 * Copyright (c) 2008 Andrew G. Morgan <morgan@kernel.org>
 *
 * This is a simple script that attempts to verify a file has
 * a specific set of capabilities associated with it. This
 * code is intended to be a simple prototype for inclusion
 * in package manager applications.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/capability.h>

static int caps_differ(const char *filename, const char *file_caps)
{
    int cmp;
    cap_t in_effect, reference;
    
    in_effect = cap_get_file(filename);
    reference = cap_from_text(file_caps);

    if ((in_effect == NULL) || (reference == NULL)) {
	char *text;

	if (in_effect == reference) {
	    return 0;
	} else if (in_effect != NULL) {
	    text = cap_to_text(in_effect, NULL);
	    printf("reference caps (empty) vs. current (%s)\n", text);
	} else {
	    text = cap_to_text(reference, NULL);
	    printf("reference caps (%s) vs. current (empty)\n", text);
	}
	cap_free(text);
	return 1;
    }

    cmp = cap_compare(in_effect, reference);
    if (cmp == 0) {
	return 0;
    } else {
	char *text_ref, *text_current;

	text_current = cap_to_text(in_effect, NULL);
	text_ref = cap_to_text(reference, NULL);

	printf("reference caps (%s) vs. current (%s) [differ:%s%s%s]\n",
	       text_ref, text_current,
	       CAP_DIFFERS(cmp, CAP_PERMITTED) ? "p" : "",
	       CAP_DIFFERS(cmp, CAP_INHERITABLE) ? "i" : "",
	       CAP_DIFFERS(cmp, CAP_EFFECTIVE) ? "e" : "");

	cap_free(text_ref);
	cap_free(text_current);

	return cmp;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
	fprintf(stderr, "usage: %s <filename> <expected-caps>\n", argv[0]);
	exit(1);
    }

    if (caps_differ(argv[1], argv[2])) {
	printf("capabilities differ\n");
	exit(1);
    } else {
	printf("capabibilities are as expected\n");
    }

    exit(0);
}
