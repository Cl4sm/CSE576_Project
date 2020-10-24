static int
dec_xdr_long(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register long *np;
	int cc = 0;

	for (np = (long*)vp; cnt-- > 0; np += std)
#ifdef USE_XDR_LONGLONG
		if (!xdr_longlong_t(&mp->m_xdr, np))
#else
		if (!xdr_long(&mp->m_xdr, np))
#endif
		{
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
#ifdef USE_XDR_LONGLONG
				if (!xdr_longlong_t(&mp->m_xdr, np))
#else
				if (!xdr_long(&mp->m_xdr, np))
#endif
				{
					cc = PvmNoData;
					break;
				}
		}
	mp->m_cpos = xdr_getpos(&mp->m_xdr);
	return cc;
}
