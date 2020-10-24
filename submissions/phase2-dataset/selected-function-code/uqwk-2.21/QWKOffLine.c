int bytes;
FILE *fd;
/*
 *  Process offline commands.  Message is open on fd.  We
 *  must be careful to leave the file pointer ready for the
 *  next message.
 */
{
	FILE *pfd;
	char c, cmd[PATH_LEN];
	int	ret = 0;

	/* Open mail pipe to send results back to user */
/*
	sprintf (buf, "%s -s 'Results of your request' %s",
			MAILER_PATH, user_name);
*/
	sprintf (buf, "%s %s",
			SENDMAIL, user_name);
	if (NULL == (pfd = popen (buf, "w"))) {
		fprintf (stderr, "%s: can't popen() mail\n", progname);
		while (bytes--) fread (&c, 1, 1, fd);
		return -1;
	}

	fprintf (pfd, "Subject: Results of your request\n\nHere are the results of your mail to UQWK:\n");

	/* Get lines, process them */
	while (GetLine (&bytes, fd)) {
		/* Echo command */
		fprintf (pfd, "\nCommand: %s\n", buf);

		/* Extract command */
		if (1 != sscanf (buf, "%s", cmd)) {
			fprintf (pfd, "Malformed command.\n");
			fprintf (stderr, "%s: Malformed offline command: \"%s\"\n",
				 progname, buf);
		} else {
			fprintf (stdout, "%s: performing offline command: \"%s\"\n", progname, buf);
			/* Look up command */
			if ( !strcasecmp (cmd, "HELP") ) {
				Help(pfd);
			} else if ( !strcasecmp (cmd, "SUBSCRIBE") ) {
				if((ret = Subscribe(pfd)) == -1)
					break;
			} else if ( !strcasecmp (cmd, "UNSUBSCRIBE") ) {
				if((ret = Unsubscribe(pfd)) == -1)
					break;
			} else if ( !strcasecmp (cmd, "GROUPS") ) {
				if((ret = Groups(pfd)) == -1)
					break;
			} else if ( !strcasecmp (cmd, "ALLGROUPS") ) {
				Allgroups(pfd);
			} else if ( !strcasecmp (cmd, "SHELL") ) {
				if((ret = Shell(pfd)) == -1)
					break;
			} else if ( !strcasecmp (cmd, "CATCHUP") ) {
				if((ret = Catchup(pfd)) == -1)
					break;
			} else {
				fprintf (pfd, "No such command.  ");
				fprintf (pfd, "Send HELP for help.\n");
			}
		}
	}

	fprintf (pfd, "\nEnd of commands.\n");
	pclose (pfd);
	return ret;
}
