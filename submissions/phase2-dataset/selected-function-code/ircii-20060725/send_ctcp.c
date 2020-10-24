send_ctcp(u_char *type, u_char *to, u_char *datatag, char *format, ...)
{
	va_list vl;
	u_char putbuf[BIG_BUFFER_SIZE], sendbuf[BIG_BUFFER_SIZE];
	u_char *sendp;

	if (in_on_who)
		return;	/* Silently drop it on the floor */
	if (format)
	{
		va_start(vl, format);
		vsnprintf(CP(putbuf), sizeof putbuf, format, vl);
		va_end(vl);
		
		if (datatag)
		{
			snprintf(CP(sendbuf), sizeof sendbuf, "%c%s %s%c",
			    CTCP_DELIM_CHAR, datatag, putbuf, CTCP_DELIM_CHAR);
			sendp = sendbuf;
		}
		else
			sendp = putbuf;
	}
	else
	{
		snprintf(CP(sendbuf), sizeof sendbuf, "%c%s%c",
		    CTCP_DELIM_CHAR, datatag, CTCP_DELIM_CHAR);
		sendp = sendbuf;
	}

	/*
	 * ugh, special case dcc because we don't want to go through
	 * send_text in its current state.  XXX - fix send_text to
	 * deal with ctcp's as well.
	 */
	if (*to == '=')
		dcc_message_transmit(to + 1, sendp, DCC_CHAT, 0);
	else
		send_to_server("%s %s :%s", type, to, sendp);
}
