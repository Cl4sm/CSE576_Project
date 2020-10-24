enc_xdr_double(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register double *dp;
	int cc = 0;
	int n;

	for (dp = (double*)vp; cnt-- > 0; dp += std) {
		if (!xdr_double(&mp->m_xdr, dp)) {
			if (cc = enc_xdr_step(mp))
				break;
			else
				if (!xdr_double(&mp->m_xdr, dp)) {
					cc = PvmNoMem;
					break;
				}
		}
		mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
	}
	return cc;
}
