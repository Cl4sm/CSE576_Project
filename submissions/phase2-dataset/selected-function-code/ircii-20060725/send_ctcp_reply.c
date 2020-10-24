send_ctcp_reply(u_char *to, u_char *datatag, char *format, ...)
{
	va_list vl;
	u_char	putbuf[BIG_BUFFER_SIZE];

	if (in_on_who)
		return;	/* Silently drop it on the floor */
	if (to && (*to == '='))
		return;	/* don't allow dcc replies */
	my_strmcat(CTCP_Reply_Buffer, "\001", sizeof CTCP_Reply_Buffer);
	my_strmcat(CTCP_Reply_Buffer, datatag, sizeof CTCP_Reply_Buffer);
	my_strmcat(CTCP_Reply_Buffer, " ", sizeof CTCP_Reply_Buffer);
	if (format)
	{
		va_start(vl, format);
		vsnprintf(CP(putbuf), sizeof putbuf, format, vl);
		va_end(vl);
		my_strmcat(CTCP_Reply_Buffer, putbuf, BIG_BUFFER_SIZE);
	}
	else
		my_strmcat(CTCP_Reply_Buffer, putbuf, sizeof CTCP_Reply_Buffer);
	my_strmcat(CTCP_Reply_Buffer, "\001", sizeof CTCP_Reply_Buffer);
}
