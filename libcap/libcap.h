/*
 * Copyright (c) 1997 Andrew G Morgan <morgan@kernel.org>
 *
 * This file contains internal definitions for the various functions in
 * this small capability library.
 */

#ifndef LIBCAP_H
#define LIBCAP_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/capability.h>

#ifndef __u8
#define __u8    unsigned char
#endif /* __8 */

#ifndef __u32
#define __u32   unsigned int
#endif /* __u32 */

/* include the names for the caps and a definition of __CAP_BITS */
#include "cap_names.h"

/*
 * This is a pointer to a struct containing three consecutive
 * capability sets in the order of the cap_flag_t type: the are
 * effective,inheritable and permitted.  This is the type that the
 * user-space routines think of as 'internal' capabilities - this is
 * the type that is passed to the kernel with the system calls related
 * to processes.
 */

#define CAP_T_MAGIC 0xCA90D0
struct _cap_struct {
    struct __user_cap_header_struct head;
    struct __user_cap_data_struct set;
};

/* string magic for cap_free */
#define CAP_S_MAGIC 0xCA95D0

/*
 * Do we match the local kernel?
 */

#if !defined(_LINUX_CAPABILITY_VERSION) || \
            (_LINUX_CAPABILITY_VERSION != 0x19980330)

# error "Kernel <linux/capability.h> does not match library"
# error "file "libcap.h" --> fix and recompile libcap"

#endif

/*
 * kernel API cap set abstraction
 */

#define NUMBER_OF_CAP_SETS      3   /* effective, inheritable, permitted */
#define CAP_SET_SIZE (sizeof(struct __user_cap_data_struct)/NUMBER_OF_CAP_SETS)
#define __CAP_BLKS   (CAP_SET_SIZE/sizeof(__u32))
typedef struct {
    __u32 _blk[__CAP_BLKS];
} __cap_s;
#define raise_cap(x)   _blk[(x)>>5] |= (1<<((x)&31))
#define lower_cap(x)   _blk[(x)>>5] &= ~(1<<((x)&31))
#define isset_cap(y,x) ((y)->_blk[(x)>>5] & (1<<((x)&31)))

/*
 * Private definitions for internal use by the library.
 */

#define __libcap_check_magic(c,magic) ((c) && *(-1+(__u32 *)(c)) == (magic))
#define good_cap_t(c)        __libcap_check_magic(c, CAP_T_MAGIC)
#define good_cap_string(c)   __libcap_check_magic(c, CAP_S_MAGIC)

/*
 * library debugging
 */
#ifdef DEBUG

#include <stdio.h>
# define _cap_debug(f, x...)  { \
    fprintf(stderr, __FUNCTION__ "(" __FILE__ ":%d): ", __LINE__); \
    fprintf(stderr, f, ## x); \
    fprintf(stderr, "\n"); \
}
# define _cap_debugcap(s, c) \
    fprintf(stderr, __FUNCTION__ "(" __FILE__ ":%d): " s \
       "%08x\n", __LINE__, *(c))

#else /* !DEBUG */

# define _cap_debug(f, x...)
# define _cap_debugcap(s, c)

#endif /* DEBUG */

extern char *_libcap_strdup(const char *text);

/*
 * These are semi-public prototypes, they will only be defined in
 * <sys/capability.h> if _POSIX_SOURCE is not #define'd, so we
 * place them here too.
 */

extern int capset(cap_user_header_t header, cap_user_data_t data);
extern int capget(cap_user_header_t header, const cap_user_data_t data);
extern int capgetp(pid_t pid, cap_t cap_d);
extern int capsetp(pid_t pid, cap_t cap_d);

#endif /* LIBCAP_H */
