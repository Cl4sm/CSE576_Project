static int
enc_xdr_init(mp)
	struct pmsg *mp;
{
	struct frag *fp;
	int cc;

	if (!(mp->m_flag & MM_PACK)) {
		mp->m_flag &= ~MM_UPACK;
		mp->m_flag |= MM_PACK;
		if ((fp = mp->m_frag->fr_link) == mp->m_frag) {
			if (cc = pmsg_extend(mp))
				return cc;
			fp = fp->fr_link;
		}

		xdrmem_create(&mp->m_xdr,
				fp->fr_dat,
				(unsigned)(fp->fr_max - (fp->fr_dat - fp->fr_buf)),
				XDR_ENCODE);
	}
	return 0;
}
