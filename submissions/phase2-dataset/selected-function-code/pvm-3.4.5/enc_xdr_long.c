enc_xdr_long(mp, vp, cnt, std, siz)
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
#ifdef USE_XDR_LONGLONG
			if ((*np & ~(long)0x7fffffffffffffff)
			&& (*np & ~(long)0x7fffffffffffffff)
					!= ~(long)0x7fffffffffffffff)
#else
			if ((*np & ~(long)0x7fffffff)
			&& (*np & ~(long)0x7fffffff) != ~(long)0x7fffffff)
#endif
			{
				cc = PvmOverflow;
				break;
			}
			mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
			if (cc = enc_xdr_step(mp))
				break;
			else
#ifdef USE_XDR_LONGLONG
				if (!xdr_longlong_t(&mp->m_xdr, np))
#else
				if (!xdr_long(&mp->m_xdr, np))
#endif
				{
					cc = PvmNoMem;
					break;
				}
		}
	mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
	return cc;
}
