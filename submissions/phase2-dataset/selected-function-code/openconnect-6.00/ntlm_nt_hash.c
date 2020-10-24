static int ntlm_nt_hash (const char *pass, char hash[21])
{
	struct oc_text_buf *utf16pass = buf_alloc();
	int ret;

	/* Preallocate just to ensure md4sum() doesn't have to realloc, which
	   would leave a copy of the password lying around. There is always
	   at least one byte of padding, then 8 bytes of length, and round up
	   to the next multiple of 64. */
	ret = buf_ensure_space(utf16pass, ((strlen(pass) * 2) + 1 + 8 + 63) & ~63);
	if (ret)
		return ret;

	ret = buf_append_utf16le(utf16pass, pass);
	if (ret < 0)
		return ret;

	ret = buf_error(utf16pass);
	if (ret)
		return ret;

	ret = md4sum(utf16pass, (unsigned char *) hash);
	if (ret)
		return ret;

	memset (hash + 16, 0, 5);

	memset(utf16pass->data, 0, utf16pass->pos);
	buf_free(utf16pass);
	return 0;
}