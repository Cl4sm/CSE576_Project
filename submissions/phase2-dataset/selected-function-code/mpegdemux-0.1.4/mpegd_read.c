{
	unsigned      ret;
	unsigned      i;
	unsigned char *tmp;

	tmp = (unsigned char *) buf;

	i = (n < mpeg->buf_n) ? n : mpeg->buf_n;

	ret = i;

	if (i > 0) {
		memcpy (tmp, &mpeg->buf[mpeg->buf_i], i);

		tmp += i;
		mpeg->buf_i += i;
		mpeg->buf_n -= i;
		n -= i;
	}

	if (n > 0) {
		ret += fread (tmp, 1, n, mpeg->fp);
	}

	mpeg->ofs += ret;

	return (ret);
}
