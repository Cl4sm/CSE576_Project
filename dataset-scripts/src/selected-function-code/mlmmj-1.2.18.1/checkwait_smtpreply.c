char *checkwait_smtpreply(int sockfd, int replytype)
{
	char *smtpreply;

	smtpreply = mygetline(sockfd);
	if(smtpreply == NULL) {
		/* This will never be a valid SMTP response so will always be returned,
		 * but is more descriptive than an empty string. */
		smtpreply = mystrdup("none / error / closed connection");
	}

#if 0
	printf("replytype = [%d], smtpreply = [%s]\n", replytype, smtpreply);
	fprintf(stderr, "%s", smtpreply);
#endif

	/* This case might seem like (and is ATM) total overkill. But it's
	 * easy for us to extend it later on if needed.
	 */
	switch(replytype) {
		case MLMMJ_CONNECT:
			if(smtpreply[0] != '2' || smtpreply[1] != '2')
				return smtpreply;
			break;
		case MLMMJ_HELO:
			if(smtpreply[0] != '2' || smtpreply[1] != '5')
				return smtpreply;
			break;
		case MLMMJ_FROM:
			if(smtpreply[0] != '2' || smtpreply[1] != '5')
				return smtpreply;
			break;
		case MLMMJ_RCPTTO:
			if(smtpreply[0] != '2' || smtpreply[1] != '5')
				return smtpreply;
			break;
		case MLMMJ_DATA:
			if(smtpreply[0] != '3' || smtpreply[1] != '5')
				return smtpreply;
			break;
		case MLMMJ_DOT:
			if(smtpreply[0] != '2' || smtpreply[1] != '5')
				return smtpreply;
			break;
		case MLMMJ_QUIT:
			if(smtpreply[0] != '2' || smtpreply[1] != '2')
				return smtpreply;
			break;
		case MLMMJ_RSET:
			if(smtpreply[0] != '2' || smtpreply[1] != '5')
				return smtpreply;
			break;
		default:
			break;
	}

	myfree(smtpreply);

	return NULL;
}