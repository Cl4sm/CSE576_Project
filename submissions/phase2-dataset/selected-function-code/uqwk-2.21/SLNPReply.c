{
	char fname[PATH_LEN], kind[PATH_LEN], type[PATH_LEN];
	int rval=0;

	/* Look through lines in REPLIES file */
	while (Fgets (buf, BUF_LEN, rep_fd)) {
		if (3 != sscanf (buf, "%s %s %s", fname, kind, type)) {
			fprintf (stderr, "%s: malformed REPLIES line\n",
					progname);
			return (-1);
		}

		/* Look for mail or news */
		if (strcmp(kind,"mail") && strcmp(kind,"news")) {
			fprintf (stderr, "%s: bad reply kind: %s\n",
					progname, kind);
		}

		/* Check reply type */
		else if ( (type[0] != 'u') &&
			  (type[0] != 'b') &&
			  (type[0] != 'B') ) {
			fprintf (stderr, "%s: reply type %c not supported\n",
					progname, type[0]);
		} else {
			/* Make file name */
			strcat (fname, ".MSG");

			/* Process it */
			switch (type[0]) {
			case 'u':
				if( !strcmp(kind, "mail")) rval=SLuMail(fname);
				if( !strcmp(kind, "news")) rval|=SLuNews(fname);
				break;

			case 'b':
			case 'B':
				if( !strcmp(kind, "mail")) rval=SLbMail(fname);
				if( !strcmp(kind, "news")) rval|=SLbNews(fname);
				break;
			}

			/* Delete it */
			if (!read_only) unlink (fname);
		}
	}
	return(rval);
}
