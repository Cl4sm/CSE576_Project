msgid_put_header(mp)
	struct msgid *mp;
{

	if (BADMAGIC(LMSG_MSGID, mp->magic))
	{
		pvmlogerror("msgid_put_header(): bad magic number \n");
		pvmbailout(0);
		return PvmSysErr;
	}			
#ifdef	CLUMP_ALLOC
	if (nummsgids == 0)
		freemsgids.ms_link = freemsgids.ms_rlink = &freemsgids;
	LISTPUTBEFORE(&freemsgids, mp, ms_link, ms_rlink);
	nummsgids++;

#else
	PVM_FREE(mp);
#endif
	return 0;
}
