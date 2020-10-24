pmsg_get()
{
	struct pmsg *mp;
	int n;

#ifdef	CLUMP_ALLOC
	if (numpmsgs == 0) {
		freepmsgs.m_link = freepmsgs.m_rlink = &freepmsgs;
		if (!(mp = TALLOC(PMSG_CLUMP, struct pmsg, "pmsgs")))
			return (struct pmsg *)0;
		for (n = PMSG_CLUMP; n-- > 0; ) {
			LISTPUTBEFORE(&freepmsgs, mp, m_link, m_rlink);
			mp++;
		}
		numpmsgs = PMSG_CLUMP;
	}
	numpmsgs--;
	mp = freepmsgs.m_link;
	LISTDELETE(mp, m_link, m_rlink);

#else
	mp = TALLOC(1, struct pmsg, "pmsg");
#endif

	return mp;
}
