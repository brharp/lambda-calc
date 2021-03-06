#ifndef _READ_H_INCLUDED_
#define _READ_H_INCLUDED_
#include "types.h"
/*++
/* NAME
/*	read 3h
/* SUMMARY
/*	Interface to the reader.
/* DESCRIPTION
/* .nf

 /* Function prototypes. */

const Exp *read_exp(FILE *);
const Exp *read_statement(FILE *);

/* AUTHOR
/*	Brent Harp
/*--*/
#endif
