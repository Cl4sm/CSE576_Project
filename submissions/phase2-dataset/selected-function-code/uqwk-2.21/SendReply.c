{
	FILE    *pfd;
	unsigned char    c;
        char    to[PATH_LEN], subject[PATH_LEN], group[PATH_LEN];
	int     i, n, blocks, bytes, conf;
	struct  nrc_ent *np;

	/* Extract recipient */
	strncpy (buf, rep_hdr.to, 25);
	buf[25] = 0;
	sscanf (buf, "%s", to);

	/* Extract conference number */
	strncpy (buf, rep_hdr.number, 7);
	buf[7] = 0;
	sscanf (buf, "%d", &conf);

	/* Extract subject */
	strncpy (buf, rep_hdr.subject, 25);
	buf[25] = 0;
	strcpy (subject, buf);

	/* Get rid of single quotes in subject */
	n = strlen (subject);
	for (i=0; i<n; i++) if (subject[i] == '\'') subject[i] = '`';

	/* Find newsgroup with this conference number */
	np = nrc_list;
	while (np != NULL) {
		if (np->conf == conf) break;
		np = np->next;
	}

	/* Get newsgroup name */
	if (np == NULL) {
		/* Bet this generates lots of email for "ALL" */
		rep_hdr.status = QWK_PRIVATE;
	} else {
		strcpy (group, np->name);
	}

	/* Extract block count */
	strncpy (buf, rep_hdr.blocks, 6);
	buf[6] = 0;
	sscanf (buf, "%d", &blocks);
	blocks -= 1;
	bytes = 128 * blocks;

	/* Check for off-line command message */
	if ( (!strcmp (to, "uqwk")) || (!strcmp (to, "UQWK")) )
		return(QWKOffLine (bytes, rep_fd));

	/* Check for a configuration message intended for some
	   other QWK "door" */
	if (	(!strcasecmp (to, "MARKMAIL"))	||
		(!strcasecmp (to, "QMAIL"))	||
		(!strcasecmp (to, "ROSEMAIL"))	||
		(!strcasecmp (to, "KMAIL"))	||
		(!strcasecmp (to, "MAINMAIL"))	||
		(!strcasecmp (to, "CMPMAIL"))	||
		(!strcasecmp (to, "ULTRABBS"))	||
		(!strcasecmp (to, "BGQWK"))	||
		(!strcasecmp (to, "CAM-MAIL"))	||
		(!strcasecmp (to, "TRIMAIL"))	||
		(!strcasecmp (to, "QSO"))	) {
		/* Send warning to user */
		if((n=SendWarning(to)) <= 0) return(n);

		/* Skip the rest of the message */
		while (bytes--) fread (&c, 1, 1, rep_fd);

		return (0);
	}

	/* Check for a "To: " line in the body of the message */
	CheckTo (to, bytes);

	/* Open pipe to proper program */
	pfd = NULL;

/*
 *	QWK is beyond hope -- don't even bother basic GNKSA-checking :-(
 */

	if ( (rep_hdr.status == QWK_PUBLIC) ||
	     (rep_hdr.status == QWK_PUBLIC2) ) {
		/* Public message, open pipe to inews */
		if (xprt_mode) {
			sprintf (buf, "%s", posting_agent);
		} else {
#ifdef NNTP
			sprintf (buf, "%s", inews_path);
#else
			sprintf (buf, "%s -t '%s' -n '%s'",
				inews_path, subject, group);
#endif
		}
		printf ("%s: Posting to %s\n", progname, group);
		if (NULL == (pfd = popen (buf, "w"))) {
			fprintf (stderr, "%s: can't popen() %s\n",
					progname, inews_path);
			return(-1);
		}
#ifdef NNTP
		if (!xprt_mode && (pfd != NULL) ) {
			fprintf (pfd, "Newsgroups: %s\nSubject: %s\n\n",
				group, subject);
		}
#endif
	} else if ( (rep_hdr.status == QWK_PRIVATE) ||
	          (rep_hdr.status == QWK_PRIVATE2) ) {
		/* Open pipe to mail */
		if (xprt_mode) {
			sprintf (buf, "%s", XPRT_MAILER);
		} else {
			sprintf (buf, "%s '%s'",
				SENDMAIL, to);
		}
		printf ("%s: Mailing to %s\n", progname, to);
		if (NULL == (pfd = popen (buf, "w"))) {
			fprintf (stderr, "%s: can't popen() %s\n", progname, buf);
			return(-1);
		}
		fprintf(pfd,"Subject: %s\n", subject);
	}

	/* Read and send all bytes of message */
	for (i=0; i<bytes; i++) {
		fread (&c, 1, 1, rep_fd);
		if (c == QWK_EOL) c = 012;
		if( (pfd != NULL) && (fwrite(&c, 1, 1, pfd) != 1)) return(-1);
	}

	return(pclose (pfd));
}
