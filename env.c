/*++
/* NAME
/*	env 3
/* SUMMARY
/*	Environments.
/* 
/*--*/

#include <assert.h>
#include <stdio.h>

#include "mystdlib.h"
#include "types.h"
#include "env.h"
#include "print.h"


 /* function prototypes */

static Binding *make_binding(const wchar_t * name, const Value * value,
                             Binding * lnk);

static Env *make_env(Env * link);


 /* structure definitions */

struct Binding {
	const wchar_t *name;
	const Value *value;
	Binding *link;
};

struct Env {
	Binding *bindings;
	Env *link;
};


static Binding *make_binding(const wchar_t * name,
                             const Value * val, Binding * lnk)
{
	Binding *bnd = 0;

	assert(name != 0);
	assert(val  != 0);

	bnd = (Binding *)mymalloc(sizeof(*bnd));
	bnd->name  = name;
	bnd->value = val;
	bnd->link  = lnk;

	return bnd;
}


/* make_env - makes a new environment */

static Env *make_env(Env *link)
{
	Env *env = 0;

	env = mymalloc(sizeof(*env));
	env->bindings = UNBOUND;
	env->link     = link;

	return env;
}


/* get_binding - returns the binding of name in env. If name is not
   bound, inserts name into environment as an unbound symbol. */

Binding *get_binding(Env *env, const wchar_t *name)
{
	Binding *bnd = UNBOUND;

	assert(env  != 0);
	assert(name != 0);

	for (; env != 0; env = env->link) {
		for (bnd = env->bindings; bnd != UNBOUND; bnd = bnd->link) {
			if (wcscmp(bnd->name, name) == 0) {
				goto found;
			}
		}
	}
found:
	return bnd;
}


/* put_binding - inserts a new binding into an environment */

Binding *put_binding(Env *env, const wchar_t *name, const Value *val)
{
	assert(env  != 0);
	assert(name != 0);
	assert(val  != 0);

	return env->bindings = make_binding(name, val, env->bindings);
}


/* print_locals - prints one level of bindings */

void print_locals(Env *env, FILE *stream)
{
	Binding *bnd = 0;
	static bool rec = false;

	assert(env != 0);
	assert(stream != 0);

	if (rec == true) {
		fputws(L"...", stream);
		return;
	}

	rec = true;

	for (bnd = env->bindings; bnd != UNBOUND; bnd = bnd->link) {
		fputws(bnd->name, stream);
		fputwc(L'=', stream);
		print_value(bnd->value, stream);
		if (bnd->link != 0) {
			fputwc(L',', stream);
		}
	}

	rec = false;
}


/* print_env - prints an environment */

void print_env(Env *env, FILE *stream)
{
	Binding *bnd = 0;
	static bool rec = 0;

	if (rec == 1) {
		fputws(L"...", stream);
		return;
	}

	rec = 1;

	fputws(L"#<Env", stream);
	for (; env != 0; env = env->link) {
		for (bnd = env->bindings; bnd != UNBOUND; bnd = bnd->link) {
			fputwc(L' ', stream);
			fputws(bnd->name, stream);
			fputwc(L'=', stream);
			print_value(bnd->value, stream);
			if (bnd->link != 0) {
				fputwc(L',', stream);
			}
		}
	}

	rec = 0;

	fputwc(L'>', stream);
}


/* bind - binds name to value in an environment */

const Value *bind(const wchar_t *name, const Value *value, Env *env)
{
	Binding *bnd = 0;

	assert(env   != 0);
	assert(name  != 0);
	assert(value != 0);

	if (value->type == T_Function) {
		value->data.function->name = name;
	}

	if ((bnd = get_binding(env, name)) == UNBOUND) {
		/* create new binding */
		put_binding(env, name, value);
	} else {
		/* change existing binding */
		bnd->value = value;
	}

	return value;
}


/* link -  Binds name to value in a new environment. Returns the new
	environment. */

Env *link(const wchar_t *name, const Value *value, Env *env)
{
	Env *new_env = 0;

	/* we use put_binding() here and not bind(), because we 
	 * want to shadow any existing binding of `name'. */

	new_env = make_env(env);
	put_binding(new_env, name, value);

	return new_env;
}


/* lookup - Looks up a value by name in an environment.  */

const Value *lookup(const wchar_t * name, Env * env)
{
	Binding     *bnd = UNBOUND;
	const Value *val = 0;

	if ((bnd = get_binding(env, name)) != UNBOUND) {
		val = bnd->value;
	}

	assert(val != 0);

	return val;
}

static Env *global = 0;

Env *get_global_environment()
{
    if (global == 0) {
	global = make_env(0);
    }

    return global;
}

