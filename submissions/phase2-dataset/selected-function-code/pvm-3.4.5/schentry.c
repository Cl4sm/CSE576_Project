schentry(mp)
	struct pmsg *mp;
{
	int c = mp->m_tag;

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("schentry() from t%x tag %s\n", mp->m_src,
				pvmnametag(c, (int *)0));
	}

	if (c < (int)SM_FIRST || c > (int)SM_LAST) {
		pvmlogprintf("schentry() message from t%x with bogus tag %d\n",
				mp->m_src, c);
		goto bail;
	}

	c -= SM_FIRST;
	(smswitch[c])(mp);

bail:
	pmsg_unref(mp);
	return 0;
}
