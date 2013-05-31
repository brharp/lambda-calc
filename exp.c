/*++
/* NAME
/*	exp 3
/* SUMMARY
/*	expressions
/* SYNOPSIS
/*	#include <exp.h>
/*
/*	const Exp     *make_symbol_exp(name);
/*	const wchar_t *name;
/*
/*	const Exp     *make_lambda_exp(param, body);
/*	const Exp     *param;
/*	const Exp     *body;
/*
/*	const Exp     *make_pair_exp(operator, operand);
/*	const Exp     *operator;
/*	const Exp     *operand;
/*
/*	const Exp     *make_quote_exp(exp);
/*	const Exp     *exp;
/*
/*	const Exp     *make_assign_exp(lhs, rhs);
/*	const Exp     *lhs;
/*	const Exp     *rhs;
/*--*/


#include "mystdlib.h"
#include "types.h"


/* make_symbol_exp - */

const Exp *make_symbol_exp(const wchar_t *name)
{
	Exp *exp = 0;

	exp = (Exp *)mymalloc(sizeof(*exp));
	exp->type = T_Exp_Symbol;
	exp->sval = _wcsdup(name);

	return exp;
}


/* make_lambda_exp - makes a lambda expression */

const Exp *make_lambda_exp(const Exp *param, const Exp *body)
{
	Exp *exp = 0;

	exp = mymalloc(sizeof(*exp));
	exp->type = T_Exp_Lambda;
	exp->child[0] = param;
	exp->child[1] = body;

	return exp;
}


/* make_pair - makes a pair expression */

const Exp *make_pair_exp(const Exp *operator, const Exp *operand)
{
	Exp *exp = 0;

	exp = mymalloc(sizeof(*exp));
	exp->type = T_Exp_Pair;
	exp->child[0] = operator;
	exp->child[1] = operand;

	return exp;
}


/* make_quote - makes a quote expression */

const Exp *make_quote_exp(const Exp *body)
{
	Exp *exp = 0;

	exp = mymalloc(sizeof *exp);
	exp->type = T_Exp_Quote;
	exp->child[0] = body;

	return exp;
}


/* make_assign - makes an assignment expression */

const Exp *make_assign_exp(const Exp *key, const Exp *value)
{
	Exp *exp = 0;

	exp = mymalloc(sizeof(*exp));
	exp->type = T_Exp_Assign;
	exp->child[0] = key;
	exp->child[1] = value;

	return exp;
}

