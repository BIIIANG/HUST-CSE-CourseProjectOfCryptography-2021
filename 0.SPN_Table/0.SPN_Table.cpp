#pragma warning(disable:6031)
#pragma warning(disable:4996)
#include <stdio.h>

#define Nr 4

// define substitution
const char substitution[16] = { 0xE, 0x4, 0xD, 0x1, 0x2, 0xF, 0xB, 0x8, 0x3, 0xA, 0x6, 0xC, 0x5, 0x9, 0x0, 0x7 };
const char substitutionInv[16] = { 0xE, 0x3, 0x4, 0x8, 0x1, 0xC, 0xA, 0xF, 0x7, 0xD, 0x9, 0x6, 0xB, 0x2, 0x0, 0x5 };

int main()
{
	FILE* fp = NULL;



#pragma region Substitution_1
	fp = fopen("..\\0.SPN_Table\\Substitution_1.txt", "w+");
	if (fp == NULL) { return 1; }
	fprintf(fp, "const unsigned short Substitution_1[0x100] = {\n\t");
	for (int i = 0x0000; i <= 0x00FF; i++) {
		short res = (substitution[(i >> 4) & 0xF] << 12) |
			(substitution[(i) & 0xF] << 8);
		fprintf(fp, "0x%04hx", res);
		if (i != 0xFFFF) {
			fprintf(fp, ", ");
		}
		if (!((i + 1) % 16)) {
			fprintf(fp, "\n\t");
		}
	}
	fprintf(fp, "};");
	fclose(fp);
	fp = NULL;
#pragma endregion

#pragma region Substitution_2
	fp = fopen("..\\0.SPN_Table\\Substitution_2.txt", "w+");
	if (fp == NULL) { return 1; }
	fprintf(fp, "const unsigned short Substitution_2[0x100] = {\n\t");
	for (int i = 0x0000; i <= 0x00FF; i++) {
		short res = (substitution[(i >> 4) & 0xF] << 4) |
			(substitution[(i) & 0xF]);
		fprintf(fp, "0x%04hx", res);
		if (i != 0xFFFF) {
			fprintf(fp, ", ");
		}
		if (!((i + 1) % 16)) {
			fprintf(fp, "\n\t");
		}
	}
	fprintf(fp, "};");
	fclose(fp);
	fp = NULL;
#pragma endregion

#pragma region PermutationTable
	fp = fopen("..\\0.SPN_Table\\PermutationTable.txt", "w+");
	if (fp == NULL) { return 1; }
	fprintf(fp, "const unsigned short permutationTable[0x10000] = {\n\t");
	for (int i = 0; i <= 0xFFFF; i++) {
		short res = (i & 0x8000) |
			((i & 0x0800) << 3) |
			((i & 0x0080) << 6) |
			((i & 0x0008) << 9) |
			((i & 0x4000) >> 3) |
			(i & 0x0400) |
			((i & 0x0040) << 3) |
			((i & 0x0004) << 6) |
			((i & 0x2000) >> 6) |
			((i & 0x0200) >> 3) |
			(i & 0x0020) |
			((i & 0x0002) << 3) |
			((i & 0x1000) >> 9) |
			((i & 0x0100) >> 6) |
			((i & 0x0010) >> 3) |
			(i & 0x1);
		fprintf(fp, "0x%04hx", res);
		if (i != 0xFFFF) {
			fprintf(fp, ", ");
		}
		if (!((i + 1) % 16)) {
			fprintf(fp, "\n\t");
		}
	}
	fprintf(fp, "};");
	fclose(fp);
	fp = NULL;
#pragma endregion

#pragma region SubAndPerTable_En
	fp = fopen("..\\0.SPN_Table\\SubAndPerTable_En.txt", "w+");
	if (fp == NULL) { return 1; }
	fprintf(fp, "const unsigned short subAndPerTable_En[0x10000] = {\n\t");
	for (int i = 0; i <= 0xFFFF; i++) {
		short res = (substitution[(i >> 12) & 0xF] << 12) |
			(substitution[(i >> 8) & 0xF] << 8) |
			(substitution[(i >> 4) & 0xF] << 4) |
			(substitution[(i) & 0xF]);
		res = (res & 0x8000) |
			((res & 0x0800) << 3) |
			((res & 0x0080) << 6) |
			((res & 0x0008) << 9) |
			((res & 0x4000) >> 3) |
			(res & 0x0400) |
			((res & 0x0040) << 3) |
			((res & 0x0004) << 6) |
			((res & 0x2000) >> 6) |
			((res & 0x0200) >> 3) |
			(res & 0x0020) |
			((res & 0x0002) << 3) |
			((res & 0x1000) >> 9) |
			((res & 0x0100) >> 6) |
			((res & 0x0010) >> 3) |
			(res & 0x1);
		//fprintf(fp, "0x%04hx", res);
		fprintf(fp, "%hu", res);
		if (i != 0xFFFF) {
			fprintf(fp, ", ");
		}
		if (!((i + 1) % 16)) {
			fprintf(fp, "\n\t");
		}
	}
	fprintf(fp, "};");
	fclose(fp);
	fp = NULL;
#pragma endregion

#pragma region PerAndSubTable_De
	fp = fopen("..\\0.SPN_Table\\PerAndSubTable_De.txt", "w+");
	if (fp == NULL) { return 1; }
	fprintf(fp, "const unsigned short PerAndSubTable_De[0x10000] = {\n\t");
	for (int i = 0; i <= 0xFFFF; i++) {
		short res = (i & 0x8000) |
			((i & 0x0800) << 3) |
			((i & 0x0080) << 6) |
			((i & 0x0008) << 9) |
			((i & 0x4000) >> 3) |
			(i & 0x0400) |
			((i & 0x0040) << 3) |
			((i & 0x0004) << 6) |
			((i & 0x2000) >> 6) |
			((i & 0x0200) >> 3) |
			(i & 0x0020) |
			((i & 0x0002) << 3) |
			((i & 0x1000) >> 9) |
			((i & 0x0100) >> 6) |
			((i & 0x0010) >> 3) |
			(i & 0x1);
		res = (substitutionInv[(res >> 12) & 0xF] << 12) |
			(substitutionInv[(res >> 8) & 0xF] << 8) |
			(substitutionInv[(res >> 4) & 0xF] << 4) |
			(substitutionInv[(res) & 0xF]);
		//fprintf(fp, "0x%04hx", res);
		fprintf(fp, "%hu", res);
		if (i != 0xFFFF) {
			fprintf(fp, ", ");
		}
		if (!((i + 1) % 16)) {
			fprintf(fp, "\n\t");
		}
	}
	fprintf(fp, "};");
	fclose(fp);
	fp = NULL;
#pragma endregion

#pragma region SubAndPerTable_En_1
	fp = fopen("..\\0.SPN_Table\\SubAndPerTable_En_1.txt", "w+");
	if (fp == NULL) { return 1; }
	fprintf(fp, "const unsigned short SubAndPerTable_En_1[0x100] = {\n\t");
	for (int i = 0x0000; i <= 0xFFFF; i += 0x0100) {
		short res = (substitution[(i >> 12) & 0xF] << 12) |
			(substitution[(i >> 8) & 0xF] << 8);
		res = (res & 0x8000) |
			((res & 0x0800) << 3) |
			((res & 0x4000) >> 3) |
			(res & 0x0400) |
			((res & 0x2000) >> 6) |
			((res & 0x0200) >> 3) |
			((res & 0x1000) >> 9) |
			((res & 0x0100) >> 6);
		fprintf(fp, "0x%04hx", res);
		if (((i >> 8) & 0xFF) != 0xFF) {
			fprintf(fp, ", ");
		}
		if (!((((i >> 8) & 0xFF) + 1) % 16)) {
			fprintf(fp, "\n\t");
		}
	}
	fprintf(fp, "};");
	fclose(fp);
	fp = NULL;
#pragma endregion

#pragma region SubAndPerTable_En_2
	fp = fopen("..\\0.SPN_Table\\SubAndPerTable_En_2.txt", "w+");
	if (fp == NULL) { return 1; }
	fprintf(fp, "const unsigned short SubAndPerTable_En_2[0x100] = {\n\t");
	for (int i = 0x0000; i <= 0x00FF; i++) {
		short res = (substitution[(i >> 4) & 0xF] << 4) |
			(substitution[(i) & 0xF]);
		res = ((res & 0x0080) << 6) |
			((res & 0x0008) << 9) |
			((res & 0x0040) << 3) |
			((res & 0x0004) << 6) |
			(res & 0x0020) |
			((res & 0x0002) << 3) |
			((res & 0x0010) >> 3) |
			(res & 0x1);
		fprintf(fp, "0x%04hx", res);
		if ((i & 0xFF) != 0xFF) {
			fprintf(fp, ", ");
		}
		if (!(((i & 0xFF) + 1) % 16)) {
			fprintf(fp, "\n\t");
		}
	}
	fprintf(fp, "};");
	fclose(fp);
	fp = NULL;
#pragma endregion

#pragma region SubAndPerTable_De_1
	fp = fopen("..\\0.SPN_Table\\SubAndPerTable_De_1.txt", "w+");
	if (fp == NULL) { return 1; }
	fprintf(fp, "const unsigned short SubAndPerTable_De_1[0x100] = {\n\t");
	for (int i = 0x0000; i <= 0xFFFF; i += 0x0100) {
		short res = (substitutionInv[(i >> 12) & 0xF] << 12) |
			(substitutionInv[(i >> 8) & 0xF] << 8);
		res = (res & 0x8000) |
			((res & 0x0800) << 3) |
			((res & 0x4000) >> 3) |
			(res & 0x0400) |
			((res & 0x2000) >> 6) |
			((res & 0x0200) >> 3) |
			((res & 0x1000) >> 9) |
			((res & 0x0100) >> 6);
		fprintf(fp, "0x%04hx", res);
		if (((i >> 8) & 0xFF) != 0xFF) {
			fprintf(fp, ", ");
		}
		if (!((((i >> 8) & 0xFF) + 1) % 16)) {
			fprintf(fp, "\n\t");
		}
	}
	fprintf(fp, "};");
	fclose(fp);
	fp = NULL;
#pragma endregion

#pragma region SubAndPerTable_De_2
	fp = fopen("..\\0.SPN_Table\\SubAndPerTable_De_2.txt", "w+");
	if (fp == NULL) { return 1; }
	fprintf(fp, "const unsigned short SubAndPerTable_De_2[0x100] = {\n\t");
	for (int i = 0x0000; i <= 0x00FF; i++) {
		short res = (substitutionInv[(i >> 4) & 0xF] << 4) |
			(substitutionInv[(i) & 0xF]);
		res = ((res & 0x0080) << 6) |
			((res & 0x0008) << 9) |
			((res & 0x0040) << 3) |
			((res & 0x0004) << 6) |
			(res & 0x0020) |
			((res & 0x0002) << 3) |
			((res & 0x0010) >> 3) |
			(res & 0x1);
		fprintf(fp, "0x%04hx", res);
		if ((i & 0xFF) != 0xFF) {
			fprintf(fp, ", ");
		}
		if (!(((i & 0xFF) + 1) % 16)) {
			fprintf(fp, "\n\t");
		}
	}
	fprintf(fp, "};");
	fclose(fp);
	fp = NULL;
#pragma endregion

#pragma region SubAndPerTable_De
	fp = fopen("..\\0.SPN_Table\\SubAndPerTable_De.txt", "w+");
	if (fp == NULL) { return 1; }
	fprintf(fp, "const unsigned short SubAndPerTable_De[0x10000] = {\n\t");
	for (int i = 0; i <= 0xFFFF; i++) {
		short res = (substitutionInv[(i >> 12) & 0xF] << 12) |
			(substitutionInv[(i >> 8) & 0xF] << 8) |
			(substitutionInv[(i >> 4) & 0xF] << 4) |
			(substitutionInv[(i) & 0xF]);
		res = (res & 0x8000) |
			((res & 0x0800) << 3) |
			((res & 0x0080) << 6) |
			((res & 0x0008) << 9) |
			((res & 0x4000) >> 3) |
			(res & 0x0400) |
			((res & 0x0040) << 3) |
			((res & 0x0004) << 6) |
			((res & 0x2000) >> 6) |
			((res & 0x0200) >> 3) |
			(res & 0x0020) |
			((res & 0x0002) << 3) |
			((res & 0x1000) >> 9) |
			((res & 0x0100) >> 6) |
			((res & 0x0010) >> 3) |
			(res & 0x1);
		//fprintf(fp, "0x%04hx", res);
		fprintf(fp, "%hu", res);
		if (i != 0xFFFF) {
			fprintf(fp, ", ");
		}
		if (!((i + 1) % 16)) {
			fprintf(fp, "\n\t");
		}
	}
	fprintf(fp, "};");
	fclose(fp);
	fp = NULL;
#pragma endregion

#pragma region PermutationTable_1
		fp = fopen("..\\0.SPN_Table\\PermutationTable_1.txt", "w+");
	if (fp == NULL) { return 1; }
	fprintf(fp, "const unsigned short PermutationTable_1[0x100] = {\n\t");
	for (int i = 0x0000; i <= 0xFFFF; i += 0x0100) {
		short res = (i & 0x8000) |
			((i & 0x0800) << 3) |
			((i & 0x4000) >> 3) |
			(i & 0x0400) |
			((i & 0x2000) >> 6) |
			((i & 0x0200) >> 3) |
			((i & 0x1000) >> 9) |
			((i & 0x0100) >> 6);
		fprintf(fp, "0x%04hx", res);
		if (((i >> 8) & 0xFF) != 0xFF) {
			fprintf(fp, ", ");
		}
		if (!((((i >> 8) & 0xFF) + 1) % 16)) {
			fprintf(fp, "\n\t");
		}
	}
	fprintf(fp, "};");
	fclose(fp);
	fp = NULL;
#pragma endregion

#pragma region PermutationTable_2
	fp = fopen("..\\0.SPN_Table\\PermutationTable_2.txt", "w+");
	if (fp == NULL) { return 1; }
	fprintf(fp, "const unsigned short PermutationTable_2[0x100] = {\n\t");
	for (int i = 0x0000; i <= 0x00FF; i++) {
		short res = ((i & 0x0080) << 6) |
			((i & 0x0008) << 9) |
			((i & 0x0040) << 3) |
			((i & 0x0004) << 6) |
			(i & 0x0020) |
			((i & 0x0002) << 3) |
			((i & 0x0010) >> 3) |
			(i & 0x1);
		fprintf(fp, "0x%04hx", res);
		if ((i & 0xFF) != 0xFF) {
			fprintf(fp, ", ");
		}
		if (!(((i & 0xFF) + 1) % 16)) {
			fprintf(fp, "\n\t");
		}
	}
	fprintf(fp, "};");
	fclose(fp);
	fp = NULL;
#pragma endregion

	return 0;

}
