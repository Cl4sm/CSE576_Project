static int set_hotp_mode(struct openconnect_info *vpninfo,
			 const char *token_str)
{
#ifdef HAVE_LIBOATH
	int ret, toklen;
	char *p;

	ret = oath_init();
	if (ret != OATH_OK)
		return -EIO;

	toklen = strlen(token_str);
	p = strrchr(token_str, ',');
	if (p) {
		long counter;
		toklen = p - token_str;
		p++;
		counter = strtol(p, &p, 0);
		if (counter < 0 || *p)
			return -EINVAL;
		vpninfo->token_time = counter;
	}

	if (strncasecmp(token_str, "base32:", strlen("base32:")) == 0) {
		ret = oath_base32_decode(token_str + strlen("base32:"),
					 toklen - strlen("base32:"),
					 &vpninfo->oath_secret,
					 &vpninfo->oath_secret_len);
		if (ret != OATH_OK)
			return -EINVAL;
	} else {
		vpninfo->oath_secret = strdup(token_str);
		vpninfo->oath_secret_len = toklen;
	}

	vpninfo->token_mode = OC_TOKEN_MODE_HOTP;
	return 0;
#else
	return -EOPNOTSUPP;
#endif
}