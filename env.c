/*++
/* NAME
/*	env 3
/* SUMMARY
/*	Environments bind names to values.
/* SYNOPSIS
/*  #include <env.h>
/*  
/*  const Value *lookup(const wchar_t * name, Env * env)
/*  Env *link(const wchar_t *name, const Value *value, Env *env)
/*  const Value *bind(const wchar_t *name, const Value *value, Env *env)
/* DESCRIPTION
/*  lookup() searches an environment for a named value. Names are
/*  compared by wcscmp(). If a match is found (wcscmp returns 0) then
/*  the value is returned. Otherwise, lookup() searches the next
/*  environment by following the environment's link pointer.
/*
/*  link() binds name to value in a new environment. If name is already
/*  bound in env, the new binding shadows the old binding, but does
/*  not replace it. That is, calling lookup() on the original environment
/*  returns the old value.
/*
/*  bind() binds name to value in the environment env. Any previous binding
/*  of name in the environment is destroyed.
/* RETURN VALUE
/*  lookup() returns a constant value if name is bound in the 
/*  environment, otherwise it returns 0;
/*
/*  link() returns a new environment, with name bound to value. The new
/*  environment inherits and shadows any bindings from env.
/*
/*  bind() returns the bound value.
/*
/*  get_global_environment() returns a reference to the global environment.
/*--*/

#include <assert.h>
#include <stdio.h>

#include "mystdlib.h"
#include "types.h"
#include "env.h"
#include "print.h"


 /* function prototypes */

static const Binding *get_binding(Env *, const wchar_t *);
static const Binding *make_binding(const wchar_t *, const Value *, const Binding *);
static const Binding *put_binding(Env *, const wchar_t *, const Value *);
static       Env     *make_env(Env *);


 /* structure definitions */

struct Binding {
	const wchar_t *name;
	const Value *value;
	const Binding *link;
};

struct Env {
	const Binding *bindings;
	Env *link;
};


/* make_binding - makes a new binding */

static const Binding *make_binding(const wchar_t *name,
                             const Value   *val,
							 const Binding *lnk)
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

static const Binding *get_binding(Env *env, const wchar_t *name)
{
	const Binding *bnd = UNBOUND;

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

static const Binding *put_binding(Env *env, const wchar_t *name, const Value *val)
{
	assert(env  != 0);
	assert(name != 0);
	assert(val  != 0);

	return env->bindings = make_binding(name, val, env->bindings);
}


/* print_locals - prints one level of bindings */

void print_locals(Env *env, FILE *stream)
{
	const Binding *bnd = 0;
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
	const Binding *bnd = 0;
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

	/* create new binding */
	put_binding(env, name, value);

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
	const Binding *bnd = UNBOUND;
	const Value   *val = 0;

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

