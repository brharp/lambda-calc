/*++
/* NAME
/*	compile 3
/* SUMMARY
/*	Compilation.
/* SYNOPSIS
/*	#include <eval.h>
/*
/*	const Value *compile(const Exp *exp, Env *env);
/*
/* DESCRIPTION
/*	This module compiles expressions in the lambda calculus.
/*
/*	The primary interface to the compiler is compiler(). It evaluates
/*	an expression in a given environment and returns the result.
/*
/*--*/

#include <assert.h>
#include <stdio.h>

#include "mystdlib.h"
#include "types.h"
#include "read.h"
#include "eval.h"
#include "char.h"
#include "env.h"
#include "exp.h"
#include "print.h"
#include "num.h"

/* compile - compile an expression */

const Value *compile(const Exp * exp, Env * env)
{
	const Value *op = 0, *lhs = 0, *rhs = 0, *val = 0;
	const Function *fn;
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
		compile(exp->child[1], env);	/* push parameter */
		/* execute function body */
		loc++ = call;
		loc++ = compile(exp->child[0], env);
		fn = (Function *)the(T_Function, op);
		val = fn->apply(fn, promise(exp->child[1], env));
		break;

	case T_Exp_Quote:
		val = make_exp_value(exp->child[0]);
		break;

	case T_Exp_Assign:
		assert(exp->child[0] != 0);
		assert(exp->child[1] != 0);
		assert(exp->child[0]->type == T_Exp_Symbol);
		assert(exp->child[0]->sval != 0);
		val = bind(exp->child[0]->sval, eval(exp->child[1], env), env);
		break;

	case T_Exp_Seq:
		assert(exp->child[0] != 0);
		assert(exp->child[1] != 0);
		force(eval(exp->child[0], env));
		val = force(eval(exp->child[1], env));
		break;

	case T_Exp_Num:
		val = eval(church_encode(exp->nval), env);
		break;

	default:
		fwprintf(stderr, L"%s: %d: %s: illegal expression type\n",
			__FILE__, __LINE__, "eval");
		exit(EXIT_FAILURE);
	}

	return val;
}


const Value *pop()
{

}

static Value *code;
static Env *env;
static int pc;

const Value *execute(const Function *fun, const Env *env)
{
	int ip;	/* instruction pointer */

	for (ip = 0; ip < codelen; ip++) {
		fun->code[ip](&fun->code[ip]);
	}
}


const Value *call()
{
	const Value *fun;

	fun = pop();
	assert(fun != 0);
	assert(fun->type == T_Function);
	code = fun->data.function->code;
	assert(code != 0);
	env = fun->data.env;
	pc = 0;
}


/* push variable onto stack */

void variable()
{
	int i, j;

}


void ret()
{
	/* Pop return address. */
	ip = fun->code;
	ip(ip[1]);

	return eval(fun->body, link(fun->param->sval, arg, fun->env));
}


const Value *loopupc(const Value *symbol)
{
	++sp = lookup(symbol->data.exp->sval, ep);
}
