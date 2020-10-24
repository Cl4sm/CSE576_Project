static int
ibol(o, p, n)
	int o;
	char *p;
	int n;
{
	int i, j;

	if (p[0] == 0) {
		i = 0;	/* ll */
	} else if (p[0] == n - 1) {
		i = 3;	/* hh */
	} else if (p[0] == n / 2) {
		i = 2;	/* hl */
	} else if (p[0] == n / 2 - 1) {
		i = 1;	/* lh */
	} else {
		fprintf(stderr, "can't generate signature for my integer byte order\n");
		abort();
	}

	j = ffs(n) - 1;
/*
	printf(".%d%d.%d%d%d", (i & 2) ? 1 : 0, (i & 1) ? 1 : 0,
			(j & 4) ? 1 : 0, (j & 2) ? 1 : 0, (j & 1) ? 1 : 0);
*/
	return ((i << 3) | j) << o;
}
