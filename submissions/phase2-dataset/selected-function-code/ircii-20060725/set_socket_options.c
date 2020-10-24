set_socket_options(s)
	int	s;
{
#ifndef _Windows
#ifndef NO_STRUCT_LINGER
	struct linger	lin;
#endif /* !NO_STRUCT_LINGER */
	int	opt = 1;
	int	optlen = sizeof(opt);

	(void) setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, optlen);
	opt = 1;
	(void) setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char *) &opt, optlen);
#ifndef NO_STRUCT_LINGER
	lin.l_onoff = lin.l_linger = 0;
	(void) setsockopt(s, SOL_SOCKET, SO_LINGER, (char *) &lin, optlen);
#endif /* NO_STRUCT_LINGER */
#endif /* !Windows */
}
