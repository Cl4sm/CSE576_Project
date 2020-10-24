dec_xdr_short(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register short *np;
	int cc = 0;

	for (np = (short*)vp; cnt-- > 0; np += std)
		if (!xdr_short(&mp->m_xdr, np)) {
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_short(&mp->m_xdr, np)) {
					cc = PvmNoData;
					break;
				}
		}
	mp->m_cpos = xdr_getpos(&mp->m_xdr);
	return cc;
}
