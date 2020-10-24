static int buf_append_utf16le(struct oc_text_buf *buf, const char *utf8)
{
	int len = 0;
	unsigned char c;
	unsigned long utfchar;
	int min;
	int nr_extra;

	/* Ick. Now I'm implementing my own UTF8 handling too. Perhaps it's
	   time to bite the bullet and start requiring something like glib? */
	while (*utf8) {
		c = *(utf8++);
		if (c < 128) {
			utfchar = c;
			nr_extra = 0;
			min = 0;
		} else if ((c & 0xe0) == 0xc0) {
			utfchar = c & 0x1f;
			nr_extra = 1;
			min = 0x80;
		} else if ((c & 0xf0) == 0xe0) {
			utfchar = c & 0x0f;
			nr_extra = 2;
			min = 0x800;
		} else if ((c & 0xf8) == 0xf0) {
			utfchar = c & 0x07;
			nr_extra = 3;
			min = 0x10000;
		} else {
			return -EINVAL;
		}

		while (nr_extra--) {
			c = *(utf8++);
			if ((c & 0xc0) != 0x80)
				return -EINVAL;
			utfchar <<= 6;
			utfchar |= (c & 0x3f);
		}
		if (utfchar > 0x10ffff || utfchar < min)
			return -EINVAL;

		if (utfchar >= 0x10000) {
			utfchar -= 0x10000;
			if (buf_ensure_space(buf, 4))
				return -ENOMEM;
			buf->data[buf->pos++] = (utfchar >> 10) & 0xff;
			buf->data[buf->pos++] = 0xd8 | ((utfchar >> 18) & 3);
			buf->data[buf->pos++] = utfchar & 0xff;
			buf->data[buf->pos++] = 0xdc | ((utfchar >> 8) & 3);
			len += 4;
		} else {
			if (buf_ensure_space(buf, 2))
				return -ENOMEM;
			buf->data[buf->pos++] = utfchar & 0xff;
			buf->data[buf->pos++] = utfchar >> 8;
			len += 2;
		}
	}
	return len;
}