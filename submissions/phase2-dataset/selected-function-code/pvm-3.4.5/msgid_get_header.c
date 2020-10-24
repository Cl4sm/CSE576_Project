static struct msgid *
msgid_get_header()
{
	struct msgid *mp;
	int n;

#ifdef	CLUMP_ALLOC
	if (nummsgids == 0) {
		freemsgids.ms_link = freemsgids.ms_rlink = &freemsgids;
		if (!(mp = TALLOC(MSGID_CLUMP, struct msgid, "mids")))
			return (struct msgid *)0;
		for (n = MSGID_CLUMP; n-- > 0; ) {
			LISTPUTBEFORE(&freemsgids, mp, ms_link, ms_rlink);
			mp++;
		}
		nummsgids = MSGID_CLUMP;
	}
	nummsgids--;
	mp = freemsgids.ms_link;
	LISTDELETE(mp, ms_link, ms_rlink);

#else
	mp = TALLOC(1, struct msgid, "mid");
#endif

	mp->magic = LMSG_MSGID;  /* set the magic number */
	return mp;
}
