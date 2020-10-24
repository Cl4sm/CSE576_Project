msendrecv(other, code, context)
	int other;				/* dst, src tid */
	int code;				/* message tag */
	int context;            /* context to use for the message */
{
	static int nextwaitid = 1;	/* I hope */

	int cc;
	struct pmsg *up;

	if (!pvmsbuf)
		return PvmNoBuf;

	/* send code to other */
	if (pvmdebmask & PDMMESSAGE) {
		sprintf(pvmtxt, "msendrecv() to t%x tag %s\n", other,
				pvmnametag(code, (int *)0));
		pvmlogerror(pvmtxt);
	}

   pvmsbuf->m_wid = nextwaitid++;
   pvmsbuf->m_ctx = context;

   if ((cc = mroute(pvmsbuf->m_mid, other, code, (struct timeval *)0)) < 0)
		return cc;

	/* Oops now what? */
	/* 	if (code == TM_MCA)		 */
		/* for node_mcast() */
	/* 		return 1; */

	/* recv code from other */
	for (up = pvmrxlist->m_link; 1; up = up->m_link) {
		if (up == pvmrxlist) {
			up = up->m_rlink;
			if ((cc = mroute(0, 0, 0, (struct timeval *)0)) < 0)
				return cc;
			up = up->m_link;
		}

		if (pvmdebmask & PDMMESSAGE) {
			sprintf(pvmtxt, "msendrecv() from t%x tag %s\n",
					up->m_src, pvmnametag(up->m_tag, (int *)0));
			pvmlogerror(pvmtxt);
		}
		if (up->m_src == other && up->m_tag == code)
			break;
	}
	LISTDELETE(up, m_link, m_rlink);
	if (pvmrbuf)
		umbuf_free(pvmrbuf);
	pvmrbuf = 0;
	if (cc = pvm_setrbuf(up->m_mid))
		return cc;
	return up->m_mid;
}
