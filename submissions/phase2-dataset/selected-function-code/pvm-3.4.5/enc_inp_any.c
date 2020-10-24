static int
enc_inp_any(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	struct frag *fp;

	if (std != 1)
		return PvmNotImpl;
	if (vp && cnt && siz) {
		fp = fr_snew((char *)vp, cnt * siz);
		LISTPUTBEFORE(mp->m_frag, fp, fr_link, fr_rlink);
	}
#if 0
	if (std != 1) {
		if (std < 1)
			return PvmBadParam;
		fp->fr_max = fp->fr_len = siz + (cnt - 1) * std * siz;
		fp->fr_csz = siz;
		fp->fr_lnc = std * siz;
		fp->fr_u.spr = 1;
	}
#endif
	return 0;
}
