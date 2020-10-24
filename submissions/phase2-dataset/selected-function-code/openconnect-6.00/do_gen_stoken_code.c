static int do_gen_stoken_code(struct openconnect_info *vpninfo,
			      struct oc_auth_form *form,
			      struct oc_form_opt *opt)
{
#ifdef HAVE_LIBSTOKEN
	char tokencode[STOKEN_MAX_TOKENCODE + 1];

	if (!vpninfo->token_time)
		vpninfo->token_time = time(NULL);
	vpn_progress(vpninfo, PRG_INFO, _("Generating RSA token code\n"));

	/* This doesn't normally fail */
	if (stoken_compute_tokencode(vpninfo->stoken_ctx, vpninfo->token_time,
				     vpninfo->stoken_pin, tokencode) < 0) {
		vpn_progress(vpninfo, PRG_ERR, _("General failure in libstoken.\n"));
		return -EIO;
	}

	vpninfo->token_tries++;
	opt->value = strdup(tokencode);
	return opt->value ? 0 : -ENOMEM;
#else
	return 0;
#endif
}