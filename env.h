#ifndef _ENV_H_INCLUDED_
#define _ENV_H_INCLUDED_
/*++
/* NAME
/*	env 3h
/* SUMMARY
/*	Environments.
/* DESCRIPTION
/* .nf
/*

 /* Local includes */

#include "types.h"


 /* Function prototypes */

const Value *lookup(const wchar_t *name, Env * env);
Env *link(const wchar_t *name, const Value *value, Env *env);
Env *unlink(Env *env);
const Value *bind(const wchar_t *name, const Value *value, Env *env);
Env *get_global_environment();
void print_env(Env * env, FILE *stream);
void print_locals(Env * env, FILE *stream);

/* AUTHOR
/*	Brent Harp
/*--*/
#endif
