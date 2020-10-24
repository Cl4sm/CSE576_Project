void DefaultOptions()
/*
 *  Set up default options
 */
{
	struct passwd *pw;

	/* Do user-specific stuff*/
	if (NULL == (pw = getpwuid(getuid()))) {
		fprintf (stderr, "%s: warning: you don't exist\n", progname);
		strcpy (user_name, DEF_USER_NAME);
		strcpy (home_dir, DEF_HOME_DIR);
	} else {
		strcpy (user_name, pw->pw_name);
		strcpy (home_dir, pw->pw_dir);
	}

	/* Dinky misc options */
	do_debug = DEF_DO_DEBUG;
	do_mail = DEF_DO_MAIL;
	do_news = DEF_DO_NEWS;
#ifdef SCORES
        do_kills = DEF_DO_KILLS;
	kill_thresh = 0;
#endif
	ignore_0fd = DEF_IGNORE0FD;
	inc_hdrs = DEF_INC_HDRS;
	prt_opts = DEF_PRT_OPTS;
	read_only = DEF_READ_ONLY;
	max_blks = DEF_MAX_BLKS;
	grp_len = DEF_GRP_LEN;
	waf_mode = DEF_WAF_MODE;
	slnp_mode = DEF_SLNP_MODE;
	zip_mode = DEF_ZIP_MODE;
	xrf_mode = DEF_XRF_MODE;
	bw_kludge = DEF_BW_KLUDGE;
	xprt_mode = DEF_XPRT_MODE;
	sum_mode = DEF_SUM_MODE;
	sel_mode = DEF_SEL_MODE;
	every_mode = DEF_EVERY_MODE;

#ifdef NNTPAUTH
	strcpy (auth_user, DEF_AUTH_USER);
	strcpy (auth_pass, DEF_AUTH_PASS);
#endif
	strcpy (mail_dir, DEF_MAIL_DIR);
	strcpy (mail_file, DEF_MAIL_FILE);
	strcpy (nrc_file, DEF_NRC_FILE);
#ifndef NNTP
	strcpy (act_file, DEF_ACT_FILE);
	strcpy (news_dir, DEF_NEWS_DIR);
#endif

	strcpy (bbs_name, DEF_BBS_NAME);
	strcpy (bbs_city, DEF_BBS_CITY);
	strcpy (bbs_phone, DEF_BBS_PHONE);
	strcpy (bbs_sysop, DEF_BBS_SYSOP);
	strcpy (bbs_id, DEF_BBS_ID);
	strcpy (rep_file, DEF_REP_FILE);
	strcpy (trn_file, DEF_TRN_FILE);
	strcpy (host_name, DEF_HOST_NAME);
	strcpy (sum_file, DEF_SUM_FILE);
	strcpy (sel_file, DEF_SEL_FILE);
	strcpy (ng_file, DEF_NG_FILE);

	strcpy (inews_path, INEWS_PATH);
#ifdef SCORES
	strcpy (kill_dir, home_dir);
	strcat (kill_dir, "/");
	strcat (kill_dir, DEF_KILL_DIR);
/*	strcpy (kill_dir, DEF_KILL_DIR);	*/
#endif
#ifdef	GNKSA
	gnksa_level = GNKSA_VERBOSE|GNKSA_HONOUR;
#endif
}
