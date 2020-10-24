void CommandOptions (argc, argv)
int argc;
char *argv[];
/*
 *  Override options from command line
 */
{
	int i;

	for (i=1; i<argc; i++) {
		switch (argv[i][0]) {
		case '+':
			switch (argv[i][1]) {
			case '%':	do_debug = 1;
					break;

#ifdef NNTPAUTH
			case 'A':	/* auth user:pass follows */
					if(!argv[i][2]) {
						fprintf(stderr,"%s: empty AUTH info, trying without NNTP authentication\n", progname);
					} else {
						char *c;
						if((c=strchr(&argv[i][2],':')) != NULL) {
							/* user:pass */
					                *c++ = '\0';
					                strcpy(auth_user,&argv[i][2]);
					                strcpy(auth_pass,c);
							do_auth = 1;
						}
					}
					break;
#endif
			case 'F':	ignore_0fd = 1;
					break;

			case 'm':	do_mail = 1;
					break;

			case 'n':	do_news = 1;
					break;

#ifdef SCORES
			case 'k':       do_kills = 1;
			                break;
#endif

			case 'h':	inc_hdrs = 1;
					break;

			case 'r':	read_only = 1;
					break;

			case 'w':	waf_mode = 1;
					break;

			case 'L':	slnp_mode = 1;
					zip_mode = 0;
					break;

			case 'z':	zip_mode = 1;
					slnp_mode = 0;
					break;
#ifdef	GNKSA
			case 'V':	if(!argv[i][2])
						gnksa_level = GNKSA_VERBOSE | GNKSA_HONOUR;
					else
						gnksa_level = atoi (&argv[i][2]);
					break;
#endif
			case 'x':	xrf_mode = 1;
					break;

			case 'W':	bw_kludge = 1;
					break;

			case 'X':	xprt_mode = 1;
					break;

			case 'e':	every_mode = 1;
					break;

			default:	BadFlag (argv[i]);
					break;
			}
			break;

		case '-':
			switch (argv[i][1]) {
			case '%':	do_debug = 0;
					break;

#ifdef NNTPAUTH
			case 'A':	do_auth = 0;
					break;
#endif
			case 'F':	ignore_0fd = 0;
					break;

			case 'm':	do_mail = 0;
					break;

			case 'n':	do_news = 0;
					break;

#ifdef SCORES
			case 'k':       do_kills = 0;
			                break;

			case 'T':	kill_thresh = atoi (&argv[i][2]);
					break;
#endif
			case 'h':	inc_hdrs = 0;
					break;

			case 'r':	read_only = 0;
					break;

			case 'w':	waf_mode = 0;
					break;

			case 'v':
			case '?':
			case 'p':	prt_opts = 1;
					break;

			case 'I':	strcpy(inews_path,&argv[i][2]);
					break;

			case 'L':	slnp_mode = 0;
					break;

			case 'z':	zip_mode = 0;
					break;
#ifdef	GNKSA
			case 'V':	gnksa_level = 0;
					break;
#endif
			case 'x':	xrf_mode = 0;
					break;

			case 'W':	bw_kludge = 0;
					break;

			case 'X':	xprt_mode = 0;
					break;

			case 'e':	every_mode = 0;
					break;

			case 'M':	strcpy (mail_dir, &argv[i][2]);
					break;

			case 'f':	strcpy (mail_file, &argv[i][2]);
					break;

			case 'u':	strcpy (user_name, &argv[i][2]);
					break;

			case 'H':	strcpy (home_dir, &argv[i][2]);
					break;

			case 'b':	strcpy (bbs_name, &argv[i][2]);
					break;

			case 'c':	strcpy (bbs_city, &argv[i][2]);
					break;

			case 'P':	strcpy (bbs_phone, &argv[i][2]);
					break;

			case 's':	strcpy (bbs_sysop, &argv[i][2]);
					break;

			case 'i':	strcpy (bbs_id, &argv[i][2]);
					break;

#ifndef NNTP
			case 'a':	strcpy (act_file, &argv[i][2]);
					break;

			case 'S':	strcpy (news_dir, &argv[i][2]);
					break;
#endif

			case 'N':	strcpy (nrc_file, &argv[i][2]);
					break;

			case 'B':	max_blks = atoi (&argv[i][2]);
					break;

			case 'R':	strcpy (rep_file, &argv[i][2]);
					break;

			case 'l':	grp_len = atoi (&argv[i][2]);
					break;

			case 't':	strcpy (trn_file, &argv[i][2]);
					break;

			case 'd':	strcpy (host_name, &argv[i][2]);
					break;

			case 'U':	strcpy (sum_file, &argv[i][2]);
					sum_mode = 1;
					break;

			case 'E':	strcpy (sel_file, &argv[i][2]);
					sel_mode = 1;
					break;

			case 'D':	strcpy (ng_file, &argv[i][2]);
					break;

#ifdef SCORES
			case 'K':	strcpy (kill_dir, &argv[i][2]);
			                break;
#endif

			default:	BadFlag (argv[i]);
					break;
			}
			break;

		default:
			BadFlag (argv[i]);
			break;
		}
	}

	/* If mail file has not been overridden, set it */
	if (!strcmp (mail_file, DEF_MAIL_FILE)) {
		strcpy (mail_file, mail_dir);
		strcat (mail_file, "/");
		strcat (mail_file, user_name);
	}

	/* If .newsrc file has not been overridden, set it */
	if (!strcmp (nrc_file, DEF_NRC_FILE)) {
		strcpy (nrc_file, home_dir);
		strcat (nrc_file, "/.newsrc");
	}

	/* Set up posting agent (inews_path + '-h') */
	sprintf(posting_agent, "%s -h", inews_path);

	/* Some sanity checks */

	if(do_news && !slnp_mode && !zip_mode) {
		fprintf(stderr, "%s: The QWK format is incompatible with Usenet beyond repair;\n", progname);
		fprintf(stderr, "%s: please use another format to participate in Usenet!\n", progname);
	}

	/* Summary or selection mode implies do news */
	if (sum_mode || sel_mode) do_news = 1;

	/* Summary mode implies no mail */
	if (sum_mode) do_mail = 0;

	/* Summary mode implies no special packet format */
	if (sum_mode) zip_mode = slnp_mode = 0;

	/* Do auth iff both username and password are there */

#ifdef NNTPAUTH
	if(auth_user[0] && auth_pass[0]) {
		do_auth = 1;
	}
	if(do_auth && (!auth_user[0] || !auth_pass[0])) {
		fprintf(stderr,"%s: no username or password: trying without NNTP authentication\n",progname);
		do_auth = 0;
	}
#endif

	if (prt_opts) {
		PrintOptions();
		exit (0);
	}
}
