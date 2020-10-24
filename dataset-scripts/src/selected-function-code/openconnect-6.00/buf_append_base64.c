void buf_append_base64(struct oc_text_buf *buf, const void *bytes, int len)
{
	const unsigned char *in = bytes;
	int hibits;

	if (!buf || buf->error)
		return;

	if (buf_ensure_space(buf, (4 * (len + 2) / 3) + 1))
		return;

	while (len > 0) {
		buf->data[buf->pos++] = b64_table[in[0] >> 2];
		hibits = (in[0] << 4) & 0x30;
		if (len == 1) {
			buf->data[buf->pos++] = b64_table[hibits];
			buf->data[buf->pos++] = '=';
			buf->data[buf->pos++] = '=';
			break;
		}
		buf->data[buf->pos++] = b64_table[hibits | (in[1] >> 4)];
		hibits = (in[1] << 2) & 0x3c;
		if (len == 2) {
			buf->data[buf->pos++] = b64_table[hibits];
			buf->data[buf->pos++] = '=';
			break;
		}
		buf->data[buf->pos++] = b64_table[hibits | (in[2] >> 6)];
		buf->data[buf->pos++] = b64_table[in[2] & 0x3f];
		in += 3;
		len -= 3;
	}
	buf->data[buf->pos] = 0;
}