dec_xdr_ulong(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register unsigned long *np;
	int cc = 0;

	for (np = (unsigned long*)vp; cnt-- > 0; np += std)
#ifdef USE_XDR_LONGLONG
		if (!xdr_u_longlong_t(&mp->m_xdr, np))
#else
		if (!xdr_u_long(&mp->m_xdr, np))
#endif
		{
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
#ifdef USE_XDR_LONGLONG
				if (!xdr_u_longlong_t(&mp->m_xdr, np))
#else
				if (!xdr_u_long(&mp->m_xdr, np))
#endif
				{
					cc = PvmNoData;
					break;
				}
		}
	mp->m_cpos = xdr_getpos(&mp->m_xdr);
	return cc;
}
