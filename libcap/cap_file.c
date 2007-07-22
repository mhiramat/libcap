/*
 * Copyright (c) 1997,2007 Andrew G Morgan <morgan@kernel.org>
 *
 * This file deals with setting capabilities on files.
 */

#include <sys/types.h>
#include <attr/xattr.h>
#include <byteswap.h>

#define XATTR_SECURITY_PREFIX "security."

#include "libcap.h"

#if __BYTE_ORDER == __BIG_ENDIAN
#define FIXUP_32BITS(x) bswap_32(x)
#else
#define FIXUP_32BITS(x) (x)
#endif

static cap_t _fcaps_load(struct vfs_cap_data *rawvfscap, cap_t result)
{
    __u32 magic_etc;

    magic_etc = FIXUP_32BITS(rawvfscap->magic_etc);
    switch (magic_etc & VFS_CAP_REVISION_MASK) {
    case VFS_CAP_REVISION_1:
	result->set.inheritable =
	    FIXUP_32BITS(rawvfscap->data[0].inheritable);
	result->set.permitted =
	    FIXUP_32BITS(rawvfscap->data[0].permitted);
	if (magic_etc & VFS_CAP_FLAGS_EFFECTIVE) {
	    result->set.effective =
		result->set.inheritable | result->set.permitted;
	}
	break;
    default:
	cap_free(result);
	result = NULL;
    }

    return result;
}

static int _fcaps_save(struct vfs_cap_data *rawvfscap, cap_t cap_d)
{
    if (!good_cap_t(cap_d)) {
	errno = EINVAL;
	return -1;
    }

    _cap_debug("setting named file capabilities");

    if (cap_d->set.effective == 0) {
	rawvfscap->magic_etc = FIXUP_32BITS(VFS_CAP_REVISION);
    } else if ((~(cap_d->set.effective))
	       & (cap_d->set.inheritable|cap_d->set.permitted)) {
	errno = EINVAL;
	return -1;
    } else {
	rawvfscap->magic_etc
	    = FIXUP_32BITS(VFS_CAP_REVISION|VFS_CAP_FLAGS_EFFECTIVE);
    }

    rawvfscap->data[0].permitted = FIXUP_32BITS(cap_d->set.permitted);
    rawvfscap->data[0].inheritable = FIXUP_32BITS(cap_d->set.inheritable);

    return 0;      /* success */
}

/*
 * Get the capabilities of an open file, as specified by its file
 * descriptor.
 */

cap_t cap_get_fd(int fildes)
{
    cap_t result;

    /* allocate a new capability set */
    result = cap_init();
    if (result) {
	struct vfs_cap_data rawvfscap;

	_cap_debug("getting fildes capabilities");

	/* fill the capability sets via a system call */
	if (sizeof(rawvfscap) != fgetxattr(fildes, XATTR_NAME_CAPS,
					   &rawvfscap, sizeof(rawvfscap))) {
	    cap_free(result);
	    result = NULL;
	}

	result = _fcaps_load(&rawvfscap, result);
    }

    return result;
}

/*
 * Set the capabilities on a named file.
 */

cap_t cap_get_file(const char *filename)
{
    cap_t result;

    /* allocate a new capability set */
    result = cap_init();
    if (result) {
	struct vfs_cap_data rawvfscap;

	_cap_debug("getting filename capabilities");

	/* fill the capability sets via a system call */
	if (sizeof(rawvfscap) != getxattr(filename, XATTR_NAME_CAPS,
					  &rawvfscap, sizeof(rawvfscap))) {
	    cap_free(result);
	    result = NULL;
	}

	result = _fcaps_load(&rawvfscap, result);
    }

    return result;
}

/*
 * Set the capabilities of an open file, as specified by its file
 * descriptor.
 */

int cap_set_fd(int fildes, cap_t cap_d)
{
    struct vfs_cap_data rawvfscap;

    if (cap_d == NULL) {
	_cap_debug("deleting fildes capabilities");
	return fremovexattr(fildes, XATTR_NAME_CAPS);
    } else if (_fcaps_save(&rawvfscap, cap_d) != 0) {
	return -1;
    }

    _cap_debug("setting fildes capabilities");

    return fsetxattr(fildes, XATTR_NAME_CAPS, &rawvfscap,
		     sizeof(rawvfscap), 0);
}

/*
 * Set the capabilities of a named file.
 */

int cap_set_file(const char *filename, cap_t cap_d)
{
    struct vfs_cap_data rawvfscap;

    if (cap_d == NULL) {
	_cap_debug("removing filename capabilities");
	return removexattr(filename, XATTR_NAME_CAPS);
    } else if (_fcaps_save(&rawvfscap, cap_d) != 0) {
	return -1;
    }

    _cap_debug("setting filename capabilities");
    return setxattr(filename, XATTR_NAME_CAPS, &rawvfscap,
		    sizeof(rawvfscap), 0);
}
