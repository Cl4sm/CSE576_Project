static int do_gen_hotp_code(struct openconnect_info *vpninfo,
			    struct oc_auth_form *form,
			    struct oc_form_opt *opt)
{
#ifdef HAVE_LIBOATH
	int oath_err;
	char tokencode[7];

	vpn_progress(vpninfo, PRG_INFO, _("Generating OATH HOTP token code\n"));

	oath_err = oath_hotp_generate(vpninfo->oath_secret,
				      vpninfo->oath_secret_len,
				      vpninfo->token_time,
				      6, false, OATH_HOTP_DYNAMIC_TRUNCATION,
				      tokencode);
	if (oath_err != OATH_OK) {
		vpn_progress(vpninfo, PRG_ERR,
			     _("Unable to generate OATH HOTP token code: %s\n"),
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