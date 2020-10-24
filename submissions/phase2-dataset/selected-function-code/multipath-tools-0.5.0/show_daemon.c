show_daemon (char ** r, int *len)
{
	char * c;
	char * reply;

	unsigned int maxlen = INITIAL_REPLY_LEN;
	reply = MALLOC(maxlen);

	if (!reply)
		return 1;

	c = reply;
	c += snprintf(c, INITIAL_REPLY_LEN, "pid %d %s\n",
		      daemon_pid, daemon_status());

	*r = reply;
	*len = (int)(c - reply + 1);
	return 0;
}
