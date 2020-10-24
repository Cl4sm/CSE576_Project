char *name;
int num;
{
	struct conf_ent *tmp_cp;
	char *c;

	/* Get space for new conference */
	if (NULL == (tmp_cp = (struct conf_ent *) malloc
					(sizeof (struct conf_ent)))) {
		fprintf (stderr, "%s: out of memory\n", progname);
		exit(-1);
	}

	/* Get space for name */
	if (NULL == (c = (char *) malloc (1+strlen(name)))) {
		fprintf (stderr, "%s: out of memory\n", progname);
		exit(-1);
	}

	/* Fill in conference name */
	tmp_cp->name = c;
	strcpy (tmp_cp->name, name);

	/* Fill in conference number */
	tmp_cp->number = num;

	/* Article count */
	tmp_cp->count = 0;

	/* Add to end of conference list */
	if (last_conf == NULL) {
		/* This is first conference */
		conf_list = tmp_cp;
	} else {
		last_conf->next = tmp_cp;
	}
	tmp_cp->next = NULL;
	last_conf = tmp_cp;

	if (slnp_mode) {
		/* Open SLNP message file */
		sprintf (msg_fn, "%s/%07d.MSG", home_dir, num);
		if (NULL == (msg_fd = fopen (msg_fn, "w"))) {
			fprintf (stderr, "%s: can't open %s\n",
					progname, msg_fn);
			exit (-1);
		}
	} else if (!zip_mode && !sum_mode) {
		/* Else open new QWK index file */
		if (!bw_kludge && !strcmp (name, MAIL_CONF_NAME)) {
			strcpy (ndx_fn, home_dir);
			strcat (ndx_fn, "/");
			strcat (ndx_fn, "personal.ndx");
		} else {
			sprintf (ndx_fn, "%s/%03d.ndx", home_dir, num);
		}

		if (NULL == (ndx_fd = fopen (ndx_fn, "w"))) {
			fprintf (stderr, "%s: can't open %s\n",
					progname, ndx_fn);
			exit (-1);
		}
	}

	/* Maintain conf_cnt: should always be one greater than highest
	   newsgroup conference number encountered */
	if (strcmp (name, MAIL_CONF_NAME)) {
		if (num >= conf_cnt) conf_cnt = num + 1;
	}

	/* Remember no summaries for this newsgroup yet */
	if (sum_mode) sum_flag = 0;

	return (tmp_cp);
}
