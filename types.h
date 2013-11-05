#include <wchar.h>
#ifndef _TYPES_H_INCLUDED_
#define _TYPES_H_INCLUDED_
/*++
/* NAME
/*	types 3h
/* SUMMARY
/*	Types and objects.
/* SYNOPSIS
/*	#include <types.h>
/* DESCRIPTION
/* .nf

 /* constants */

#define UNBOUND (0)


 /* booleans */

#if !defined(_WIN32)
#include <stdbool.h>
#elif !defined(__BOOL_DEFINED)
typedef int bool;
#define false 0
#define true !false
#endif


 /* enumeration of expression types */

enum Exp_Type {
	T_Exp_Symbol,
	T_Exp_Lambda,
	T_Exp_Pair,
	T_Exp_Quote,
	T_Exp_Assign,
	T_Exp_Seq
};


 /* enumeration of value types */

enum Type {
	T_Exp,
	T_Function,
	T_Thunk,
	T_Env
};


 /* type definitions */

typedef enum   Type     Type;
typedef enum   Exp_Type Exp_Type;
typedef union  Object   Object;
typedef struct Env      Env;		/* environments */
typedef struct Binding  Binding;
typedef struct Function Function;
typedef struct Exp      Exp;		/* expressions */
typedef struct Value    Value;
typedef struct Thunk    Thunk;


 /* Procedure - funcation call procedure */

typedef const Value *(*Procedure)(const Function *, const Value *);

 /* object - a union of all internal types */

union Object {
	const Exp *exp;
	Function  *function;
	Thunk     *thunk;
	Env       *env;
	void      *und;
};


 /* value - a tagged datum */

struct Value {
	Type   type;
	Object data;
};


 /* Exp - an expression */

struct Exp {
	Exp_Type type;
	const wchar_t *sval;
	const Exp *child[2];
	int nval;
};


 /* Function - a function object */

struct Function {
	const wchar_t *name;
	const Exp *param;
	const Exp *body;
	Env *env;
	Procedure apply;
};


 /* Thunk - a parameterless function */

struct Thunk {
	const Value *value;
	const Exp *exp;
	Env *env;
};


/* AUTHOR
/*	Brent Harp
/*
/*--*/
#endif
