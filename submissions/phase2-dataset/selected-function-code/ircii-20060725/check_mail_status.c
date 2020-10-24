int
check_mail_status()
{

#if defined(AMS_MAIL) || defined(UNIX_MAIL)
	struct	stat	stat_buf;
	static	time_t	old_stat = 0L;

#ifdef DAEMON_UID
	if (getuid() == DAEMON_UID)
		return 0;
#endif /* DAEMON_UID */

	if (!get_int_var(MAIL_VAR))
	{
		old_stat = 0L;
		return (0);
	}
	init_mail();
	if (stat(CP(mail_path), &stat_buf) == -1)
		return (0);
	if (stat_buf.st_ctime > old_stat)
	{
		old_stat = stat_buf.st_ctime;
		return (1);
	}
#endif /* defined(AMS_MAIL) || defined(UNIX_MAIL) */
	return (0);
}
