int
pmsg_unpack(mp, mp2)
	struct pmsg *mp;		/* message to unpack from */
	struct pmsg *mp2;		/* blank message to write on */
{
	struct frag *fp;
	int cc;
	int mlen;
	int frl;

	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mlen, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mp2->m_ctx, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mp2->m_tag, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mp2->m_wid, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mp2->m_enc, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mp2->m_crc, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mp2->m_src, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mp2->m_dst, 1, 1, sizeof(int)))
		return cc;

	mp2->m_len = 0;

	if (mlen < 0)
		cc = PvmBadMsg;
	else {
		while (mlen > 0) {
			if (cc = (mp->m_codef->dec_int)
					(mp, (void*)&frl, 1, 1, sizeof(int)))
				break;
			if (!(fp = fr_new(frl + MAXHDR))) {
				cc = PvmNoMem;
				break;
			}
			fp->fr_dat += MAXHDR;
			fp->fr_len = frl;
			if (cc = (mp->m_codef->dec_byte)
					(mp, (void*)fp->fr_dat, frl, 1, 1))
				break;
			LISTPUTBEFORE(mp2->m_frag, fp, fr_link, fr_rlink);
			mp2->m_len += frl;
			mlen -= frl;
		}
		pmsg_setenc(mp2, mp2->m_enc);
	}
	return cc;
}
