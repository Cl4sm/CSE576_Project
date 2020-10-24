dec_xdr_double(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register double *dp;
	int cc = 0;

	for (dp = (double*)vp; cnt-- > 0; dp += std) {
		if (!xdr_double(&mp->m_xdr, dp)) {
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_double(&mp->m_xdr, dp)) {
					cc = PvmNoData;
					break;
				}
		}
		mp->m_cpos = xdr_getpos(&mp->m_xdr);
	}
	return cc;
}
