#pragma warning(disable:6031)
#pragma warning(disable:4996)
#pragma warning(disable:4146)
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>

void mpz_gcd_xba(mpz_ptr rop, mpz_srcptr op1, mpz_srcptr op2)	// rop = (op1, op2)
{
	// init
	mpz_t dividend, divisor, remainder;	// dividend = divisor * quotient + remainder
	mpz_init(dividend);
	mpz_init(divisor);
	mpz_init(remainder);
	if (mpz_cmp(op1, op2) > 0) {
		mpz_init_set(dividend, op1);
		mpz_init_set(divisor, op2);
	}
	else {
		mpz_init_set(divisor, op1);
		mpz_init_set(dividend, op2);
	}

	// calculate the gcd
	mpz_mod(remainder, dividend, divisor);
	while (mpz_cmp_ui(remainder, 0) != 0) {
		mpz_set(dividend, divisor);
		mpz_set(divisor, remainder);
		mpz_mod(remainder, dividend, divisor);
	}

	// clear and free
	mpz_set(rop, divisor);
	mpz_clear(dividend);
	mpz_clear(divisor);
	mpz_clear(remainder);
	return;
}

void mpz_inv_xba(mpz_ptr rop, mpz_srcptr op, mpz_srcptr div)	// rop = op ^ (-1) (mod div)	(equal to mpz_invert)
{
	// init
	mpz_t l_rop, l_op, l_div;
	mpz_init(l_rop);
	mpz_init_set(l_op, op);
	mpz_init_set(l_div, div);
	mpz_t dividend, divisor, remainder;	// dividend = divisor * quotient + remainder
	mpz_t* S = (mpz_t*)malloc(2048 * sizeof(mpz_t));
	mpz_t* q = (mpz_t*)malloc(2048 * sizeof(mpz_t));	// q => quotient
	if (!S || !q) { return; }
	mpz_mod(l_op, l_op, l_div);
	mpz_init_set(dividend, l_div);
	mpz_init_set(divisor, l_op);
	mpz_init(remainder);
	for (int j = 0; j < 2048; j++) {
		mpz_init(S[j]);
		mpz_init(q[j]);
	}

	// get the quotient array
	int j = 0;
	do {
		mpz_fdiv_qr(q[j], remainder, dividend, divisor);	// cdiv=>ceil  fdiv=>floor
		mpz_set(dividend, divisor);
		mpz_set(divisor, remainder);
		j++;
	} while (mpz_cmp_si(remainder, 0));

	// calculate the S array
	mpz_set_ui(S[0], 0);
	mpz_set_ui(S[1], 1);
	mpz_t mul;
	mpz_init(mul);
	for (int k = 2; k <= j; k++) {
		mpz_mul(mul, S[k - 1], q[j - k]);
		mpz_sub(S[k], S[k - 2], mul);
	}
	mpz_mod(l_rop, S[j], l_div);
	mpz_set(rop, l_rop);

	// clear and free
	mpz_clear(l_rop);
	mpz_clear(l_op);
	mpz_clear(l_div);
	mpz_clear(dividend);
	mpz_clear(divisor);
	mpz_clear(remainder);
	mpz_clear(mul);
	for (int j = 0; j < 2048; j++) {
		mpz_clear(S[j]);
		mpz_clear(q[j]);
	}
	free(S);
	free(q);

#ifdef _DEBUG
	gmp_printf("mpz_inv_xba:%Zd\n", rop);
#endif // _DEBUG

	return;
}


int main()
{

#ifdef _DEBUG
	freopen("..\\Examples\\第五题样例\\1.in", "r", stdin);
#endif // _DEBUG


	int n = 0;
	scanf("%d", &n);
	getchar();

	mpz_t e, p, q;
	mpz_init(e);
	mpz_init(p);
	mpz_init(q);

	for (int i = 0; i < n; i++) {

		gmp_scanf("%Zd %Zd %Zd", e, p, q);

		// 1. p or q is not a prime
		if (!mpz_probab_prime_p(p, 10) || !mpz_probab_prime_p(q, 10)) {
			printf("ERROR\n");
			continue;
		}

		// 2. p or q is too small
		if ((mpz_cmp_ui(p, 10) < 0) || (mpz_cmp_ui(q, 10) < 0)) {
			printf("ERROR\n");
			continue;
		}

		// 3. e is too small
		if (mpz_cmp_ui(e, 10) < 0) {
			printf("ERROR\n");
			continue;
		}

		// 4. e and phi is not coprime
		mpz_t rop, phi;
		mpz_init(rop);
		mpz_init(phi);
		mpz_sub_ui(p, p, 1);
		mpz_sub_ui(q, q, 1);
		mpz_mul(phi, p, q);
		mpz_gcd_xba(rop, e, phi);
		if (mpz_cmp_ui(rop, 1) != 0) {
			printf("ERROR\n");
			continue;
		}

		// 5. gcd(p - 1, q - 1) must be small
		mpz_gcd_xba(rop, p, q);
		if (mpz_cmp_ui(rop, 10000) > 0) {
			printf("ERROR\n");
			continue;
		}

		// 6. abs(p - q) must be big
		mpz_sub(rop, p, q);
		mpz_abs(rop, rop);
		if (mpz_cmp_ui(rop, 10000) < 0) {
			printf("ERROR\n");
			continue;
		}

		// calculate d by Euclidean Algorithm
		mpz_t d;
		mpz_init(d);
		mpz_inv_xba(d, e, phi);
		gmp_printf("%Zd\n", d);
		mpz_clear(d);
	}
	
	mpz_clear(e);
	mpz_clear(p);
	mpz_clear(q);

	return 0;
}