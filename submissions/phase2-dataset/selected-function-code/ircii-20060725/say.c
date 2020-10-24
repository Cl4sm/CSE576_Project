say(char *format, ...)
{
	va_list vl;

	if (window_display)
	{
		u_char *fmt = (u_char *) 0;
		PUTBUF_INIT

		if (get_int_var(SHOW_STARS_VAR))
		{
			u_char *s;
			int flag = 0;

			s = expand_alias((u_char *) 0,
			    get_string_var(STAR_PREFIX_VAR),
			    empty_string, &flag, (u_char **) 0);

			if (s)
			{
				malloc_strcpy(&fmt, s);
				new_free(&s);
			}
			malloc_strcat(&fmt, UP(format));
			format = CP(fmt);
		}
		va_start(vl, format);
		PUTBUF_SPRINTF(format, vl)
		va_end(vl);
		add_to_log(irclog_fp, putbuf);
		add_to_screen(putbuf);
		PUTBUF_END
		if (fmt)
			new_free(&fmt);
	}
}
