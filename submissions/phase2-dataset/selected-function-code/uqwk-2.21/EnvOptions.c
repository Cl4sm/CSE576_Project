/*
 *  Override options from environment variables
 */
{
	char *c;

	if (NULL != (c = getenv ("UQ_DO_DEBUG"))) do_debug = atoi (c);
	if (NULL != (c = getenv ("UQ_DO_MAIL"))) do_mail = atoi (c);
	if (NULL != (c = getenv ("UQ_DO_NEWS"))) do_news = atoi (c);
#ifdef SCORES
	if (NULL != (c = getenv ("UQ_DO_KILLS"))) do_kills = atoi (c);
	if (NULL != (c = getenv ("UQ_KILL_THRESHOLD"))) kill_thresh = atoi (c);
#endif
	if (NULL != (c = getenv ("UQ_IGNORE0FD"))) ignore_0fd = atoi (c);
	if (NULL != (c = getenv ("UQ_INC_HDRS"))) inc_hdrs = atoi (c);
	if (NULL != (c = getenv ("UQ_PRT_OPTS"))) prt_opts = atoi (c);
	if (NULL != (c = getenv ("UQ_READ_ONLY"))) read_only = atoi (c);
	if (NULL != (c = getenv ("UQ_MAX_BLKS"))) max_blks = atoi (c);
	if (NULL != (c = getenv ("UQ_GRP_LEN"))) grp_len = atoi (c);
	if (NULL != (c = getenv ("UQ_WAF_MODE"))) waf_mode = atoi (c);
	if (NULL != (c = getenv ("UQ_ZIP_MODE"))) zip_mode = atoi (c);
	if(zip_mode) slnp_mode = 0;
	if (NULL != (c = getenv ("UQ_SOUP_MODE"))) slnp_mode = atoi (c);
	if(slnp_mode) zip_mode = 0;
	if (NULL != (c = getenv ("UQ_XRF_MODE"))) xrf_mode = atoi (c);
	if (NULL != (c = getenv ("UQ_BW_KLUDGE"))) bw_kludge = atoi (c);
	if (NULL != (c = getenv ("UQ_XPRT_MODE"))) xprt_mode = atoi (c);
	if (NULL != (c = getenv ("UQ_EVERY_MODE"))) every_mode = atoi (c);

	if (NULL != (c = getenv ("UQ_HOME_DIR"))) strcpy (home_dir, c);
#ifdef NNTPAUTH
	if (NULL != (c = getenv ("UQ_AUTH_USER"))) strcpy (auth_user, c);
	if (NULL != (c = getenv ("UQ_AUTH_PASS"))) strcpy (auth_pass, c);
#endif
	if (NULL != (c = getenv ("UQ_MAIL_DIR")))
		strcpy (mail_dir, c);
	else
		if (NULL != (c = getenv ("MAIL"))) strcpy (mail_file, c);
	if (NULL != (c = getenv ("UQ_MAIL_FILE"))) strcpy (mail_file, c);
	if (NULL != (c = getenv ("UQ_USER_NAME"))) strcpy (user_name, c);

        if (NULL != (c = getenv ("UQ_INEWS_PATH"))) strcpy (inews_path, c);
#ifdef SCORES
        if (NULL != (c = getenv ("UQ_KILL_DIR"))) strcpy (kill_dir, c);
#endif
#ifdef	GNKSA
	if (NULL != (c = getenv ("UQ_VALIDATE"))) gnksa_level = atoi (c);
#endif

	if (NULL != (c = getenv ("UQ_BBS_NAME"))) strcpy (bbs_name, c);
	if (NULL != (c = getenv ("UQ_BBS_CITY"))) strcpy (bbs_city, c);
	if (NULL != (c = getenv ("UQ_BBS_PHONE"))) strcpy (bbs_phone, c);
	if (NULL != (c = getenv ("UQ_BBS_SYSOP"))) strcpy (bbs_sysop, c);
	if (NULL != (c = getenv ("UQ_BBS_ID"))) strcpy (bbs_id, c);

#ifndef NNTP
	if (NULL != (c = getenv ("UQ_ACT_FILE"))) strcpy (act_file, c);
	if (NULL != (c = getenv ("UQ_NEWS_DIR"))) strcpy (news_dir, c);
#endif
	if (NULL != (c = getenv ("UQ_NRC_FILE"))) strcpy (nrc_file, c);

	if (NULL != (c = getenv ("UQ_REP_FILE"))) strcpy (rep_file, c);
	if (NULL != (c = getenv ("UQ_TRN_FILE"))) strcpy (trn_file, c);
	if (NULL != (c = getenv ("UQ_HOST_NAME"))) strcpy (host_name, c);
	if (NULL != (c = getenv ("UQ_NG_FILE"))) strcpy (ng_file, c);

	if (NULL != (c = getenv ("UQ_SUM_FILE")))
	{
		strcpy (sum_file, c);
		sum_mode = 1;
	}
	if (NULL != (c = getenv ("UQ_SEL_FILE")))
	{
		strcpy (sel_file, c);
		sel_mode = 1;
	}
}
