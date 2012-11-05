#ifndef _EVAL_H_INCLUDED_
#define _EVAL_H_INCLUDED_

#include "types.h"

const Value *eval(const Exp *exp, Env *env);
const Value *apply(const Function *fn, const Value *arg);
const Value *force(const Value *val);
const Value *make_value(Object data, Type type);

#endif

