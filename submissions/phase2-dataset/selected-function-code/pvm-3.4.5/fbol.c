fbol(o, p, n)
	int o;
	unsigned char *p;
	int n;
{
	int i, j;

	for (i = 0; i < 16; i++) {
		if (thesigs[i].length == n) {
			for (j = 0; j < n; j++)
				if (p[j] != thesigs[i].bytes[j])
					break;
			if (j == n)
				return ((3 << 4) | i) << o;

			for (j = 0; j < n; j++)
				if (p[n - 1 - j] != thesigs[i].bytes[j])
					break;
			if (j == n)
				return i << o;

			/* weird float byte order for Strongarm?! */
			/* submitted Alexander Schulz <un23@rz.uni-karlsruhe.de> */
			for (j = 0; j < n; j++)
				if (p[(((j/4)*8)+3)-j] != thesigs[i].bytes[j])
					break;
			if (j == n)
				return ((2 << 4) | i) << o;
		}
	}
	fprintf(stderr, "can't generate signature for my float byte order\n");
	abort();
	return 0;
}
