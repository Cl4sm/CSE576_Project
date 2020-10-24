static int
dec_xdr_dcplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register double *zp;
	int cc = 0;

	std = std * 2 - 1;
	for (zp = (double*)vp; cnt-- > 0; zp += std) {
		if (!xdr_double(&mp->m_xdr, zp)) {
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_double(&mp->m_xdr, zp)) {
					cc = PvmNoData;
					break;
				}
		}
		mp->m_cpos = xdr_getpos(&mp->m_xdr);
		zp++;
		if (!xdr_double(&mp->m_xdr, zp)) {
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_double(&mp->m_xdr, zp)) {
					cc = PvmNoData;
					break;
				}
		}
		mp->m_cpos = xdr_getpos(&mp->m_xdr);
	}
	return cc;
}
