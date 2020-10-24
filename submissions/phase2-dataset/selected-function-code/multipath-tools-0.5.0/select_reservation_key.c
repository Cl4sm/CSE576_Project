extern int
select_reservation_key (struct multipath * mp)
{
	int j;
	unsigned char *keyp;
	uint64_t prkey = 0;

	mp->reservation_key = NULL;

	if (mp->mpe && mp->mpe->reservation_key) {
		keyp =  mp->mpe->reservation_key;
		for (j = 0; j < 8; ++j) {
			if (j > 0)
				prkey <<= 8;
			prkey |= *keyp;
			++keyp;
		}

		condlog(3, "%s: reservation_key = 0x%" PRIx64 " "
				"(multipath setting)",  mp->alias, prkey);

		mp->reservation_key = mp->mpe->reservation_key;
		return 0;
	}

	if (conf->reservation_key) {
		keyp = conf->reservation_key;
		for (j = 0; j < 8; ++j) {
			if (j > 0)
				prkey <<= 8;
			prkey |= *keyp;
			++keyp;
		}

		condlog(3, "%s: reservation_key  = 0x%" PRIx64
				" (config file default)", mp->alias, prkey);

		mp->reservation_key = conf->reservation_key;
		return 0;
	}

	return 0;
}
