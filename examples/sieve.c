#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned char byte;

void * xmalloc(size_t n)
{
	void * ret = malloc(n);
	if (!ret)
	{
		fprintf(stderr, "error: malloc() failed\n");
		exit(EXIT_FAILURE);
	}
	return ret;
}

// <default>
unsigned int sieve(unsigned int limit)
{
	size_t mem_sz = (limit + 2) * sizeof(byte);
	byte * is_prime = xmalloc(mem_sz);

	memset(is_prime, true, mem_sz);
	is_prime[0] = is_prime[1] = false;

	for (unsigned int i = 2; i <= limit; ++i)
	{
		if ((unsigned long long)(i * i) <= limit && is_prime[i])
		{
			for (unsigned int j = i * i; j <= limit; j += i)
				is_prime[j] = false;
		}
	}

	unsigned int prime = 0;
	for (unsigned int i = limit; i >= 0; --i)
	{
		if (is_prime[i])
		{
			prime = i;
			break;
		}
	}

	free(is_prime);
	return prime;
}
// </default>

int main(int argc, char * argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Use: <prog> <num>\n");
		return 1;
	}

	unsigned int sieve_limit = 0;
	if (sscanf(argv[1], "%u", &sieve_limit) != 1)
	{
		fprintf(stderr, "error: '%s' not a number\n", argv[1]);
		return 2;
	}

	printf("%u\n", sieve(sieve_limit));

	return 0;
}
