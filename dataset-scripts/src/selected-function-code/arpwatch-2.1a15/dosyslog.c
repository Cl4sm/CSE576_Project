void
dosyslog(register int p, register char *s, register u_int32_t a,
    register u_char *ea, register u_char *ha, char *interface)
{
	char xbuf[64];

	/* No report until we're initialized */
	if (initializing)
		return;

	/* Display both ethernet addresses if they don't match */
	(void)strcpy(xbuf, e2str(ea));
	if (ha != NULL && MEMCMP(ea, ha, 6) != 0) {
		(void)strcat(xbuf, " (");
		(void)strcat(xbuf, e2str(ha));
		(void)strcat(xbuf, ")");
	}

	if (debug)
		fprintf(stderr, "%s: %s %s %s %s\n", prog, s, intoa(a),
			xbuf, interface);
	else
		syslog(p, "%s %s %s %s", s, intoa(a), xbuf, interface);
}