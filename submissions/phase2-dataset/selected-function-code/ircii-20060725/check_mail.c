check_mail()
{
#if !defined(AMS_MAIL) && !defined(UNIX_MAIL)
	return	(u_char *) 0;
#else
	static	unsigned int	cnt = 0;
	static	time_t	old_stat = 0L;
	static	u_char	ret_str[8];
	struct	stat	stat_buf;
	unsigned int	new_cnt = 0;
	u_char	tmp[8];
	static	int	VirginProgram = 1;  /* It's its first time */
	int	lastlog_level;
	u_char	buffer[BIG_BUFFER_SIZE];
#ifdef UNIX_MAIL
	int	des;
	int	blanks = 1;
	int	reset_blanks = 0;
#endif /* UNIX_MAIL */

#ifdef DAEMON_UID
	if (getuid()==DAEMON_UID)
		return ((u_char *) 0);
#endif /* DAEMON_UID */

	init_mail();
#ifdef UNIX_MAIL
	if (stat(CP(mail_path), &stat_buf) == -1)
		return ((u_char *) 0);
	lastlog_level = set_lastlog_msg_level(LOG_CRAP);
	save_message_from();
	message_from((u_char *) 0, LOG_CURRENT);
	if (stat_buf.st_ctime > old_stat)
	{
		old_stat = stat_buf.st_ctime;
		if ((des = open(CP(mail_path), O_RDONLY, 0)) >= 0)
		{
			new_cnt = 0;
			while (dgets(buffer, sizeof buffer, des,(u_char *) 0)>0)
			{
				if (buffer[0] == '\n') {
					blanks++;
					continue;
				}
				else
					reset_blanks = 1;
				if (!my_strncmp(MAIL_DELIMITER, buffer, sizeof(MAIL_DELIMITER) - 1) && blanks)
				{
					new_cnt++;
					if (new_cnt > cnt && !VirginProgram && get_int_var(MAIL_VAR) == 2)
					{
						while (dgets(buffer, sizeof buffer, des, (u_char *) 0) > 0 && *buffer != '\0' && *buffer != '\n')
						{
							buffer[my_strlen(buffer)-1] = '\0';
							if (!my_strncmp(buffer, "From:", 5) || !my_strncmp(buffer, "Subject:", 8))
								say("%s", buffer);
						}
					}
				}
				if (reset_blanks)
					reset_blanks = blanks = 0;
			}
			VirginProgram=0;
			new_close(des);
		}
#else
# ifdef AMS_MAIL
		if (stat(mail_path, &stat_buf) == -1)
		{
			set_lastlog_msg_level(lastlog_level);
			return ((u_char *) 0);
		}
		if (stat_buf.st_ctime > old_stat)
		{
			new_cnt = count_files(mail_path, old_stat);
			old_stat = stat_buf.st_ctime;
		}
# endif /* AMS_MAIL */
#endif /* UNIX_MAIL */
		/* yeeeeack */
		if (new_cnt > cnt)
		{
			snprintf(CP(tmp), sizeof tmp, "%d", new_cnt - cnt);
			snprintf(CP(buffer), sizeof buffer, "%d", new_cnt);
			if (do_hook(MAIL_LIST, "%s %s", tmp, buffer) && get_int_var(MAIL_VAR) == 1)
				say("You have new email.");
		}
		cnt = new_cnt;
	}
	set_lastlog_msg_level(lastlog_level);
	restore_message_from();
	if (cnt && (cnt < 65536))
	{
		snprintf(CP(ret_str), sizeof ret_str, "%d", cnt);
		return (ret_str);
	}
	else
		return ((u_char *) 0);
#endif /* !defined(AMS_MAIL) && !defined(UNIX_MAIL) */
}
