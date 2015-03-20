
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


typedef void(*Opcode)(void);

/* internal registers */

Opcode *ip;	/* instruction pointer */
int *cp;	/* compiler pointer */
int *sp;	/* stack pointer */


/* memory */

int m[16384];



/* execute - execute a compiled function */

const Value *execute(const Function *fun, const Value *arg)
{
	assert(fun != 0);
	assert(arg != 0);

	*--sp = arg;	/* push arg on the stack */

	/*
	 * execute opcodes
	 */
	ip = fun->code;
	while (*ip != 0) {
		(*ip++)();
	}

	return *sp++;	/* pop return value */
}


/* call - call a function (with argument) */

void call(void)
{
	const Function *fun;
	const Value *arg, *val;

	/* 
	 * pop function and argument 
	 */
	fun = *sp++;
	arg = *sp++;

	/* apply function */
	val = fun->apply(fun, arg);

	/* push result */
	*--sp = val;
}


/* variable - fetches the value of a variable */

void variable(void)
{
	assert(sp > cp);

	*--sp = *ip++;
}


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
		*cp++ = variable;
		*cp++ = lookup(exp->sval, env);
		break;

	case T_Exp_Lambda:
		assert(exp->child[0] != 0);
		assert(exp->child[1] != 0);
		assert(exp->child[0]->type == T_Exp_Symbol);
		val = make_function(exp->child[0], exp->child[1], env);
		break;

	case T_Exp_Pair:
		compile(exp->child[1], env);
		compile(exp->child[0], env);
		*cp++ = call;
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
