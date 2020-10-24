static void init_token(struct openconnect_info *vpninfo,
		       oc_token_mode_t token_mode, const char *token_str)
{
	int ret;

	ret = openconnect_set_token_mode(vpninfo, token_mode, token_str);

	switch (token_mode) {
	case OC_TOKEN_MODE_STOKEN:
		switch (ret) {
		case 0:
			return;
		case -EINVAL:
			fprintf(stderr, _("Soft token string is invalid\n"));
			exit(1);
		case -ENOENT:
			fprintf(stderr, _("Can't open ~/.stokenrc file\n"));
			exit(1);
		case -EOPNOTSUPP:
			fprintf(stderr, _("OpenConnect was not built with libstoken support\n"));
			exit(1);
		default:
			fprintf(stderr, _("General failure in libstoken\n"));
			exit(1);
		}

		break;

	case OC_TOKEN_MODE_TOTP:
	case OC_TOKEN_MODE_HOTP:
		switch (ret) {
		case 0:
			return;
		case -EINVAL:
			fprintf(stderr, _("Soft token string is invalid\n"));
			exit(1);
		case -EOPNOTSUPP:
			fprintf(stderr, _("OpenConnect was not built with liboath support\n"));
			exit(1);
		default:
			fprintf(stderr, _("General failure in liboath\n"));
			exit(1);
		}

		break;

	case OC_TOKEN_MODE_NONE:
		/* No-op */
		break;

	/* Option parsing already checked for invalid modes. */
	}
}