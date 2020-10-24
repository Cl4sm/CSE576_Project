void  __attribute__ ((format (printf, 2, 3)))
	buf_append(struct oc_text_buf *buf, const char *fmt, ...)
{
	va_list ap;

	if (!buf || buf->error)
		return;

	if (buf_ensure_space(buf, 1))
		return;

	while (1) {
		int max_len = buf->buf_len - buf->pos, ret;

		va_start(ap, fmt);
		ret = vsnprintf(buf->data + buf->pos, max_len, fmt, ap);
		va_end(ap);
		if (ret < 0) {
			buf->error = -EIO;
			break;
		} else if (ret < max_len) {
			buf->pos += ret;
			break;
		} else if (buf_ensure_space(buf, ret))
			break;
	}
}