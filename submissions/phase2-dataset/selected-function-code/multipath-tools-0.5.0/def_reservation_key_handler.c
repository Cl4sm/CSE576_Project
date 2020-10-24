static int
def_reservation_key_handler(vector strvec)
{
	char *buff;
	char *tbuff;
	int j, k;
	int len;
	uint64_t prkey;

	buff = set_value(strvec);
	if (!buff)
		return 1;

	tbuff = buff;

	if (!memcmp("0x",buff, 2))
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

	if (!conf->reservation_key)
		conf->reservation_key = (unsigned char *) malloc(8);

	memset(conf->reservation_key, 0, 8);

	for (j = 7; j >= 0; --j) {
		conf->reservation_key[j] = (prkey & 0xff);
		prkey >>= 8;
	}

	FREE(tbuff);
	return 0;
}
