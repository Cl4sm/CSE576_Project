{
	unsigned char buf[4096];
	unsigned      i, j;

	while (n > 0) {
		i = (n < 4096) ? n : 4096;

		j = mpegd_read (mpeg, buf, i);

		if (j > 0) {
			if (fwrite (buf, 1, j, fp) != j) {
				return (1);
			}
		}

		if (i != j) {
			return (1);
		}

		n -= i;
	}

	return (0);
}
