/*
 * Copyright (c) 1997-8 Andrew G. Morgan <morgan@kernel.org>
 *
 * This file deals with flipping of capabilities on internal
 * capability sets as specified by POSIX.1e (formerlly, POSIX 6).
 */

#include "libcap.h"

/*
 * Return the state of a specified capability flag.  The state is
 * returned as the contents of *raised.  The capability is from one of
 * the sets stored in cap_d as specified by set and value
 */

int cap_get_flag(cap_t cap_d, cap_value_t value, cap_flag_t set,
		 cap_flag_value_t *raised)
{
    /*
     * Do we have a set and a place to store its value?
     * Is it a known capability?
     */

    if (raised && good_cap_t(cap_d) && value >= 0 && value < __CAP_BITS
	&& set >= 0 && set < NUMBER_OF_CAP_SETS) {
	*raised = isset_cap(cap_d,value,set) ? CAP_SET:CAP_CLEAR;
	return 0;
    } else {
	_cap_debug("invalid arguments");
	errno = EINVAL;
	return -1;
    }
}

/*
 * raise/lower a selection of capabilities
 */

int cap_set_flag(cap_t cap_d, cap_flag_t set,
		 int no_values, cap_value_t *array_values,
		 cap_flag_value_t raise)
{
    /*
     * Do we have a set and a place to store its value?
     * Is it a known capability?
     */

    if (good_cap_t(cap_d) && no_values > 0 && no_values <= __CAP_BITS
	&& (set >= 0) && (set < NUMBER_OF_CAP_SETS)
	&& (raise == CAP_SET || raise == CAP_CLEAR) ) {
	int i;
	for (i=0; i<no_values; ++i) {
	    if (array_values[i] < 0 || array_values[i] >= __CAP_BITS) {
		_cap_debug("weird capability (%d) - skipped", array_values[i]);
	    } else {
		int value = array_values[i];

		if (raise == CAP_SET) {
		    cap_d->raise_cap(value,set);
		} else {
		    cap_d->lower_cap(value,set);
		}
	    }
	}
	return 0;

    } else {

	_cap_debug("invalid arguments");
	errno = EINVAL;
	return -1;

    }
}

/*
 *  Reset the capability to be empty (nothing raised)
 */

int cap_clear(cap_t cap_d)
{
    if (good_cap_t(cap_d)) {

	memset(&(cap_d->u), 0, sizeof(cap_d->u));
	return 0;

    } else {

	_cap_debug("invalid pointer");
	errno = EINVAL;
	return -1;

    }
}

