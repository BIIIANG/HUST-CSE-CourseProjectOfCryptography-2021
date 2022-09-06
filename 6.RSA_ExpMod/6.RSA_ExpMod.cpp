#pragma warning(disable:6031)
#pragma warning(disable:4996)
#pragma warning(disable:4146)
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>

void mpz_pow_SquMul_xba(mpz_ptr rop, mpz_srcptr base, mpz_srcptr exp, mpz_srcptr div)		// rop = base ^ exp (mod div)
{
	// init
	mpz_set_ui(rop, 1);
	char l_expArr[8192] = "\0";

	// square multiply
	mpz_get_str(l_expArr, 2, exp);
	for (int i = 0; l_expArr[i] != '\0'; i++) {
		mpz_mul(rop, rop, rop);
		mpz_mod(rop, rop, div);		// rop = rop ^ 2 mod div
		if (l_expArr[i] == '1') {
			mpz_mul(rop, rop, base);
			mpz_mod(rop, rop, div);		// if exp[i] = 1 then rop = rop * base mod div
		}
	}

#ifdef _DEBUG
	gmp_printf("mpz_pow_squareMul_xba:%Zd\n", rop);
#endif // _DEBUG

	return;
}

void mpz_pow_ModRptSqu_xba(mpz_ptr rop, mpz_srcptr base, mpz_srcptr exp, mpz_srcptr div)		// rop = base ^ exp (mod div)
{
	// init
	mpz_t a, b;
	mpz_init_set_ui(a, 1);
	mpz_init_set(b, base);
	char l_expArr[8192] = "\0";

	// modular repeated square
	mpz_get_str(l_expArr, 2, exp);
	for (int i = strlen(l_expArr) - 1; i > -1; i--)
	{
		if (l_expArr[i] == '1')
		{
			mpz_mul(a, a, b);
			mpz_mod(a, a, div);		// if exp[i] = 1 then a = a * b mod div
		}
		mpz_mul(b, b, b);
		mpz_mod(b, b, div);		// b = b ^ 2 mod div
	}
	mpz_set(rop, a);

#ifdef _DEBUG
	gmp_printf("mpz_pow_ModRptSqu_xba:%Zd\n", rop);
#endif // _DEBUG

	return;
}

void mpz_pow_slidingWindow_xba(mpz_ptr rop, mpz_srcptr base, mpz_srcptr exp, mpz_srcptr div)		// rop = base ^ exp (mod div)
{
	// init let k = 6
	int k = 6;
	mpz_t g[32], g_2;	// g[i] = g^(2*i+1)
	mpz_init_set(g[0], base);
	mpz_init(g_2);
	mpz_mul(g_2, base, base);
	mpz_mod(g_2, g_2, div);
	for (int i = 1; i < 32; i++) {
		mpz_init(g[i]);
		mpz_mul(g[i], g[i - 1], g_2);
		mpz_mod(g[i], g[i], div);
	}
	mpz_set_ui(rop, 1);
	char l_expArr[8192] = "\0";

	// repeat square
	mpz_get_str(l_expArr, 2, exp);
	for (int i = 0; l_expArr[i] != '\0';) {
		if (l_expArr[i] == '0') {
			mpz_mul(rop, rop, rop);	// rop = rop ^ 2 mod div
			mpz_mod(rop, rop, div);
			i++;
		}
		else {
			int l = k + i - 1, tempNum = 0;
			for (; l >= i; l--) {
				if (l_expArr[l] == '1') { break; }
			}
			for (int j = i; j <= l; j++) {
				tempNum <<= 1;
				tempNum += l_expArr[j] - '0';
			}
			for (int j = 0; j < l - i + 1; j++) {
				mpz_mul(rop, rop, rop);
				mpz_mod(rop, rop, div);
			}
			mpz_mul(rop, rop, g[(tempNum - 1) / 2]);
			mpz_mod(rop, rop, div);
			i = l + 1;
		}
	}

	// clear and free
	for (int i = 0; i < 32; i++) { mpz_clear(g[i]); }
	mpz_clear(g_2);

#ifdef _DEBUG
	gmp_printf("mpz_pow_slidingWindow_xba:%Zd\n", rop);
#endif // _DEBUG

	return;
}

int main()
{

#ifdef _DEBUG
	freopen("..\\Examples\\第六题样例\\1.in", "r", stdin);
#endif // _DEBUG

	//// test mpz_pow_ModRptSqu_xba, the result should be 7775
	//mpz_t rop, base, exp, mod;
	//mpz_init(rop);
	//mpz_init_set_ui(base, 12996);
	//mpz_init_set_ui(exp, 227);
	//mpz_init_set_ui(mod, 37909);
	//mpz_pow_ModRptSqu_xba(rop, base, exp, mod);

	int n = 0;
	scanf("%d", &n);
	getchar();

	mpz_t e, p, q, m, mul, rop;
	mpz_init(e);
	mpz_init(p);
	mpz_init(q);
	mpz_init(m);
	mpz_init(mul);
	mpz_init(rop);

	for (int i = 0; i < n; i++) {

		gmp_scanf("%Zd %Zd %Zd %Zd", e, m, p, q);
		mpz_mul(mul, p, q);
		mpz_pow_slidingWindow_xba(rop, m, e, mul);
		gmp_printf("%Zd\n", rop);

	}

	mpz_clear(e);
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(m);
	mpz_clear(mul);
	mpz_clear(rop);

	return 0;
}