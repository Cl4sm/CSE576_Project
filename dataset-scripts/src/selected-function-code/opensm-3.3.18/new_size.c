static int new_size(int size)
{
	int i;

#if 0
	for (i = 3; i < 31; i++) {
		if ((1 << i) > size)
			return 1 << i;
	}
	return -1;
#else
	int newsize;

	for (i = 0, newsize = MINSIZE;
	     i < sizeof(primes) / sizeof(primes[0]); i++, newsize <<= 1) {
		if (newsize > size)
			return primes[i];
	}
	/* Ran out of polynomials */
	return 0;		/* should raise exception */
#endif
}