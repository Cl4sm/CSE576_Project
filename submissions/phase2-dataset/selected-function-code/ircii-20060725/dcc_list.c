dcc_list(args)
	u_char	*args;
{
	DCC_list	*Client;
	unsigned	flags;
	int	lastlog_level;

	lastlog_level = set_lastlog_msg_level(LOG_DCC);
	if (do_hook(DCC_LIST_LIST, DCC_FORM_HOOK, DCC_FORM_HEADER))
		put_it(DCC_FORM, DCC_FORM_HEADER);
	for (Client = ClientList ; Client != NULL ; Client = Client->next)
	{
		u_char	sent[9],
			rd[9];
		u_char	*timestr;

		snprintf(CP(sent), sizeof sent, "%ld", (long)Client->bytes_sent);
		snprintf(CP(rd), sizeof rd, "%ld", (long)Client->bytes_read);
		timestr = (Client->starttime) ? dcc_time(Client->starttime) : empty_string;
		flags = Client->flags;

#ifdef DCC_DCNT_PEND
#define DCC_CONT_PEND_FORM	flags & DCC_CNCT_PEND ?	"Connecting" :
#else /* DCC_DCNT_PEND */
#define DCC_CONT_PEND_FORM	/* nothing */
#endif /* DCC_DCNT_PEND */

#define DCC_FORM_BODY		dcc_types[flags & DCC_TYPES],		\
				Client->user,				\
				flags & DCC_OFFER ? "Offered" :		\
				flags & DCC_DELETE ? "Closed" :		\
				flags & DCC_ACTIVE ? "Active" :		\
				flags & DCC_WAIT ? "Waiting" :		\
				DCC_CONT_PEND_FORM			\
				"Unknown",				\
				timestr,				\
				sent,					\
				rd,					\
				Client->description

		if (do_hook(DCC_LIST_LIST, DCC_FORM_HOOK, DCC_FORM_BODY))
			put_it(DCC_FORM, DCC_FORM_BODY);
		if (*timestr)
			new_free(&timestr);
	}
	(void) set_lastlog_msg_level(lastlog_level);
}
