void
dcc_check(rd, wd)
	fd_set	*rd,
		*wd;
{
	DCC_list	**Client;
	struct	timeval	time_out;
	int	previous_server;
	int	lastlog_level;

	previous_server = from_server;
	from_server = (-1);
	time_out.tv_sec = time_out.tv_usec = 0;
	lastlog_level = set_lastlog_msg_level(LOG_DCC);
	for (Client = (&ClientList); *Client != NULL && !break_io_processing;)
	{
#ifdef NON_BLOCKING_CONNECTS
		/*
		 * run all connect-pending sockets.. suggested by deraadt@theos.com
		 */
		if ((*Client)->flags & DCC_CNCT_PEND)
		{
			SOCKADDR_STORAGE remaddr;
			socklen_t	rl = sizeof(remaddr);

			if (getpeername((*Client)->read, (struct sockaddr *) &remaddr, &rl) != -1)
			{
				if ((*Client)->flags & DCC_OFFER)
				{
					(*Client)->flags &= ~DCC_OFFER;
					save_message_from();
					message_from((*Client)->user, LOG_DCC);
					if (((*Client)->flags & DCC_TYPES) != DCC_RAW)
						say("DCC %s connection with %s[%s] established",
							dcc_types[(*Client)->flags&DCC_TYPES],
							(*Client)->user,
							dcc_sockname(&remaddr, rl));
					restore_message_from();
				}
				(*Client)->starttime = time(NULL);
				(*Client)->flags &= ~DCC_CNCT_PEND;
				set_blocking((*Client)->read);
				if ((*Client)->read != (*Client)->write)
					set_blocking((*Client)->write);
			} /* else we're not connected yet */
		}
#endif /* NON_BLOCKING_CONNECTS */
		if ((*Client)->read != -1 && FD_ISSET((*Client)->read, rd))
		{
			switch((*Client)->flags & DCC_TYPES)
			{
			case DCC_CHAT:
				process_incoming_chat(*Client);
				break;
			case DCC_RAW_LISTEN:
				process_incoming_listen(*Client);
				break;
			case DCC_RAW:
				process_incoming_raw(*Client);
				break;
			case DCC_FILEOFFER:
				process_outgoing_file(*Client);
				break;
			case DCC_FILEREAD:
				process_incoming_file(*Client);
				break;
			}
		}
		if ((*Client)->flags & DCC_DELETE)
		{
			dcc_add_deadclient(*Client);
			Client = (&(**Client).next);
		}
		else
			Client = (&(**Client).next);
	}
	(void) set_lastlog_msg_level(lastlog_level);
	dcc_really_erase();
	from_server = previous_server;
}
