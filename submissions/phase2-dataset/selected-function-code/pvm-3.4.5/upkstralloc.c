upkstralloc(mp, ss)
	struct pmsg *mp;		/* message to unpack */
	char **ss;				/* return pointer */
{
	int cc;
	int l;

	if (!(cc = (mp->m_codef->dec_int) (mp, (void*)&l, 1, 1, sizeof(int)))) {
		if (l <= 0)
			cc = PvmNoData;
		else {
			*ss = TALLOC(l, char, "ustr");
			if ((cc = (mp->m_codef->dec_byte) (mp, (void*)*ss, l, 1, 1)) < 0) {
				PVM_FREE(*ss);
				*ss = 0;
			}
		}
	}
	return cc;
}
