int openconnect_base64_decode(void **out, const char *in)
{
	unsigned char *buf;
	int b[4];
	int len = strlen(in);

	if (len & 3)
		return -EINVAL;
	len = (len * 3) / 4;
	*out = buf = malloc(len);
	if (!buf)
		return -ENOMEM;

	len = 0;
	while (*in) {
		if (!in[1] || !in[2] || !in[3])
			goto err;
	        b[0] = b64_char(in[0]);
		b[1] = b64_char(in[1]);
		if (b[0] < 0 || b[1] < 0)
			goto err;
		*(buf++) = (b[0] << 2) | (b[1] >> 4);

		if (in[2] == '=') {
			if (in[3] != '=' || in[4] != 0)
				goto err;
			len += 1;
			break;
		}
		b[2] = b64_char(in[2]);
		if (b[2] < 0)
			goto err;
		*(buf++) = (b[1] << 4) | (b[2] >> 2);
		if (in[3] == '=') {
			if (in[4] != 0)
				goto err;
			len += 2;
			break;
		}
		b[3] = b64_char(in[3]);
		if (b[3] < 0)
			goto err;
		*(buf++) = (b[2] << 6) | b[3];
		len += 3;
		in += 4;
	}
	return len;

 err:
	free(buf);
	*out = NULL;
	return -EINVAL;
}