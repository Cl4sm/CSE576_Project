static struct srv_rr *parse_srv_rr(unsigned int logopt,
				   u_char *buffer, u_char *start, u_char *end,
				   struct rr *rr, struct srv_rr *srv)
{
	u_char *cp = start;
	char ebuf[MAX_ERR_BUF];
	char tmp[MAXDNAME];
	int len;

	GETSHORT(srv->priority, cp);
	GETSHORT(srv->weight, cp);
	GETSHORT(srv->port, cp);
	srv->ttl = rr->ttl;

	len = dn_expand(buffer, end, cp, tmp, MAXDNAME);
	if (len < 0) {
		error(logopt, "failed to expand name");
		return NULL;
	}
	srv->name = strdup(tmp);
	if (!srv->name) {
		char *estr = strerror_r(errno, ebuf, MAX_ERR_BUF);
		error(logopt, "strdup: %s", estr);
		return NULL;
	}

	return srv;
}