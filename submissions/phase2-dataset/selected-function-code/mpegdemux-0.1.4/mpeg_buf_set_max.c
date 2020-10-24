int mpeg_buf_set_max (mpeg_buffer_t *buf, unsigned max)
{
	if (buf->max == max) {
		return (0);
	}

	if (max == 0) {
		free (buf->buf);
		buf->max = 0;
		buf->cnt = 0;
		return (0);
	}

	buf->buf = realloc (buf->buf, max);
	if (buf->buf == NULL) {
		buf->max = 0;
		buf->cnt = 0;
		return (1);
	}

	buf->max = max;

	if (buf->cnt > max) {
		buf->cnt = max;
	}

	return (0);
}
