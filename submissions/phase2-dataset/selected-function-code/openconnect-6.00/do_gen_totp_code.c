static int do_gen_totp_code(struct openconnect_info *vpninfo,
			    struct oc_auth_form *form,
			    struct oc_form_opt *opt)
{
#ifdef HAVE_LIBOATH
	int oath_err;
	char tokencode[7];

	if (!vpninfo->token_time)
		vpninfo->token_time = time(NULL);

	vpn_progress(vpninfo, PRG_INFO, _("Generating OATH TOTP token code\n"));

	oath_err = oath_totp_generate(vpninfo->oath_secret,
				      vpninfo->oath_secret_len,
				      vpninfo->token_time,
				      OATH_TOTP_DEFAULT_TIME_STEP_SIZE,
				      OATH_TOTP_DEFAULT_START_TIME,
				      6, tokencode);
	if (oath_err != OATH_OK) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Unable to generate OATH TOTP token code: %s\n"),
			     oath_strerror(oath_err));
		return -EIO;
	}

	vpninfo->token_tries++;
	opt->value = strdup(tokencode);
	return opt->value ? 0 : -ENOMEM;
#else
	return 0;
#endif
}