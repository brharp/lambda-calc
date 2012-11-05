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

#include <stdio.h>
#include <assert.h>


 /* Local includes */

#include "types.h"
#include "print.h"
#include "eval.h"
#include "env.h"


 /* Constants */

const wint_t lambda    = L'\\';
const wint_t separator = L'.';
const wint_t lparen    = L'(';
const wint_t rparen    = L')';
const wint_t quote     = L'\'';
const wint_t space     = L' ';
const wint_t assign    = L'=';


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
		fputws(L"#<Function ", stream);
		if (fn->name != 0) {
			fputws(fn->name, stream);
			fputws(L" ", stream);
		}
		fputws(L"\\", stream);
		print_exp(fn->param, stream);
		fputwc(separator, stream);
		print_exp(fn->body, stream);
		/* print environment
		fputwc(space, stream);
		fputwc(L'{', stream);
		print_env(fn->env, stream);
		fputwc(L'}', stream); */
		fputws(L">", stream);
		break;

	case T_Exp:
		print_exp(val->data.exp, stream);
		break;

	case T_Thunk:
		print_value(force(val), stream);
		break;

	default:
		fputws(L"#<?>", stream);
		break;
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
    static int     indent = 0;
    static wchar_t *space = L"\t\t\t\t\t\t\t\t\t\t\t\t";
    
    indent += delta;
    fwprintf(stream, L"%.*ls", indent, space);
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
