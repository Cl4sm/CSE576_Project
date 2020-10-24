dec_xdr_byte(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	struct frag *fp;
	int cc;

	fp = mp->m_cfrag;
	if (cc = byteupk(mp, (char*)vp, cnt, 1, std))
		return cc;
	if (fp != mp->m_cfrag) {
		fp = mp->m_cfrag;
		xdrmem_create(&mp->m_xdr, fp->fr_dat, (unsigned)(fp->fr_len),
				XDR_DECODE);
	}
	mp->m_cpos = (mp->m_cpos + 3) & ~3;
	xdr_setpos(&mp->m_xdr, mp->m_cpos);
	return 0;
}
