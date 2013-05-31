#ifndef _EVAL_H_INCLUDED_
#define _EVAL_H_INCLUDED_

#include "types.h"

const Value *eval(const Exp *exp, Env *env);
const Value *apply(const Function *fn, const Value *arg);
const Value *force(const Value *val);
const Value *make_value(Object data, Type type);
const Value *make_function_value(Function *fn);
const Value *make_exp_value(const Exp *exp);
const Value *make_thunk_value(Thunk *thk);

#endif

