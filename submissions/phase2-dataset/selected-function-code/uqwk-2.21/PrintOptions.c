{
	printf ("Version: %s\n", UQWK_VERSION);
	printf ("Do mail: %s\n", do_mail?"yes":"no");
	printf ("Do news: %s\n", do_news?"yes":"no");
#ifdef NNTPAUTH
	printf ("Use NNTP authentication: %s\n", do_auth?"yes":"no");
	if(do_auth) {
		printf ("AUTH user: %s\n", auth_user);
		printf ("AUTH pass: %s\n", auth_pass);
	}
#endif
#ifdef SCORES
	printf ("Score articles: %s\n", do_kills?"yes":"no");
	if(do_kills) {
		printf ("Kill threshold: %d\n", kill_thresh);
		printf ("Score directory: %s\n", kill_dir);
		printf ("Global score file: %s/%s\n", kill_dir, GLOBAL_SCORE_FILE);
	}
#endif
#ifdef	GNKSA
	printf ("Validate articles before posting: %s, invalid articles are %s%s\n",
		gnksa_level?"yes":"no",
		(gnksa_level&GNKSA_HONOUR)?"not posted":"posted anyway",
		(gnksa_level&GNKSA_VERBOSE)?"\n\t(errors or warnings are reported)":""
		);
#endif
	printf ("SOUP mode: %s\n", slnp_mode?"yes":"no");
	printf ("ZipNews mode: %s\n", zip_mode?"yes":"no");
	printf ("QWK mode: %s\n", (slnp_mode||zip_mode)?"no":"yes");
	printf ("Xref mode: %s\n", xrf_mode?"yes":"no");
	printf ("Read only: %s\n", read_only?"yes":"no");
	printf ("Maximum blocks: %d\n", max_blks);
	printf ("Mail directory: %s\n", mail_dir);
#ifndef NNTP
	printf ("Active file: %s\n", act_file);
	printf ("News directory: %s\n", news_dir);
#endif
	printf ("Mail file: %s\n", mail_file);
	printf ("User name: %s\n", user_name);
	printf ("Home directory: %s\n", home_dir);
	printf (".newsrc file: %s\n", nrc_file);
	printf ("Reply file: %s\n", rep_file);
	printf ("Posting agent: %s, called as: %s\n", inews_path, posting_agent);
	printf ("Newsgroup name translation table: %s\n", trn_file);
	printf ("Host name: %s\n", host_name);
	printf ("Summary file: %s\n", sum_file);
	printf ("Selection file: %s\n", sel_file);
	printf ("Newsgroup handling order file: %s\n", ng_file);
	printf ("Interpret newsrc file as a Waffle \"join\" file: %s\n", waf_mode?"yes":"no");
	printf ("Expert mode (QWK): %s\n", xprt_mode?"yes":"no");
	printf ("Include headers (QWK): %s\n", inc_hdrs?"yes":"no");
	printf ("Every group in CONTROL.DAT (QWK): %s\n", every_mode?"yes":"no");
	printf ("Group name limit (QWK): %d\n", grp_len);
	printf ("Blue Wave kludge (QWK): %s\n", bw_kludge?"yes":"no");
	printf ("BBS name (QWK): %s\n", bbs_name);
	printf ("BBS city (QWK): %s\n", bbs_city);
	printf ("BBS phone (QWK): %s\n", bbs_phone);
	printf ("BBS sysop (QWK): %s\n", bbs_sysop);
	printf ("BBS id (QWK): %s\n", bbs_id);
}
