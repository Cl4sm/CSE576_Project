int
umbuf_dump(mid, lvl)
	int mid;
	int lvl;	/* 0 - summary, 1 - frag lengths, 2 - frag data */
{
	struct pmsg *mp;
	struct frag *fp;

	if (mid <= 0) {
		pvmlogprintf("umbuf_dump() BadParam\n");
		return PvmBadParam;
	}
	if (!(mp = midtobuf(mid))) {
		pvmlogprintf("umbuf_dump() NoSuchBuf\n");
		return PvmNoSuchBuf;
	}

	mp = pvmmidh[mid].m_umb;
	pmsg_dump(mp, lvl);
	return 0;
}
