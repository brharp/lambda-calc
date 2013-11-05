#ifndef _EXP_H_INCLUDED_
#define _EXP_H_INCLUDED_

const Exp *make_lambda_exp(const Exp *, const Exp *);
const Exp *make_pair_exp(const Exp *, const Exp *);
const Exp *make_quote_exp(const Exp *);
const Exp *make_assign_exp(const Exp *, const Exp *);
const Exp *make_symbol_exp(const wchar_t *);
const Exp *make_seq_exp(const Exp *, const Exp *);

#endif
