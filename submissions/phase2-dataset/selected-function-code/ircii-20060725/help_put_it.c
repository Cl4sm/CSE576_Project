help_put_it(u_char *topic, char *format, ...)
{
	va_list vl;
	PUTBUF_INIT
	int	lastlog_level;
	va_start(vl, format);
	PUTBUF_SPRINTF(format, vl)
	va_end(vl);

	in_help = 1;
	lastlog_level = set_lastlog_msg_level(LOG_HELP);
	if (do_hook(HELP_LIST, "%s %s", topic, putbuf))
	{
		if (window_display)
		{
			add_to_log(irclog_fp, putbuf);
			add_to_screen(putbuf);
		}
	}
	(void) set_lastlog_msg_level(lastlog_level);
	in_help = 0;
	PUTBUF_END
}
