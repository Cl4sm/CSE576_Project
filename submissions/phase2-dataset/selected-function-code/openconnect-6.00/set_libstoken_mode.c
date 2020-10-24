static int set_libstoken_mode(struct openconnect_info *vpninfo,
			      const char *token_str)
{
#ifdef HAVE_LIBSTOKEN
	int ret;

	if (!vpninfo->stoken_ctx) {
		vpninfo->stoken_ctx = stoken_new();
		if (!vpninfo->stoken_ctx)
			return -EIO;
	}

	ret = token_str ?
	      stoken_import_string(vpninfo->stoken_ctx, token_str) :
	      stoken_import_rcfile(vpninfo->stoken_ctx, NULL);
	if (ret)
		return ret;

	vpninfo->token_mode = OC_TOKEN_MODE_STOKEN;
	return 0;
#else
	return -EOPNOTSUPP;
#endif
}