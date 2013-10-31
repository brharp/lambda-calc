
#include "num.h"

const Exp *church_encode(const int n)
{
	const Exp *f = make_symbol_exp(L"f");
	const Exp *x = make_symbol_exp(L"x");

	return make_lambda_exp(f, make_lambda_exp(x,
		n == 0 ? x : make_pair_exp(f, church_encode(n - 1))));
}