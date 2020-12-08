#include <iostream>
#include <string.h>

using namespace std;

#define L(addr) \
	(*(addr))
#define H(addr) \
	(*((addr) + 1))
#define MASKBITS(i) \
	((i) % (sizeof(Word) * 8))
#define MASK(i) \
	((Word(-1) >> (sizeof(Word) * 8 - (MASKBITS(i)))))

typedef size_t Size;
typedef unsigned Word;

inline Size
wordCount(Size l) {
	return (l + sizeof(Word) * 8 - 1) / (sizeof(Word) * 8);
}

inline void
allToZero(Size wc, Word* data) {
	memset(data, 0, sizeof(Word) * 2 * wc);
}

inline void
retX(Size wc, Word* data) {
	L(data) = 1;
	H(data) = 1;
	if (wc > 1) {
		data += 2;
		allToZero(wc - 1, data);
	}
}

inline void
ret1(Size wc, Word* data) {
	L(data) = 1;
	H(data) = 0;
	if (wc > 1) {
		data += 2;
		allToZero(wc - 1, data);
	}
}

void 
opReduceXor(Size l, Word* ret, Word* rhs) {
	Size wc = wordCount(l);
	Word* tempR = rhs;
	for (Size i = 0; i < wc - 1; ++i) {
		if (H(tempR))
			return retX(wc, ret);
		tempR += 2;
	}
	Word mask = MASK(l);
	if (H(tempR) & mask)
		return retX(wc, ret);

	Size mid = (wc + 1) / 2;
	Word* tempRhs = new Word[4 * mid];
	memset(tempRhs, 0, sizeof(Word) * 4 * mid);
	memcpy(tempRhs, rhs, sizeof(Word) * 2 * wc);
	Word* pRhs = tempRhs;
	Word* tempRet = new Word[2 * mid];
	memset(tempRet, 0, sizeof(Word) * 2 * mid);
	Word* pRet = tempRet;
	while (mid > 1) {
		for (Size i = 0; i < mid; ++i) {
			L(tempRet) = L(tempRhs) ^ L(tempRhs + mid);
			tempRhs += 2;
			tempRet += 2;
		}
		memset(pRhs, 0, sizeof(Word) * 4 * mid);
		memcpy(pRhs, pRet, sizeof(Word) * 2 * mid);
		memset(pRet, 0, sizeof(Word) * 2 * mid);
		mid = (mid + 1) / 2;
		tempRhs = pRhs;
		tempRet = pRet;
	}
	Size cnt1 = 0; 
	while (L(tempRhs)) {
		++cnt1;
		L(tempRhs) = L(tempRhs) & (L(tempRhs) - 1);
	}
	delete tempRhs;
	delete tempRet;

	if (cnt1 % 2)
		return ret1(wc, ret);
	else
		return allToZero(wc, ret);
}

int main() {
	Size n;
	//Word rhs[4] = { 1, 0, 5, 0 }, ret[4] = { 0 };
	Word rhs[4] = { 0 }, ret[4] = { 0 };
	Size l = 36;
	cout << "input the number of input data: " << endl;
	cin >> n;
	while (n) {
		--n;	
		cout << "input rhs: " << endl;
		cin >> rhs[0] >> rhs[1] >> rhs[2] >> rhs[3];

		opReduceXor(l, ret, rhs);

		cout << "output ret: " << endl;
		cout << ret[0] << " " << ret[1]  << " " << ret[2] << " " << ret[3] << endl;
	}

/*
	opReduceXor(l, ret, rhs);

	cout << ret[0] << " " << ret[1]  << " " << ret[2] << " " << ret[3] << endl;
*/
	return 0;
}

