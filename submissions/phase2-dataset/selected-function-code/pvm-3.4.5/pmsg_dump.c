pmsg_dump(mp, lvl)
	struct pmsg *mp;
	int lvl;			/* 0 - summary, 1 - frag lengths, 2 - frag data */
{
	struct frag *fp;

	if (mp->m_flag & MM_PACK)
		pmsg_setlen(mp);
#ifdef	MCHECKSUM
	pvmlogprintf(
	"pmsg_dump(0x%x) ref=%d mid=%d len=%d ctx=0x%x tag=%d wid=0x%x src=0x%x dst=0x%x enc=0x%x flag=%d crc=0x%x\n",
			mp, mp->m_ref, mp->m_mid, mp->m_len,
			mp->m_ctx, mp->m_tag, mp->m_wid, mp->m_src, mp->m_dst,
			mp->m_enc, mp->m_flag, pmsg_crc(mp));

#else
	pvmlogprintf(
	"pmsg_dump(0x%x) ref=%d mid=%d len=%d ctx=0x%x tag=%d wid=0x%x src=0x%x dst=0x%x enc=0x%x flag=%d\n",
			mp, mp->m_ref, mp->m_mid, mp->m_len,
			mp->m_ctx, mp->m_tag, mp->m_wid, mp->m_src, mp->m_dst,
			mp->m_enc, mp->m_flag);
#endif
	if (lvl > 0) {
		for (fp = mp->m_frag->fr_link; fp != mp->m_frag; fp = fp->fr_link) {
			pvmlogprintf(" frag=0x%x max=%d ofs=%d len=%d\n",
				fp, fp->fr_max, fp->fr_dat - fp->fr_buf, fp->fr_len);
			if (lvl > 1)
				pvmhdump(fp->fr_dat, fp->fr_len, "  ");
		}
	}
	return 0;
}
