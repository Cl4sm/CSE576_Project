static int set_totp_mode(struct openconnect_info *vpninfo,
			 const char *token_str)
{
#ifdef HAVE_LIBOATH
	int ret;

	ret = oath_init();
	if (ret != OATH_OK)
		return -EIO;

	if (strncasecmp(token_str, "base32:", strlen("base32:")) == 0) {
		ret = oath_base32_decode(token_str + strlen("base32:"),
					 strlen(token_str) - strlen("base32:"),
					 &vpninfo->oath_secret,
					 &vpninfo->oath_secret_len);
		if (ret != OATH_OK)
			return -EINVAL;
	} else {
		vpninfo->oath_secret = strdup(token_str);
		vpninfo->oath_secret_len = strlen(token_str);
	}

	vpninfo->token_mode = OC_TOKEN_MODE_TOTP;
	return 0;
#else
	return -EOPNOTSUPP;
#endif
}