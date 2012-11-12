/*++
/* NAME
/*	print 3
/* SUMMARY
/*	Printing functions.
/* SYNOPSIS
/*	#include <print.h>
/*
/*	void print_string(const wchar_t *str);
/*
/*	void print_value(const Value *val);
/*
/*	void print_exp(const Exp *exp);
/*
/*--*/


 /* System libraries */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


 /* Local includes */

#include "types.h"
#include "print.h"
#include "eval.h"
#include "char.h"
#include "env.h"




/* print_value - prints a value */

void print_value(const Value * val, FILE *stream)
{
	Function *fn  = 0;
	Thunk    *thk = 0;

	if (val == 0) {
		fputws(L"(null)", stream);
		return;
	}

	switch (val->type) {
	case T_Function:
		fn = val->data.function;
		//fputws(L"#<Function ", stream);
		if (fn->name != 0) {
			fputws(fn->name, stream);
		} else {
			fputwc(lambda, stream);
			print_exp(fn->param, stream);
			fputwc(separator, stream);
			print_exp(fn->body, stream);
		}
		//fputwc(L'>', stream);
		break;

	case T_Exp:
		print_exp(val->data.exp, stream);
		break;

	case T_Thunk:
		print_thunk(val, stream);
		break;

	default:
		fwprintf(stderr, L"%s: %d: %s: Unknown value type: %d\n",
			__FILE__, __LINE__, "print_value", val->type);
		exit(EXIT_FAILURE);
	}
}


/* print_list */

static void print_list(const Exp *exp, FILE *stream)
{
	assert(exp != 0);

	if (exp->type == T_Exp_Pair) {
		print_list(exp->child[0], stream);
		fputwc(space, stream);
		print_exp(exp->child[1], stream);
	} else {
		print_exp(exp, stream);
	}
}


/* print_exp - prints an expression */

void print_exp(const Exp *exp, FILE *stream)
{
	const Exp *nxt;

	if (exp == 0) {
		fputws(L"<null>", stream);
		return;
	}

	switch (exp->type) {
	case T_Exp_Symbol:
		fputws(exp->sval, stream);
		break;

	case T_Exp_Lambda:
		fputwc(lambda, stream);
		print_exp(exp->child[0], stream);
		fputwc(separator, stream);
		print_exp(exp->child[1], stream);
		break;

	case T_Exp_Pair:
		fputwc(lparen, stream);
		print_list(exp, stream);
		fputwc(rparen, stream);
		break;

	case T_Exp_Quote:
		fputwc(quote, stream);
		print_exp(exp->child[0], stream);
		break;

	case T_Exp_Assign:
		print_exp(exp->child[0], stream);
		fputwc(space, stream);
		fputwc(assign, stream);
		fputwc(space, stream);
		print_exp(exp->child[1], stream);
		break;

	default:
		fwprintf(stderr, L"%s: %d: %s: Unknown exp type: %d\n",
			__FILE__, __LINE__, "print_exp", exp->type);
		exit(EXIT_FAILURE);
	}
}


/* dump_exp */

static wchar_t *type_name[] = {
    L"T_Exp_Symbol",
    L"T_Exp_Lambda",
    L"T_Exp_Pair",
    L"T_Exp_Quote",
    L"T_Exp_Assign"
};

int indent(int delta, FILE *stream)
{
	static int indent = 0;
	int i;

	fputwc(L'\n', stream);
	for (i = 0; i < indent; i++) {
		fputwc(L'\t', stream);
	}
	indent += delta;
	return indent;
}

void dump_exp(const Exp * exp, FILE *stream)
{
    if (exp == 0) {
	fputws(L"(null)", stream);
	return;
    }

    fputws(L"{\n", stream);
    indent(1, stream);
    fputws(L"type: ", stream);
    fputws(type_name[exp->type], stream);
    fputws(L"\n", stream);
    indent(0, stream);
    fputws(L"sval: ", stream);
    fputws(exp->sval != 0 ? exp->sval : L"(null)", stream);
    fputws(L"\n", stream);
    indent(0, stream);
    fputws(L"child: [\n", stream);
    indent(1, stream);
    dump_exp(exp->child[0], stream);
    fputws(L",\n", stream);
    indent(0, stream);
    dump_exp(exp->child[1], stream);
    fputws(L"\n", stream);
    indent(-1, stream);
    fputws(L"]\n", stream);
    indent(-1, stream);
    fputws(L"}", stream);
}


/* pexp - print expression to stderr */

void pexp(const Exp *exp) {
	print_exp(exp, stderr);
	fputwc(L'\n', stderr);
}


/* pval - print value to stderr */

void pval(const Value *val) {
	print_value(val, stderr);
	fputwc(L'\n', stderr);
}

