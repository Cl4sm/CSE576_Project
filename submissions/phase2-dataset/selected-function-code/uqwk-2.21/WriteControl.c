/*
 *  Create the CONTROL.DAT file (or AREAS if SLNP)
 */
{
	struct conf_ent *cp;
	struct tm *t;
	char ctl_fname[PATH_LEN];
	time_t clock;
	int n;

	if(!conf_list)
		return 0;

	strcpy (ctl_fname, home_dir);
	strcat (ctl_fname, "/");

	if (slnp_mode)
		strcat (ctl_fname, "AREAS");
	else
		strcat (ctl_fname, "control.dat");

	if (NULL == (ctl_fd = fopen (ctl_fname, "w"))) {
		fprintf (stderr, "%s: can't open %s\n", progname, ctl_fname);
		return -1;	/* Return failure error code to caller */
	}

	/* SLNP AREAS file is different */
	if (slnp_mode)
		return WriteAreas();

	if(fprintf(ctl_fd, "%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n",
		bbs_name, bbs_city, bbs_phone, bbs_sysop, bbs_id) < 0
	&& ferror(ctl_fd)) {
		fprintf(stderr,"%s: can't write to %s\n",
			progname, ctl_fname);
		return -1;
	}

	/* Date */
	clock = time (NULL);
	t = gmtime (&clock);
	if(fprintf(ctl_fd, "%02d-%02d-%04d,%02d:%02d:%02d\r\n",
		t->tm_mon+1, t->tm_mday, t->tm_year+1900,
		t->tm_hour, t->tm_min, t->tm_sec) < 0
	&& ferror(ctl_fd)) {
		fprintf(stderr,"%s: can't write to %s\n",
			progname, ctl_fname);
		return -1;
	}

	if(fprintf (ctl_fd, "%s\r\n \r\n0\r\n", user_name) < 0
	&& ferror(ctl_fd)) {
		fprintf(stderr,"%s: can't write to %s\n",
			progname, ctl_fname);
		return -1;
	}

	/* Count conferences with articles */
	n = 0;
	cp = conf_list;
	while (cp != NULL) {
		if (every_mode || (cp->count > 0)) n++;
		cp = cp->next;
	}
	if(fprintf (ctl_fd, "%d\r\n%d\r\n", msg_cnt, n-1) < 0
	&& ferror(ctl_fd)) {
		fprintf(stderr,"%s: can't write to %s\n",
			progname, ctl_fname);
		return -1;
	}

	/* List of conferences */
	cp = conf_list;
	while (cp != NULL) {
		if (every_mode || (cp->count > 0)) {
			strcpy (buf, cp->name);

			/* Translate the name if desired */
			if (trn_list != NULL) TransName (buf);

			/* Truncate the group name if desired */
			if ( (grp_len > 0) && (grp_len < BUF_LEN) )
				buf[grp_len] = 0;

			if(fprintf(ctl_fd, "%d\r\n%s\r\n", cp->number, buf) < 0
			&& ferror(ctl_fd)) {
				fprintf(stderr,"%s: can't write to %s\n",
					progname, ctl_fname);
				return -1;
			}
		}
		cp = cp->next;
	}

	if(fprintf (ctl_fd, "WELCOME.DAT\r\nNEWS.DAT\r\nLOGOFF.DAT\r\n\032") < 0
	&& ferror(ctl_fd)) {
		fprintf(stderr,"%s: can't write to %s\n",
			progname, ctl_fname);
		return -1;
	}
	fclose (ctl_fd);
	return 0;
}
