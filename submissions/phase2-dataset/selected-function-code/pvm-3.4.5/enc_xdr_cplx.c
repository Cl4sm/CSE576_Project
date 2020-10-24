enc_xdr_cplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register float *xp;
	int cc = 0;

	std = std * 2 - 1;
	for (xp = (float*)vp; cnt-- > 0; xp += std) {
		if (!xdr_float(&mp->m_xdr, xp)) {
			mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
			if (cc = enc_xdr_step(mp))
				break;
			else
				if (!xdr_float(&mp->m_xdr, xp)) {
					cc = PvmNoMem;
					break;
				}
		}
		xp++;
		if (!xdr_float(&mp->m_xdr, xp)) {
			mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
			if (cc = enc_xdr_step(mp))
				break;
			else
				if (!xdr_float(&mp->m_xdr, xp)) {
					cc = PvmNoMem;
					break;
				}
		}
	}
	mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
	return cc;
}
