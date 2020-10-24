{
	size_t r;

	mpeg->ofs += n;

	if (n <= mpeg->buf_n) {
		mpeg->buf_i += n;
		mpeg->buf_n -= n;
		return (0);
	}

	n -= mpeg->buf_n;
	mpeg->buf_i = 0;
	mpeg->buf_n = 0;

	while (n > 0) {
		if (n <= MPEG_DEMUX_BUFFER) {
			r = fread (mpeg->buf, 1, n, mpeg->fp);
		}
		else {
			r = fread (mpeg->buf, 1, MPEG_DEMUX_BUFFER, mpeg->fp);
		}

		if (r <= 0) {
			return (1);
		}

		n -= (unsigned) r;
	}

	return (0);
}
