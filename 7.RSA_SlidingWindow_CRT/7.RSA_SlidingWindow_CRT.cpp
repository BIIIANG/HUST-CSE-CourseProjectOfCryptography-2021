#pragma warning(disable:6031)
#pragma warning(disable:4996)
#pragma warning(disable:4146)
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

void mpz_pow_squMul_xba(mpz_ptr rop, mpz_srcptr base, mpz_srcptr exp, mpz_srcptr div)		// rop = base ^ exp (mod div)
{
	// init
	mpz_set_ui(rop, 1);
	char l_expArr[8192] = "\0";

	// repeat square
	mpz_get_str(l_expArr, 2, exp);
	for (int i = 0; l_expArr[i] != '\0'; i++) {
		mpz_mul(rop, rop, rop);	// rop = rop ^ 2 mod div
		mpz_mod(rop, rop, div);
		if (l_expArr[i] == '1') {	// if exp[i] = 1 then rop = rop * base mod div
			mpz_mul(rop, rop, base);
			mpz_mod(rop, rop, div);
		}
	}

#ifdef _DEBUG
	gmp_printf("mpz_pow_squareMul_xba:%Zd\n", rop);
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

void mpz_CRT_xba(mpz_ptr rop, mpz_srcptr num1, mpz_srcptr num2, mpz_srcptr div1, mpz_srcptr div2)	// rop = num_1 (mod div_1) && rop = num_2 (mod div_2)
{
	// init
	mpz_t l_rop, l_num1, l_num2, l_div1, l_div2, l_inv1, l_inv2, l_temp1, l_temp2, l_mul;
	mpz_init(l_rop);
	mpz_init_set(l_num1, num1);
	mpz_init_set(l_num2, num2);
	mpz_init_set(l_div1, div1);
	mpz_init_set(l_div2, div2);
	mpz_init(l_inv1);
	mpz_init(l_inv2);
	mpz_init(l_temp1);
	mpz_init(l_temp2);
	mpz_init(l_mul);

	// calculate the rop <- [num1 * div2 * (div2 ^ (-1) mod div1) + num2 * div1 * (div1 ^ (-1) mod div2)] mod (div1 * div2)
	mpz_inv_xba(l_inv1, l_div2, l_div1);
	mpz_inv_xba(l_inv2, l_div1, l_div2);
	mpz_mul(l_temp1, l_num1, l_div2);
	mpz_mul(l_temp1, l_temp1, l_inv1);
	mpz_mul(l_temp2, l_num2, l_div1);
	mpz_mul(l_temp2, l_temp2, l_inv2);
	mpz_add(l_rop, l_temp1, l_temp2);
	mpz_mul(l_mul, l_div1, l_div2);
	mpz_add(l_rop, l_temp1, l_temp2);
	mpz_mod(l_rop, l_rop, l_mul);
	mpz_set(rop, l_rop);

	// clear and free
	mpz_clear(l_rop);
	mpz_clear(l_num1);
	mpz_clear(l_num2);
	mpz_clear(l_div1);
	mpz_clear(l_div2);
	mpz_clear(l_inv1);
	mpz_clear(l_inv2);
	mpz_clear(l_temp1);
	mpz_clear(l_temp2);
	mpz_clear(l_mul);

#ifdef _DEBUG
	gmp_printf("mpz_CRT_xba:%Zd\n", rop);
#endif // _DEBUG

	return;
}

void mpz_CRT_inv_xba(mpz_ptr rop, mpz_srcptr num1, mpz_srcptr num2, mpz_srcptr div1, mpz_srcptr div2, mpz_srcptr inv1, mpz_srcptr inv2)	// rop = num_1 (mod div_1) && rop = num_2 (mod div_2)
{
	// init
	mpz_t l_num1, l_num2, l_mul;
	mpz_init_set(l_num1, num1);
	mpz_init_set(l_num2, num2);
	mpz_init(l_mul);

	// calculate the rop <- [num1 * div2 * (div2 ^ (-1) mod div1) + num2 * div1 * (div1 ^ (-1) mod div2)] mod (div1 * div2)
	mpz_mul(l_num1, l_num1, div2);
	mpz_mul(l_num1, l_num1, inv1);
	mpz_mul(l_num2, l_num2, div1);
	mpz_mul(l_num2, l_num2, inv2);
	mpz_add(rop, l_num1, l_num2);
	mpz_mul(l_mul, div1, div2);
	mpz_mod(rop, rop, l_mul);

	// clear and free
	mpz_clear(l_num1);
	mpz_clear(l_num2);
	mpz_clear(l_mul);

#ifdef _DEBUG
	gmp_printf("mpz_CRT_xba:%Zd\n", rop);
#endif // _DEBUG

	return;
}

int main()
{

#ifdef _DEBUG
	freopen("..\\Examples\\第七题样例\\1.in", "r", stdin);
#endif // _DEBUG

	//// test mpz_pow_squareMul_xba, the result should be 5761
	//mpz_t rop, base, exp, mod;
	//mpz_init(rop);
	//mpz_init_set_ui(base, 9726);
	//mpz_init_set_ui(exp, 3533);
	//mpz_init_set_ui(mod, 11413);
	////mpz_pow_squareMul_xba(rop, base, exp, mod);
	//mpz_pow_slidingWindow_xba(rop, base, exp, mod);
	
	//// test mpz_inv_xba, the result should be 10
	//mpz_t rop, op, mod;
	//mpz_init(rop);
	//mpz_init_set_ui(op, 4);
	//mpz_init_set_ui(mod, 13);
	//mpz_inv_xba(rop, op, mod);

	//// test mpz_CRT_xba, the result should be 23
	//mpz_t rop, num1, num2, div1, div2;
	//mpz_init(rop);
	//mpz_init_set_ui(num1, 1);
	//mpz_init_set_ui(num2, 2);
	//mpz_init_set_ui(div1, 11);
	//mpz_init_set_ui(div2, 3);
	//mpz_CRT_xba(rop, num1, num2, div1, div2);

	int n = 0;
	scanf("%d", &n);
	mpz_t e, p, q, c, d, phi, rop, num1, num2, temp1, temp2, inv1, inv2, mul;
	mpz_init(e);
	mpz_init(p);
	mpz_init(q);
	mpz_init(c);
	mpz_init(d);
	mpz_init(phi);
	mpz_init(rop);
	mpz_init(num1);
	mpz_init(num2);
	mpz_init(temp1);
	mpz_init(temp2);
	mpz_init(inv1);
	mpz_init(inv2);
	mpz_init(mul);
	gmp_scanf("%Zd %Zd %Zd", p, q, e);
	mpz_sub_ui(temp1, p, 1);
	mpz_sub_ui(temp2, q, 1);
	mpz_mul(phi, temp1, temp2);		// phi = (p-1)(q-1)
	mpz_inv_xba(d, e, phi);		// d = e ^ -1 mod (p-1)(q-1)
	mpz_mod(temp1, d, temp1);	// temp1 = d mod phi(p)
	mpz_mod(temp2, d, temp2);	// temp2 = d mod phi(q)
	mpz_inv_xba(inv1, q, p);	// inv1 = q ^ -1 mod p
	mpz_inv_xba(inv2, p, q);	// inv2 = p ^ -1 mod q
	mpz_mul(mul, p, q);		// mul = pq


	for (int i = 0; i < n; i++) {
		gmp_scanf("%Zd", c);

		//mpz_pow_squMul_xba(num1, c, temp1, p);	// num1 = c ^ (d mod phi(p)) mod p
		//mpz_pow_squMul_xba(num2, c, temp2, q);	// num2 = c ^ (d mod phi(q)) mod q
		mpz_pow_slidingWindow_xba(num1, c, temp1, p);
		mpz_pow_slidingWindow_xba(num2, c, temp2, q);

		mpz_CRT_inv_xba(rop, num1, num2, p, q, inv1, inv2);

		gmp_printf("%Zd\n", rop);
	}


	mpz_clear(e);
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(c);
	mpz_clear(d);
	mpz_clear(phi);
	mpz_clear(rop);
	mpz_clear(num1);
	mpz_clear(num2);
	mpz_clear(temp1);
	mpz_clear(temp2);
	mpz_clear(inv1);
	mpz_clear(inv2);
	mpz_clear(mul);

	return 0;
}