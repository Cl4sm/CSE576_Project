void inttoms (int i, unsigned char c[4])
{
	int m, e;

	if (i == 0) {
		c[0] = c[1] = c[2] = 0;
		c[3] = 0x80;
		return;
	}

	e = 152;
	m = 0x7fffff & i;

	while (!(0x800000 & m)) {
		m <<= 1;
		e--;
	}
	c[0] = 0xff & m;
	c[1] = 0xff & (m >> 8);
	c[2] = 0x7f & (m >> 16);
	c[3] = 0xff & e;
}
