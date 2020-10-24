int prepare_stoken(struct openconnect_info *vpninfo)
{
#ifdef HAVE_LIBSTOKEN
	struct oc_auth_form form;
	struct oc_form_opt opts[3], *opt = opts;
	char **devid = NULL, **pass = NULL, **pin = NULL;
	int ret = 0;

	memset(&form, 0, sizeof(form));
	memset(&opts, 0, sizeof(opts));

	form.opts = opts;
	form.message = _("Enter credentials to unlock software token.");

	vpninfo->token_tries = 0;
	vpninfo->token_bypassed = 0;

	if (stoken_devid_required(vpninfo->stoken_ctx)) {
		opt->type = OC_FORM_OPT_TEXT;
		opt->name = (char *)"devid";
		opt->label = _("Device ID:");
		devid = &opt->value;
		opt++;
	}
	if (stoken_pass_required(vpninfo->stoken_ctx)) {
		opt->type = OC_FORM_OPT_PASSWORD;
		opt->name = (char *)"password";
		opt->label = _("Password:");
		pass = &opt->value;
		opt++;
	}
	if (stoken_pin_required(vpninfo->stoken_ctx)) {
		opt->type = OC_FORM_OPT_PASSWORD;
		opt->name = (char *)"password";
		opt->label = _("PIN:");
		opt->flags = OC_FORM_OPT_NUMERIC;
		pin = &opt->value;
		opt++;
	}

	opts[0].next = opts[1].type ? &opts[1] : NULL;
	opts[1].next = opts[2].type ? &opts[2] : NULL;

	while (1) {
		nuke_opt_values(opts);

		if (!opts[0].type) {
			/* don't bug the user if there's nothing to enter */
			ret = 0;
		} else {
			int some_empty = 0, all_empty = 1;

			/* < 0 for error; 1 if cancelled */
			ret = process_auth_form(vpninfo, &form);
			if (ret)
				break;

			for (opt = opts; opt; opt = opt->next) {
				if (!opt->value || !strlen(opt->value))
					some_empty = 1;
				else
					all_empty = 0;
			}
			if (all_empty) {
				vpn_progress(vpninfo, PRG_INFO,
					     _("User bypassed soft token.\n"));
				vpninfo->token_bypassed = 1;
				ret = 0;
				break;
			}
			if (some_empty) {
				vpn_progress(vpninfo, PRG_INFO,
					     _("All fields are required; try again.\n"));
				continue;
			}
		}

		ret = stoken_decrypt_seed(vpninfo->stoken_ctx,
					  pass ? *pass : NULL,
					  devid ? *devid : NULL);
		if (ret == -EIO || (ret && !devid && !pass)) {
			vpn_progress(vpninfo, PRG_ERR,
				     _("General failure in libstoken.\n"));
			break;
		} else if (ret != 0) {
			vpn_progress(vpninfo, PRG_INFO,
				     _("Incorrect device ID or password; try again.\n"));
			continue;
		}

		if (pin) {
			if (stoken_check_pin(vpninfo->stoken_ctx, *pin) != 0) {
				vpn_progress(vpninfo, PRG_INFO,
					     _("Invalid PIN format; try again.\n"));
				continue;
			}
			free(vpninfo->stoken_pin);
			vpninfo->stoken_pin = strdup(*pin);
			if (!vpninfo->stoken_pin) {
				ret = -ENOMEM;
				break;
			}
		}
		vpn_progress(vpninfo, PRG_DEBUG, _("Soft token init was successful.\n"));
		ret = 0;
		break;
	}

	nuke_opt_values(opts);
	return ret;
#else
	return -EOPNOTSUPP;
#endif
}