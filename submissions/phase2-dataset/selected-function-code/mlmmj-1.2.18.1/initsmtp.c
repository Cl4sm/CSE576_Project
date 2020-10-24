int initsmtp(int *sockfd, const char *relayhost, unsigned short port)
{
	int retval = 0;
	char *reply = NULL;
	char *myhostname = hostnamestr();

	init_sockfd(sockfd, relayhost, port);

	if(*sockfd == -1)
		return EBADF;

	if((reply = checkwait_smtpreply(*sockfd, MLMMJ_CONNECT)) != NULL) {
		log_error(LOG_ARGS, "No proper greeting to our connect"
			  "Reply: [%s]", reply);
		myfree(reply);
		retval = MLMMJ_CONNECT;
		/* FIXME: Queue etc. */
	}	
	write_helo(*sockfd, myhostname);
	myfree(myhostname);
	if((reply = checkwait_smtpreply(*sockfd, MLMMJ_HELO)) != NULL) {
		log_error(LOG_ARGS, "Error with HELO. Reply: [%s]", reply);
		/* FIXME: quit and tell admin to configure correctly */
		myfree(reply);
		retval = MLMMJ_HELO;
	}

	return retval;
}