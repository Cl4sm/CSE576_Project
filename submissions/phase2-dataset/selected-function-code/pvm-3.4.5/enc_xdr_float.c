static int
enc_xdr_float(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register float *fp;
	int cc = 0;

	for (fp = (float*)vp; cnt-- > 0; fp += std)
		if (!xdr_float(&mp->m_xdr, fp)) {
			mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
			if (cc = enc_xdr_step(mp))
				break;
			else
				if (!xdr_float(&mp->m_xdr, fp)) {
					cc = PvmNoMem;
					break;
				}
		}
	mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
	return cc;
}
