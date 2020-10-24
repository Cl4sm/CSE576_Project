pmsg_packbody(mp, mp2)
	struct pmsg *mp;		/* message to pack into */
	struct pmsg *mp2;		/* message to include body of */
{
	struct frag *fp, *fp2;
	int cc;

	if (mp->m_enc != mp2->m_enc)
		cc = PvmMismatch;
	else {
		cc = 0;
		if ((fp = mp2->m_frag) && fp->fr_link != fp) {
			fp2 = mp->m_frag->fr_rlink;
			if (fp2 != mp->m_frag && fp2->fr_len == 0) {
				LISTDELETE(fp2, fr_link, fr_rlink);
				fr_unref(fp2);
			}
			while ((fp = fp->fr_link) != mp2->m_frag) {
				fp2 = fr_new(0);
				fp2->fr_buf = fp->fr_buf;
				fp2->fr_max = fp->fr_max;
				fp2->fr_dat = fp->fr_dat;
				fp2->fr_len = fp->fr_len;
				da_ref(fp->fr_buf);
				LISTPUTBEFORE(mp->m_frag, fp2, fr_link, fr_rlink);
			}
		}
	}
	return cc;
}
