static int
dec_xdr_cplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register float *xp;
	int cc = 0;

	std = std * 2 - 1;
	for (xp = (float*)vp; cnt-- > 0; xp += std) {
		if (!xdr_float(&mp->m_xdr, xp)) {
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_float(&mp->m_xdr, xp)) {
					cc = PvmNoData;
					break;
				}
		}
		xp++;
		if (!xdr_float(&mp->m_xdr, xp)) {
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_float(&mp->m_xdr, xp)) {
					cc = PvmNoData;
					break;
				}
		}
	}
	mp->m_cpos = xdr_getpos(&mp->m_xdr);
	return cc;
}
