dec_xdr_step(mp)
	struct pmsg *mp;
{
	struct frag *fp;
	int cc;
	int l;
	char *p;

	if (mp->m_cpos != mp->m_cfrag->fr_len) {
		p = mp->m_cfrag->fr_dat + mp->m_cpos;
		l = mp->m_cfrag->fr_len - mp->m_cpos;

		if (cc = pmsg_decmore(mp))
			return cc;

		fp = mp->m_cfrag;
		if (fp->fr_dat - fp->fr_buf < l) {
			pvmlogerror("aaugh, no space for fixup, kill me\n");
			return PvmBadMsg;

		} else {
			/*
			pvmlogprintf("fragment has %d bytes left, doing fixup\n",
					l);
			*/
			fp->fr_dat -= l;
			fp->fr_len += l;
			BCOPY(p, fp->fr_dat, l);
		}

	} else {
		if (cc = pmsg_decmore(mp))
			return cc;
		fp = mp->m_cfrag;
	}
	xdrmem_create(&mp->m_xdr, fp->fr_dat, (unsigned)(fp->fr_len),
			XDR_DECODE);
	return 0;
}
