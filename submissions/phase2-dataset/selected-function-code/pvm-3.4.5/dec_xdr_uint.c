dec_xdr_uint(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register unsigned int *np;
	int cc = 0;

	for (np = (unsigned int*)vp; cnt-- > 0; np += std)
		if (!xdr_u_int(&mp->m_xdr, np)) {
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_u_int(&mp->m_xdr, np)) {
					cc = PvmNoData;
					break;
				}
		}
	mp->m_cpos = xdr_getpos(&mp->m_xdr);
	return cc;
}
