char *mpeg_get_name (const char *base, unsigned sid)
{
	unsigned n;
	unsigned dig;
	char     *ret;

	if (base == NULL) {
		base = "stream_##.dat";
	}

	n = 0;
	while (base[n] != 0) {
		n += 1;
	}

	n += 1;

	ret = (char *) malloc (n);
	if (ret == NULL) {
		return (NULL);
	}

	while (n > 0) {
		n -= 1;
		ret[n] = base[n];

		if (ret[n] == '#') {
			dig = sid % 16;
			sid = sid / 16;
			if (dig < 10) {
				ret[n] = '0' + dig;
			}
			else {
				ret[n] = 'a' + dig - 10;
			}
		}
	}

	return (ret);
}
