#ifndef _PRINT_H_INCLUDED_
#define _PRINT_H_INCLUDED_
#include <stdio.h>
#include "types.h"
/*++
/* NAME
/*	print 3h
/* SUMMARY
/*	Printing functions.
/* DESCRIPTION
/* .nf

 /* function prototypes */

void print_value(const Value * val, FILE *);
void print_string(const wchar_t * str, FILE *);
void print_exp(const Exp * exp, FILE *);
void pexp(const Exp * exp);
void pval(const Value *val);

/* AUTHOR
/*	Brent Harp
/*--*/
#endif
