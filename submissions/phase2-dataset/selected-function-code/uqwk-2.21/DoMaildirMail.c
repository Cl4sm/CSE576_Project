int DoMaildirMail()
{
	struct conf_ent *cp;
	struct mail_ent me;
	DIR             *dirp;
	struct dirent   *d;
	char            *endp;
	char            tmp[PATH_LEN];
	filelist	    *mfs = NULL, *l;

	/* get name of directory */
	strcpy(tmp, mail_file);
	endp = strend(tmp);
	if (endp > tmp && endp[-1] == '/')
		*--endp = '\0';

	/* Do new directory */
	strcpy(endp, "/new/");
	if ((dirp = opendir(tmp))) { 			/* open directory */
		while ((d = readdir (dirp))) {		/* empty the directory */
			if (d->d_name[0] != '.') {	/* skip ., .., and .files */
				strcpy(endp+5, d->d_name);
				mfs = AddMaildirfile(mfs, tmp);
			}
		}
		closedir (dirp);	/* flush directory */
	}

	/* Do cur directory */
	strcpy(endp, "/cur/");
	if ((dirp = opendir(tmp))) { 			/* open directory */
		while ((d = readdir (dirp))) {		/* empty the directory */
			if (d->d_name[0] != '.') {	/* skip ., .., and .files */
				strcpy(endp+5, d->d_name);
				mfs = AddMaildirfile(mfs, tmp);
			}
		}
		closedir (dirp);	/* flush directory */
	}

	if (!mfs) {
		fprintf (stdout, "no mail for %s\n", user_name);
		return(0);
	}

	/* Define the mail "conference" */
	cp = NewConference (MAIL_CONF_NAME, conf_cnt);


	/* Open ZipNews mail file */
	if (zip_mode && (mail_list != NULL)) OpenZipMail();

	printf ("maildir mailbox (%s)", mail_file);
	/* Walk through all the messages */


	/* handle list of messages */
	me.next = NULL;
	for(l = mfs; l; l = l->tail) {
		long ln;
		char *uidl, *s;
		char xheaders[128] = {0};
		char status[16]    = {0};
		char xstatus[16]   = {0};

		if (!l->fnam)
			continue;

		ln = filesize(l->fnam);
		uidl = (char *) strrchr(l->fnam, '/');
		if(uidl && !strncmp(uidl-3, "cur", 3))
			strcat(status, "O");
		uidl++;
		s = (char *) strchr(uidl, ':');
		if(s != NULL) {
			*s++ = 0;
			/* Add (X-)Status codes */
			if(strchr(s, 'S'))
				strcat(status,  "R");
			if(strchr(s, 'R')) {
				strcat(status,  "r");
				strcat(xstatus, "A");
			}
			if(strchr(s, 'F'))
				strcat(xstatus, "F");
			if(strchr(s, 'T'))
				strcat(xstatus, "D");
		}
		sprintf(xheaders, "X-UIDL: %s\n", uidl?uidl:"unknown");
		if(s != NULL)
			*(s-1) = ':';
		if(*status) {
			strcat(xheaders, "Status: ");
			strcat(xheaders, status);
			strcat(xheaders, "\n");
		}
		if(*xstatus) {
			strcat(xheaders, "X-Status: ");
			strcat(xheaders, xstatus);
			strcat(xheaders, "\n");
		}
		if(ln > 0) {
			FILE *fd;

			/* Open the message file */
			if (NULL == (fd = fopen(l->fnam, "r"))) {
				fprintf (stderr, "could not open %s\n", l->fnam);
				exit(1);
			}
			cp->count++;
			me.begin = 0;
			me.end   = ln;
			AddMessage(fd, &me, xheaders);
			if(fclose(fd)==EOF) {
				fprintf(stderr, "error closing file %s\n", l->fnam);
				exit(1);
			}
		}
	}

	printf(" %d message%s\n", cp->count, cp->count==1?"":"s");
	if (!slnp_mode && !zip_mode) fclose (ndx_fd);
	if (slnp_mode) {
		fclose (msg_fd);
		msg_fd = NULL;
	}
	if (zip_mode && (mail_list != NULL)) fclose (mai_fd);

	/* in read only mode we're done now */
	if (read_only) {
		free_filelist(mfs);
		return 1;
	}

	/* Now empty the mail box */
	for(l = mfs; l; l = l->tail) {
		if (l->fnam)
			unlink(l->fnam);
	}

	/* purge old junk from tmp */
	strcpy(endp, "/tmp/");
	if ((dirp = opendir(tmp))) { 			/* open directory */
		while ((d = readdir (dirp))) {
			if ((d->d_name[0] != '.')) {	/* skip ., .., and .files */
				strcpy(endp+5, d->d_name);
				if (filetime(tmp) < (time(NULL)-(36L*60*60))
				&&  access(tmp,W_OK)==0) {
					unlink(tmp);
				}
			}
		}
		closedir (dirp);	/* flush directory */
	}

	free_filelist(mfs);
	return 1;
}
