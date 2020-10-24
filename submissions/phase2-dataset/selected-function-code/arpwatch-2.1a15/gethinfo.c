int
gethinfo(register char *hostname, register char *cpu, register int cpulen,
    register char *os, register int oslen)
{
#ifdef HAVE_DN_SKIPNAME
	register querybuf *qb;
	register u_char *cp, *eom;
	register char *bp;
	register int n;
	register HEADER *hp;
	register int type, class, buflen, ancount, qdcount;
	querybuf qbuf;

	qb = &qbuf;
	n = res_query(hostname, C_IN, T_HINFO, qb->buf, sizeof(qb->buf));
	if (n < 0)
		return (0);

	eom = qb->buf + n;
	/*
	 * find first satisfactory answer
	 */
	hp = &qb->hdr;
	ancount = ntohs(hp->ancount);
	qdcount = ntohs(hp->qdcount);
	bp = hostbuf;
	buflen = sizeof(hostbuf);
	cp = qb->buf + sizeof(HEADER);
	if (qdcount) {
		cp += dn_skipname(cp, eom) + QFIXEDSZ;
		while (--qdcount > 0)
			cp += dn_skipname(cp, eom) + QFIXEDSZ;
	}
	while (--ancount >= 0 && cp < eom) {
		if ((n = dn_expand((u_char *)qb->buf, (u_char *)eom,
		    (u_char *)cp, (u_char *)bp, buflen)) < 0)
			break;
		cp += n;
		type = _getshort(cp);
 		cp += sizeof(u_short);
		class = _getshort(cp);
 		cp += sizeof(u_short) + sizeof(u_int32_t);
		n = _getshort(cp);
		cp += sizeof(u_short);
		if (type == T_HINFO) {
			/* Unpack */
			n = *cp++;
			if (n > cpulen - 1)
				return (0);
			BCOPY(cp, cpu, n);
			cp += n;
			cpu[n] = '\0';
			n = *cp++;
			if (n > oslen - 1)
				return (0);
			BCOPY(cp, os, n);
			os[n] = '\0';
			return (1);
		}
		/* Skip unexpected junk */
		cp += n;
	}
#endif
	return (0);
}