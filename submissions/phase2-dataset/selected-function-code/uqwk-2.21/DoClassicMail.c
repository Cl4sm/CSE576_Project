int DoClassicMail ()
{
	struct mail_ent *mailp;
	struct conf_ent *cp;

	/* No mail in summary mode */
	if (sum_mode) return (0);

	/* Open the mail spool */
	if (NULL == (mail_fd = fopen(mail_file, "r"))) {
		fprintf (stdout, "no mail in %s for %s\n", mail_file, user_name);
		return(0);
	}

	/* Construct the mail linked list */
	MakeMailList ();

	if (!mail_list) {
		fprintf (stdout, "no mail in %s for %s\n", mail_file, user_name);
		fclose(mail_fd);
		return(0);
	}

	/* Define the mail "conference" */
	cp = NewConference (MAIL_CONF_NAME, conf_cnt);


	/* Open ZipNews mail file */
	if (zip_mode && (mail_list != NULL)) OpenZipMail();

	printf ("classic mailbox (%s)", mail_file);

	/* Walk through all the messages */
	mailp = mail_list;
	while (mailp != NULL) {
		cp->count++;

		AddMessage(mail_fd, mailp, NULL);

		mailp = mailp->next;
	}

	fclose (mail_fd);
	printf(" %d message%s\n", cp->count, cp->count==1?"":"s");
	if (!slnp_mode && !zip_mode) fclose (ndx_fd);
	if (slnp_mode) {
		fclose (msg_fd);
		msg_fd = NULL;
	}
	if (zip_mode && (mail_list != NULL)) fclose (mai_fd);

	/* Now empty the mail box */
	if (!read_only) {
		if (NULL == (mail_fd = fopen (mail_file, "w"))) {
			fprintf (stderr, "%s: can't write %s\n", progname,
								mail_file);
			return(-1);
		} else {
			fclose (mail_fd);
		}
	}
	return(1);
}
