char *to;
/*
 *  Mail a warning to the user if the reply packet
 *  contains a message apparently for some other QWK
 *  "door" program.
 */
{
	FILE *pfd;

	/* Open pipe to mailer */
/*
	sprintf (buf, "%s -s 'UQWK Error Message' %s",
			MAILER_PATH, user_name);
*/
	sprintf (buf, "%s %s",
			SENDMAIL, user_name);
	if (NULL == (pfd = popen (buf, "w"))) {
		fprintf (stderr, "%s: can't popen() mail\n", progname);
		return(-1);
	}

	/* Send the message */

	fprintf (pfd,
		"Subject: UQWK Error Message\n\n"

		"Hello. You sent a message to the username %s, presumably to\n"
		"perform some sort of offline configuration. This QWK processor,\n"
		"called UQWK, cannot process this message. To perform offline\n"
		"configuration using UQWK, you must send a message to the username\n"
		"UQWK. Commands are to be included in the body of the message.\n"
		"For a list of commands, send a message to UQWK with the word\n"
		"HELP in the body of the message (not the subject). Thanks!\n",
		to);

	return(pclose (pfd));
}
