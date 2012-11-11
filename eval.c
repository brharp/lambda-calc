/*++
/* NAME
/*	eval 3
/* SUMMARY
/*	Evaluation and compilation.
/* SYNOPSIS
/*	#include <eval.h>
/*
/*	const Value *eval(const Exp *exp, Env *env);
/*
/*	const Value *apply(Function *fun, const Value *arg);
/*
/*	const Value *promise(const Exp *exp, Env *env);
/*
/*	const Value *force(const Value *val);
/*
/*	const Exp *expand(const Exp *exp, Env *env);
/* DESCRIPTION
/*	This module evaluates expressions in the lambda calculus.
/*
/*	The primary interface to the evaluator is eval(). It evaluates
/*	an expression in a given environment and returns the result.
/*
/*	The evaluator is lazy. If possible the evaluator will delay evaluation
/*	of an expression, promising to evaluate the expression later if
/*	required. The function promise() takes an expression and an
/*	environment, and creates a value of type Thunk. A thunk is a function
/*	of no arguments: when evaluation is forced (by calling force()), the
/*	thunk is replaced by the result of evaluating the saved expression in
/*	the saved environment. The result of calling the thunk is saved, so
/*	calling force() again will return the same value.
/*
/*	expand() returns a fully expanded form of an expression.
/*--*/

#include <assert.h>
#include <stdio.h>

#include "mystdlib.h"
#include "types.h"
#include "read.h"
#include "eval.h"
#include "env.h"


/* function prototypes */

static const Value *make_thunk(const Exp *exp, Env *env);

static const Value *make_function(const Exp *param, const Exp *body,
		Env *env);

const Value *promise(const Exp *exp, Env *env);


/* eval - evaluate an expression */

const Value *eval(const Exp * exp, Env * env)
{
	const Value *op = 0, *lhs = 0, *rhs = 0, *val = 0;
	static int indent = 0;
	int i = 0;

	assert(exp != 0);
	assert(env != 0);

	switch (exp->type) {
	case T_Exp_Symbol:
		val = lookup(exp->sval, env);
		break;

	case T_Exp_Lambda:
		assert(exp->child[0] != 0);
		assert(exp->child[1] != 0);
		assert(exp->child[0]->type == T_Exp_Symbol);
		val = make_function(exp->child[0], exp->child[1], env);
		break;

	case T_Exp_Pair:
		op = force(eval(exp->child[0], env));
		assert(op->type == T_Function);
		val = apply(op->data.function, promise(exp->child[1], env));
		break;

	case T_Exp_Quote:
		val = make_value((Object) (exp->child[0]), T_Exp);
		break;

	case T_Exp_Assign:
		assert(exp->child[0] != 0);
		assert(exp->child[1] != 0);
		assert(exp->child[0]->type == T_Exp_Symbol);
		assert(exp->child[0]->sval != 0);
		val = bind(exp->child[0]->sval, eval(exp->child[1], env), env);
		break;

	default:
		fwprintf(stderr, L"%s: %d: %s: illegal expression type\n",
			__FILE__, __LINE__, "eval");
		exit(EXIT_FAILURE);
	}
	
	return val;
}


/* apply - apply a function to an argument */

const Value *apply(const Function *fun, const Value *arg)
{
	assert(fun != 0);
	assert(arg != 0);

	return eval(fun->body, link(fun->param->sval, arg, fun->env));
}


/* promise - delayed evaluation */

const Value *promise(const Exp *exp, Env *env)
{
	return make_thunk(exp, env);
}


/* force - force a delayed evaluation */

const Value *force(const Value *val)
{
	Thunk *thk = 0;

	assert(val != 0);
	while (val->type == T_Thunk) {
		thk = val->data.thunk;
		if (thk->value == 0) {
			thk->value = eval(thk->exp, thk->env);
		}
		val = thk->value;
		assert(val != 0);
	}

	return val;
}


/* make_function - makes a function object */

static const Value *make_function(const Exp *param, const Exp *body,
				  Env *env)
{
	Function *fn = 0;

	assert(param->type == T_Exp_Symbol);

	fn = mymalloc(sizeof(*fn));
	fn->name  = 0;
	fn->param = param;
	fn->body  = body;
	fn->env   = env;

	return make_value((Object) fn, T_Function);
}


/* make_value - makes a value */

const Value *make_value(Object data, Type type)
{
	Value *val = 0;

	val = mymalloc(sizeof(*val));
	val->type = type;
	val->data = data;

	return val;
}


/* make_thunk - makes a thunk */

static const Value *make_thunk(const Exp *exp, Env *env)
{
	Thunk *thk = 0;

	thk = mymalloc(sizeof(*thk));
	thk->value = 0;
	thk->exp   = exp;
	thk->env   = env;

	return make_value((Object) thk, T_Thunk);
}


/* print_thunk - print a thunk */

void print_thunk(const Thunk *thk, FILE *stream)
{
	fputws(L"(promise ", stream);
	fwprintf(stream, L"<Thunk %p value=%p, exp=%p>",
		thk, thk->value, thk->exp, thk->env);
}


/* main - program entry */

int main(int argc, char *argv[])
{
	Env *gbl = get_global_environment();

	for (;;) {
		const Exp *exp = 0;
		while (exp == 0) {
			exp = read_exp_list_delim(L'\n', stdin);
		}
		fputws(L";; ", stderr);
		print_exp(exp, stderr);
		fputwc(L'\n', stderr);
		print_value(force(eval(exp, gbl)), stdout);
		fputwc(L'\n', stdout);
	}

	return 0;
}
