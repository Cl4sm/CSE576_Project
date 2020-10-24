static int can_gen_stoken_code(struct openconnect_info *vpninfo,
			       struct oc_auth_form *form,
			       struct oc_form_opt *opt)
{
#ifdef HAVE_LIBSTOKEN
	if ((strcmp(opt->name, "password") && strcmp(opt->name, "answer")) ||
	    vpninfo->token_bypassed)
		return -EINVAL;
	if (vpninfo->token_tries == 0) {
		vpn_progress(vpninfo, PRG_DEBUG,
			     _("OK to generate INITIAL tokencode\n"));
		vpninfo->token_time = 0;
	} else if (vpninfo->token_tries == 1 && form->message &&
		   strcasestr(form->message, "next tokencode")) {
		vpn_progress(vpninfo, PRG_DEBUG,
			     _("OK to generate NEXT tokencode\n"));
		vpninfo->token_time += 60;
	} else {
		/* limit the number of retries, to avoid account lockouts */
		vpn_progress(vpninfo, PRG_INFO,
			     _("Server is rejecting the soft token; switching to manual entry\n"));
		return -ENOENT;
	}
	return 0;
#else
	return -EOPNOTSUPP;
#endif
}