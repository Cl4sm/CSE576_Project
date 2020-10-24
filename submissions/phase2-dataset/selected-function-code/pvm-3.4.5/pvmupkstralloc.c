int
pvmupkstralloc(ss)
	char **ss;				/* return pointer */
{
	int cc;
	int l;

	if (!pvmrbuf)
		cc = PvmNoBuf;
	else {
		if (!(cc = (pvmrbuf->m_codef->dec_int)
				(pvmrbuf, (void*)&l, 1, 1, sizeof(int)))) {
			*ss = TALLOC(l, char, "pustr");
			if ((cc = (pvmrbuf->m_codef->dec_byte)
					(pvmrbuf, (void*)*ss, l, 1, 1)) < 0) {
				PVM_FREE(*ss);
				*ss = 0;
			}
		}
	}
	return cc;
}
