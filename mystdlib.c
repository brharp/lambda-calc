/*++
/* NAME
/*	mystdlib 3
/* SUMMARY
/*	My versions of standard library functions.
/* SYNOPSIS
/*	void	*mymalloc(sz);
/*	size_t	sz;
/* DESCRIPTION
/*	Memory allocation errors are fatal errors. There is no garbage
/*	collector at this point.
/*--*/

#include <assert.h>
#include "mystdlib.h"

/* mymalloc - allocate memory (or die) */

void *mymalloc(size_t sz)
{
    void *ptr;

    assert(sz > 0);
    assert(sz < 512);
    ptr = malloc(sz);
    assert(ptr != 0);

    return ptr;
}
