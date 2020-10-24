int buf_ensure_space(struct oc_text_buf *buf, int len)
{
	int new_buf_len;

	new_buf_len = (buf->pos + len + BUF_CHUNK_SIZE - 1) & ~(BUF_CHUNK_SIZE - 1);

	if (new_buf_len <= buf->buf_len)
		return 0;

	if (new_buf_len > MAX_BUF_LEN) {
		buf->error = -E2BIG;
		return buf->error;
	} else {
		realloc_inplace(buf->data, new_buf_len);
		if (!buf->data)
			buf->error = -ENOMEM;
		else
			buf->buf_len = new_buf_len;
	}
	return buf->error;
}