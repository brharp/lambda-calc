/*++
/* NAME
/*	num 3
/* SUMMARY
/*	numerical functions
/* SYNOPSIS
/*	#include <num.h>
/*
/*	const Exp	*church_encode(const unsigned int n);
/* DESCRIPTION
/*  The function church_encode() takes a constant
/*  integer and returns the "Church encoding" of that integer;
/*  that is, a function of two arguments, f and x, that applies
/*  function f to x, n times.
/* AUTHOR
/*  Brent Harp
/*--*/

#include "num.h"
#include "exp.h"

const Exp *church_encode(const unsigned int n)
{
	const Exp    *f = make_symbol_exp(L"f");
	const Exp    *x = make_symbol_exp(L"x");
	const Exp    *e = x;
	unsigned int  i = n;

	while (i--) {
		e = make_pair_exp(f, e);
	}

	return make_lambda_exp(f, make_lambda_exp(x, e));
}