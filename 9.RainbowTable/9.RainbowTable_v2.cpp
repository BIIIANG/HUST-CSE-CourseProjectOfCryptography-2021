#pragma warning(disable:6031)
#pragma warning(disable:4996)
#pragma warning(disable:4146)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define N 10000
#define MAXCHAIN 10000

unsigned int SHA1_tmp;

#define SHA1_ROTL(a,b) (SHA1_tmp=(a),((SHA1_tmp>>(32-b))&(0x7fffffff>>(31-b)))|(SHA1_tmp<<b))

#define SHA1_F(B,C,D,t) ((t<40)?((t<20)?((B&C)|((~B)&D)):(B^C^D)):((t<60)?((B&C)|(B&D)|(C&D)):(B^C^D)))

int UnitSHA1(const char* str, int length, unsigned sha1[5])
{
	/*
	计算字符串SHA-1
	参数说明：
	str         字符串指针
	length      字符串长度
	sha1         用于保存SHA-1的字符串指针
	返回值为参数sha1
	*/
	unsigned char* pp, * ppend;
	unsigned int l, i, K[80], W[80], TEMP, A, B, C, D, E, H0, H1, H2, H3, H4;
	H0 = 0x67452301, H1 = 0xEFCDAB89, H2 = 0x98BADCFE, H3 = 0x10325476, H4 = 0xC3D2E1F0;
	for (i = 0; i < 20; K[i++] = 0x5A827999);
	for (i = 20; i < 40; K[i++] = 0x6ED9EBA1);
	for (i = 40; i < 60; K[i++] = 0x8F1BBCDC);
	for (i = 60; i < 80; K[i++] = 0xCA62C1D6);
	l = length + ((length % 64 > 56) ? (128 - length % 64) : (64 - length % 64));
	if (!(pp = (unsigned char*)malloc((unsigned int)l))) return -1;
	for (i = 0; i < length; pp[i + 3 - 2 * (i % 4)] = str[i], i++);
	for (pp[i + 3 - 2 * (i % 4)] = 128, i++; i < l; pp[i + 3 - 2 * (i % 4)] = 0, i++);
	*((unsigned int*)(pp + l - 4)) = length << 3;
	*((unsigned int*)(pp + l - 8)) = length >> 29;
	for (ppend = pp + l; pp < ppend; pp += 64) {
		for (i = 0; i < 16; W[i] = ((unsigned int*)pp)[i], i++);
		for (i = 16; i < 80; W[i] = SHA1_ROTL((W[i - 3] ^ W[i - 8] ^ W[i - 14] ^ W[i - 16]), 1), i++);
		A = H0, B = H1, C = H2, D = H3, E = H4;
		for (i = 0; i < 80; i++) {
			TEMP = SHA1_ROTL(A, 5) + SHA1_F(B, C, D, i) + E + W[i] + K[i];
			E = D, D = C, C = SHA1_ROTL(B, 30), B = A, A = TEMP;
		}
		H0 += A, H1 += B, H2 += C, H3 += D, H4 += E;
	}
	free(pp - l);
	sha1[0] = H0, sha1[1] = H1, sha1[2] = H2, sha1[3] = H3, sha1[4] = H4;
	return 0;
}

void getstr(unsigned n, char str[8])
{
	str[0] = 'a'; str[1] = '0'; str[2] = '0'; str[3] = '0'; str[4] = '0'; str[5] = '0'; str[6] = '0'; str[7] = '0';
	int i = 2;
	while (n)
	{
		unsigned tmp = n % 36;
		if (tmp < 10)
			str[i++] = tmp + '0';
		else
		{
			str[i++] = tmp - 10 + 'a';
		}
		n = n / 36;
	}
}

void R(unsigned sha1[5], char str[8], int i)
{
	getstr((sha1[0] + sha1[1] * i) % 2176782336, str);
}

char head[MAXCHAIN][16];
char tail[MAXCHAIN][16];

int main()
{

#ifdef _DEBUG
	freopen("..\\Examples\\第九题样例\\7.in", "r", stdin);
#endif // _DEBUG

	// scanf
	int m = 0;
	scanf("%d", &m);
	for (int i = 0; i < m; i++) {
		scanf("%s", head[i]);
		scanf("%s", tail[i]);
	}
	uint32_t SHA1ToDecrypt[5] = { 0 };
	char SHA1ToDecryptStr[48] = { 0 };
	scanf("%s", SHA1ToDecryptStr);
	for (int i = 0; i < 5; i++) {
		SHA1ToDecrypt[i] = 0;
		for (int j = 8 * i; j < 8 * (i + 1); j++) {
			SHA1ToDecrypt[i] <<= 4;
			if (SHA1ToDecryptStr[j] >= '0' && SHA1ToDecryptStr[j] <= '9') {
				SHA1ToDecrypt[i] += SHA1ToDecryptStr[j] - '0'; 
			}
			else { 
				SHA1ToDecrypt[i] += SHA1ToDecryptStr[j] - 'a' + 10;
			}
		}
	}

	// search
	for (int i = 99; i >= 0; i--) {
		uint32_t tempSHA1[5] = { 0 };
		char tempStr[16] = { 0 };
		R(SHA1ToDecrypt, tempStr, i % 100 + 1);
		for (int j = 0; j < N / 100; j++) {
			for (int k = j == 0 ? i + 1 : 0; k < 100; k++) {
				UnitSHA1(tempStr, 8, tempSHA1);
				R(tempSHA1, tempStr, k % 100 + 1);
			}
			char possibleTail[16] = { 0 };
			strncpy(possibleTail, tempStr, 8);
			for (int k = 0; k < m; k++) {
				if (possibleTail[0] == tail[k][0] && possibleTail[1] == tail[k][1] && 
					possibleTail[2] == tail[k][2] && possibleTail[3] == tail[k][3] && 
					possibleTail[4] == tail[k][4] && possibleTail[5] == tail[k][5] && 
					possibleTail[6] == tail[k][6] && possibleTail[7] == tail[k][7]) {
					uint32_t tempSHA1_2[5] = { 0 };
					char tempStr_2[16] = { 0 };
					memcpy(tempStr_2, head[k], 8);
					UnitSHA1(tempStr_2, 8, tempSHA1_2);
					if (memcmp(tempSHA1_2, SHA1ToDecrypt, 5 * sizeof(uint32_t) == 0)) {
						printf("%s\n", tempStr_2);
						return 0;
					}
					for (int l = 0; l < N - 1 - 100 * j; l++) {
						R(tempSHA1_2, tempStr_2, l % 100 + 1);
						UnitSHA1(tempStr_2, 8, tempSHA1_2);
						if (memcmp(tempSHA1_2, SHA1ToDecrypt, 5 * sizeof(uint32_t)) == 0) {
							printf("%s\n", tempStr_2);
							return 0;
						}
					}
				}
			}
		}
	}

	printf("None\n");
	return 0;
}