int mpegd_buffer_fill (mpeg_demux_t *mpeg)
{
	unsigned i, n;
	size_t   r;

	if ((mpeg->buf_i > 0) && (mpeg->buf_n > 0)) {
		for (i = 0; i < mpeg->buf_n; i++) {
			mpeg->buf[i] = mpeg->buf[mpeg->buf_i + i];
		}
	}

	mpeg->buf_i = 0;

	n = MPEG_DEMUX_BUFFER - mpeg->buf_n;

	if (n > 0) {
		r = fread (mpeg->buf + mpeg->buf_n, 1, n, mpeg->fp);
		if (r < 0) {
			return (1);
		}

		mpeg->buf_n += (unsigned) r;
	}

	return (0);
}
