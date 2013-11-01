/*++
/* NAME
/*	read 3
/* SUMMARY
/*	expression reader
/* SYNOPSIS
/*	#include <read.h>
/*	
/*	Exp	*read(void);
/*	
/*--*/


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <search.h>
#include <string.h>
#include <assert.h>

#include "read.h"
#include "exp.h"
#include "char.h"
#include "num.h"

 /* key words */

static const wchar_t *print = L"print";

 /* function prototypes */

static const Exp *read_exp_list(FILE *);
static const Exp *read_sub_exp(FILE *);
static const Exp *read_quote_exp(FILE *);
static const Exp *read_lambda_exp(FILE *);
static const Exp *read_symbol_exp(FILE *);
static const Exp *read_num_exp(FILE *);
static void read_dot(FILE *);
static wint_t read_char(FILE *, bool);
static void unread_char(wint_t, FILE *);
static const Exp *parse_error(const wchar_t *, ...);
static wint_t get_char(FILE *);

const Exp *read_exp_list_delim(wint_t c, FILE * stream)
{
    const Exp *e = 0;

    e = read_exp_list(stream);
    if (read_char(stream, false) != c) {
	parse_error(L"expected '%c'", c);
    }

    return e;
}

static const Exp *read_exp_list(FILE * stream)
{
	const Exp *exp = 0, *lst = 0;
	wint_t c = 0;

	if ((exp = read_exp(stream)) == 0) {
		return 0;
	}

	if ((c = read_char(stream, true)) == L'=') {
		return make_assign_exp(exp, read_exp_list(stream));
	}

	unread_char(c, stream);
	for (lst = read_exp(stream); lst != 0; lst = read_exp(stream)) {
		exp = make_pair_exp(exp, lst);
	}

	return exp;
}

const Exp *read_exp(FILE * stream)
{
	wint_t c;

	c = read_char(stream, true);

	if (c == L'\\') {
		return read_lambda_exp(stream);
	} else if (c == L'\'') {
		return read_quote_exp(stream);
	} else if (c == L'(') {
		return read_sub_exp(stream);
	} else if (L'a' <= c && c <= L'z'
		    || L'A' <= c && c <= L'Z'
			|| L'0' <= c && c <= L'9') {
		unread_char(c, stream);
		return read_symbol_exp(stream);
	} else if (c == L'#') {
		return read_num_exp(stream);
	} else {
		/* invalid expression */
		unread_char(c, stream);
		return 0;
	}
}


static const Exp *read_sub_exp(FILE *stream)
{
	return read_exp_list_delim(rparen, stream);
}


static const Exp *read_quote_exp(FILE *stream)
{
	return make_quote_exp(read_exp(stream));
}


static const Exp *read_lambda_exp(FILE *stream)
{
	const Exp *param = 0, *body = 0, *exp = 0;

	param = read_symbol_exp(stream);
	read_dot(stream);
	body = read_exp_list(stream);
	exp = make_lambda_exp(param, body);

	return exp;
}

static void read_dot(FILE * stream)
{
	wint_t ch = 0;

	ch = read_char(stream, false);

	if (ch != L'.') {
		parse_error(L"expected '.', found '%lc'", ch);
	}
}


/* read_symbol - read a symbol from a stream */

static const Exp *read_symbol_exp(FILE *stream)
{
	#define MAX_SYMBOL_LENGTH (64)
	const size_t max_symbol_length = 64;
	wchar_t sb[MAX_SYMBOL_LENGTH + 1], *sp = 0;
	wint_t ch = 0;

	/* initialize */
	sp = sb;
	ch = read_char(stream, true);	/* skip whitespace */

	/* loop while char is alpha-numeric */
	while (L'a' <= ch && ch <= L'z'
		|| L'A' <= ch && ch <= L'Z'
		|| L'0' <= ch && ch <= L'9') {
		if (sp < sb + max_symbol_length) {
			*sp++ = ch;
		}
		ch = read_char(stream, false);
	}

	/* put back non-alpha-numeric char */
	unread_char(ch, stream); 

	/* assert one or more characters were read */
	assert(sp != sb);

	/* null terminate string */
	*sp = L'\0';

	/* make and return symbol expression */
	return make_symbol_exp(sb);
}


  /* read_num - read a number from a stream */

static const Exp *read_num_exp(FILE *stream)
{
	wint_t       ch   = 0;
	unsigned int nval = 0;

	/* initialize */
	ch = read_char(stream, true);	/* skip whitespace */

	/* loop while char is numeric */
	while (L'0' <= ch && ch <= L'9') {
		nval *= 10;
		nval += (ch - L'0');
		ch = read_char(stream, false);
	}

	/* put back non-alpha-numeric char */
	unread_char(ch, stream);

	/* church encode integer */
	return church_encode(nval);
}


/* read_char - read a character, optionally skipping whitespace */

static wint_t read_char(FILE *stream, bool skip_space)
{
	wint_t ch = 0;

	/* initialize */
	ch = get_char(stream);

	if (skip_space) {
		while (ch == space || ch == tab) {
			ch = get_char(stream);
		}
	}

	return ch;
}


static wint_t get_char(FILE *stream)
{
	wint_t c;
    
	if ((c = fgetwc(stream)) == comment) {
		while (c != newline) {
			c = fgetwc(stream);
		}
	}
	if (c == WEOF) {
		exit(EXIT_SUCCESS);
	}
	return c;
}


static void unread_char(wint_t ch, FILE *stream)
{
	if (ch != WEOF) {
		ungetwc(ch, stream);
	}
}


static const Exp *parse_error(const wchar_t *fmt, ...)
{
	va_list ap;

	/* format msg */
	va_start(ap, fmt);
	vfwprintf(stderr, fmt, ap);
	va_end(ap);

	return 0;
}

