#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

typedef struct bignum {
	unsigned char *data;
	size_t allocated;
	size_t length;
} bignum_t;

void bignum_from_uint(bignum_t *bn, unsigned int n)
{
	size_t i;
	unsigned char *pn;

	bn->allocated = sizeof(n);
	bn->length = bn->allocated;
	bn->data = malloc(bn->allocated * sizeof(*bn->data));

	pn = (unsigned char *)&n;
	for (i = 0; i < bn->length; ++i) {
		bn->data[i] = pn[i];
	}
}

void bignum_add_uint(bignum_t *bn, unsigned int n)
{
	unsigned short s;
	unsigned char *pn;
	unsigned char *ps;
	unsigned char c;
	size_t i;

	pn = (unsigned char *)&n;
	ps = (unsigned char *)&s;
	c = 0U;
	for (i = 0; i < bn->allocated; i += 1) {
		s = bn->data[i] + c + (i < sizeof(n) ? pn[i] : 0U);
		bn->data[i] = ps[0];
		c = ps[1];
	}

	if (c != 0) {
		bn->allocated += 1;
		bn->length = bn->allocated;
		bn->data = realloc(bn->data, bn->allocated * sizeof(*bn->data));
		bn->data[bn->length - 1] = c;
	}
}

void bignum_print(bignum_t *bn)
{
	size_t i;

	for (i = bn->length; i > 0; i -= 1) {
		printf("%02X", bn->data[i - 1]);
	}
	printf("\n");
}

int main()
{
	bignum_t biga;

	bignum_from_uint(&biga, UINT_MAX - 1U);
	bignum_print(&biga);
	bignum_add_uint(&biga, 1U);
	bignum_print(&biga);
	bignum_add_uint(&biga, 1U);
	bignum_print(&biga);

	return 0;
}
