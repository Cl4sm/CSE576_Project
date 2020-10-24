static int
mp_reservation_key_handler (vector strvec)
{
	char *buff;
	char *tbuff;
	struct mpentry *mpe = VECTOR_LAST_SLOT(conf->mptable);

	int j, k, len;
	uint64_t prkey;

	if (!mpe)
		return 1;

	buff = set_value(strvec);
	if (!buff)
		return 1;

	tbuff = buff;
	if (!memcmp(buff, "0x", 2))
		buff = buff + 2;

	len = strlen(buff);

	k = strspn(buff, "0123456789aAbBcCdDeEfF");
	if (len != k) {
		FREE(tbuff);
		return 1;
	}

	if (1 != sscanf (buff, "%" SCNx64 "", &prkey))
	{
		FREE(tbuff);
		return 1;
	}

	if (!mpe->reservation_key)
		mpe->reservation_key = (unsigned char *) malloc(8);

	memset(mpe->reservation_key, 0, 8);

	for (j = 7; j >= 0; --j) {
		mpe->reservation_key[j] = (prkey & 0xff);
		prkey >>= 8;
	}

	FREE(tbuff);
	return 0;
}
