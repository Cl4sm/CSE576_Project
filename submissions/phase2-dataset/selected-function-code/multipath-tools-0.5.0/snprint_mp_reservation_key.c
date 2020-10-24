snprint_mp_reservation_key (char * buff, int len, void * data)
{
	int i;
	unsigned char *keyp;
	uint64_t prkey = 0;
	struct mpentry * mpe = (struct mpentry *)data;

	if (!mpe->reservation_key)
		return 0;
	keyp = (unsigned char *)mpe->reservation_key;
	for (i = 0; i < 8; i++) {
		if (i > 0)
			prkey <<= 8;
		prkey |= *keyp;
		keyp++;
	}

	return snprintf(buff, len, "0x%" PRIx64, prkey);
}
