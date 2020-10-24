int openconnect_set_token_mode(struct openconnect_info *vpninfo,
			       oc_token_mode_t token_mode,
			       const char *token_str)
{
	vpninfo->token_mode = OC_TOKEN_MODE_NONE;

	switch (token_mode) {
	case OC_TOKEN_MODE_NONE:
		return 0;

	case OC_TOKEN_MODE_STOKEN:
		return set_libstoken_mode(vpninfo, token_str);

	case OC_TOKEN_MODE_TOTP:
		return set_totp_mode(vpninfo, token_str);

	case OC_TOKEN_MODE_HOTP:
		return set_hotp_mode(vpninfo, token_str);

	default:
		return -EOPNOTSUPP;
	}
}