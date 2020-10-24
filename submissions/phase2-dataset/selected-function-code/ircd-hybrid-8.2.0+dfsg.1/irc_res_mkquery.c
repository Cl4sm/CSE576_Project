int
irc_res_mkquery(
	     const char *dname,		/* domain name */
	     int class, int type,	/* class and type of query */
	     unsigned char *buf,		/* buffer to put query */
	     int buflen)		/* size of buffer */
{
	HEADER *hp;
	unsigned char *cp;
	int n;
	const unsigned char *dnptrs[20], **dpp, **lastdnptr;

	/*
	 * Initialize header fields.
	 */
	if ((buf == NULL) || (buflen < HFIXEDSZ))
		return (-1);
	memset(buf, 0, HFIXEDSZ);
	hp = (HEADER *) buf;

	hp->id = 0;
	hp->opcode = QUERY;
	hp->rd = 1;		/* recurse */
	hp->rcode = NO_ERRORS;
	cp = buf + HFIXEDSZ;
	buflen -= HFIXEDSZ;
	dpp = dnptrs;
	*dpp++ = buf;
	*dpp++ = NULL;
	lastdnptr = dnptrs + sizeof dnptrs / sizeof dnptrs[0];

	if ((buflen -= QFIXEDSZ) < 0)
	  return (-1);
	if ((n = irc_ns_name_compress(dname, cp, buflen, dnptrs,
                                                         lastdnptr)) < 0)
	  return (-1);

	cp += n;
	buflen -= n;
	IRC_NS_PUT16(type, cp);
	IRC_NS_PUT16(class, cp);
	hp->qdcount = htons(1);

	return (cp - buf);
}
