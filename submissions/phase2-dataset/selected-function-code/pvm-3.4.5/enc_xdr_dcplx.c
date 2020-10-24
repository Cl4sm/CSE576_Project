static int
enc_xdr_dcplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register double *zp;
	int cc = 0;

	std = std * 2 - 1;
	for (zp = (double*)vp; cnt-- > 0; zp += std) {
		if (!xdr_double(&mp->m_xdr, zp)) {
			if (cc = enc_xdr_step(mp))
				break;
			else
				if (!xdr_double(&mp->m_xdr, zp)) {
					cc = PvmNoMem;
					break;
				}
		}
		mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
		zp++;
		if (!xdr_double(&mp->m_xdr, zp)) {
			if (cc = enc_xdr_step(mp))
				break;
			else
				if (!xdr_double(&mp->m_xdr, zp)) {
					cc = PvmNoMem;
					break;
				}
		}
		mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
	}
	return cc;
}
