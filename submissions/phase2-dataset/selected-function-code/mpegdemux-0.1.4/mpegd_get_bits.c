{
	unsigned long r;
	unsigned long v, m;
	unsigned      b_i, b_n;
	unsigned char *buf;

	if (mpegd_need_bits (mpeg, i + n)) {
		return (0);
	}

	buf = mpeg->buf + mpeg->buf_i;

	r = 0;

	/* aligned bytes */
	if (((i | n) & 7) == 0) {
		i = i / 8;
		n = n / 8;
		while (n > 0) {
			r = (r << 8) | buf[i];
			i += 1;
			n -= 1;
		}
		return (r);
	}


	while (n > 0) {
		b_n = 8 - (i & 7);
		if (b_n > n) {
			b_n = n;
		}

		b_i = 8 - (i & 7) - b_n;

		m = (1 << b_n) - 1;
		v = (buf[i >> 3] >> b_i) & m;

		r = (r << b_n) | v;

		i += b_n;
		n -= b_n;
	}

	return (r);
}
