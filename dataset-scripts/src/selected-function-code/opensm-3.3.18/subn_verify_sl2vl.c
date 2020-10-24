static void subn_verify_sl2vl(char **sl2vl, const char *prefix)
{
	char *str, *tok, *end, *ptr;
	int count = 0;

	if (*sl2vl == NULL)
		return;

	str = strdup(*sl2vl);

	tok = strtok_r(str, ",\n", &ptr);
	while (tok) {
		long vl = strtol(tok, &end, 0);

		if (*end)
			log_report(" Warning: Cached Option %s_sl2vl:vl=%s "
				   "improperly formatted\n", prefix, tok);
		else if (vl < 0 || vl > 15)
			log_report(" Warning: Cached Option %s_sl2vl:vl=%ld "
				   "out of range\n", prefix, vl);

		count++;
		tok = strtok_r(NULL, ",\n", &ptr);
	}

	if (count < 16)
		log_report(" Warning: Cached Option %s_sl2vl: < 16 VLs "
			   "listed\n", prefix);
	else if (count > 16)
		log_report(" Warning: Cached Option %s_sl2vl: > 16 listed: "
			   "excess VLs will be dropped\n", prefix);

	free(str);
}