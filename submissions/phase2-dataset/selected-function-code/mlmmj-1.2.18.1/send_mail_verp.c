int send_mail_verp(int sockfd, struct strlist *addrs, char *mailmap,
		   size_t mailsize, const char *from, const char *listdir,
		   const char *hdrs, size_t hdrslen, const char *body,
		   size_t bodylen, const char *verpextra)
{
	int retval, i;
	char *reply, *reply2;

	if(sockfd == -1)
		return EBADF;

	retval = write_mail_from(sockfd, from, verpextra);
	if(retval) {
		log_error(LOG_ARGS, "Could not write MAIL FROM\n");
		return retval;
	}
	reply = checkwait_smtpreply(sockfd, MLMMJ_FROM);
	if(reply) {
		log_error(LOG_ARGS, "Error in MAIL FROM. Reply = [%s]",
				reply);
		myfree(reply);
		write_rset(sockfd);
		reply2 = checkwait_smtpreply(sockfd, MLMMJ_RSET);
		if (reply2 != NULL) myfree(reply2);
		return MLMMJ_FROM;
	}
	for(i = 0; i < addrs->count; i++) {
		if(gotsigterm) {
			log_error(LOG_ARGS, "TERM signal received, "
					"shutting down.");
			return -1;
		}
		if(strchr(addrs->strs[i], '@') == NULL) {
			errno = 0;
			log_error(LOG_ARGS, "No @ in address, ignoring %s",
					addrs->strs[i]);
			continue;
		}
		retval = write_rcpt_to(sockfd, addrs->strs[i]);
		if(retval) {
			log_error(LOG_ARGS, "Could not write RCPT TO:\n");
			return retval;
		}

		reply = checkwait_smtpreply(sockfd, MLMMJ_RCPTTO);
		if(reply) {
			log_error(LOG_ARGS, "Error in RCPT TO. Reply = [%s]",
					reply);
			myfree(reply);
			return MLMMJ_RCPTTO;
		}
	}

	retval = write_data(sockfd);
	if(retval) {
		log_error(LOG_ARGS, "Could not write DATA\b");
		return retval;
	}

	reply = checkwait_smtpreply(sockfd, MLMMJ_DATA);
	if(reply) {
		log_error(LOG_ARGS, "Error with DATA. Reply = [%s]", reply);
		myfree(reply);
		write_rset(sockfd);
		reply2 = checkwait_smtpreply(sockfd, MLMMJ_RSET);
		if (reply2 != NULL) myfree(reply2);
		return MLMMJ_DATA;
	}

	if(prepmailinmem) {
		retval = writen(sockfd, hdrs, hdrslen);
		if(retval < 0) {
			log_error(LOG_ARGS, "Could not write mailheaders.\n");
			return retval;
		}
		retval = writen(sockfd, body, bodylen);
		if(retval < 0) {
			log_error(LOG_ARGS, "Could not write mailbody.\n");
			return retval;
		}
	} else {
		retval = write_mailbody_from_map(sockfd, mailmap, mailsize,
						 NULL);
		if(retval) {
			log_error(LOG_ARGS, "Could not write mail\n");
			return retval;
		}
	}

	retval = write_dot(sockfd);
	if(retval) {
		log_error(LOG_ARGS, "Could not write <CR><LF>.<CR><LF>\n");
		return retval;
	}

	reply = checkwait_smtpreply(sockfd, MLMMJ_DOT);
	if(reply) {
		log_error(LOG_ARGS, "Mailserver did not ack end of mail.\n"
				"<CR><LF>.<CR><LF> was written, to no"
				"avail. Reply = [%s]", reply);
		myfree(reply);
		write_rset(sockfd);
		reply2 = checkwait_smtpreply(sockfd, MLMMJ_RSET);
		if (reply2 != NULL) myfree(reply2);
		return MLMMJ_DOT;
	}

	return 0;
}