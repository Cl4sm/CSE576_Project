pmsg_pack(mp, mp2)
	struct pmsg *mp;		/* message to pack into */
	struct pmsg *mp2;		/* message to include */
{
	struct frag *fp;
	int cc;

	if (mp2->m_flag & MM_PACK)
		pmsg_setlen(mp2);

	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_len, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_ctx, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_tag, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_wid, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_enc, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_crc, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_src, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_dst, 1, 1, sizeof(int)))
		return cc;
	if (fp = mp2->m_frag)
		while ((fp = fp->fr_link) != mp2->m_frag) {
			if (cc = (mp->m_codef->enc_int)
					(mp, (void*)&fp->fr_len, 1, 1, sizeof(int)))
				return cc;
			if (cc = (mp->m_codef->enc_byte)
					(mp, (void*)fp->fr_dat, fp->fr_len, 1, 1))
				return cc;
		}
	return cc;
}
