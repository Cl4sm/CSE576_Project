enc_xdr_byte(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	struct frag *fp;
	int cc;

	fp = mp->m_frag->fr_rlink;
	if (cc = bytepk(mp, (char*)vp, cnt, 1, std))
		return cc;
	if (fp != mp->m_frag->fr_rlink) {
		fp = mp->m_frag->fr_rlink;
		xdrmem_create(&mp->m_xdr,
				fp->fr_dat,
				(unsigned)(fp->fr_max - (fp->fr_dat - fp->fr_buf)),
				XDR_ENCODE);
	}
	fp->fr_len = (fp->fr_len + 3) & ~3;
	xdr_setpos(&mp->m_xdr, fp->fr_len);
	return 0;
}
