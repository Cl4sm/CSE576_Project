struct pmsg *
pmsg_new(master)
	int master;			/* true if a master (no data) node */
{
	struct pmsg *mp;

	if (!(mp = pmsg_get()))
		goto fail;

	mp->m_ref = 1;
	if (master) {
		mp->m_link = mp->m_rlink = mp;
		mp->m_frag = 0;
	} else {
		mp->m_link = mp->m_rlink = 0;
		if (!(mp->m_frag = fr_new(0))) {
			PVM_FREE(mp);
			goto fail;
		}
	}
	mp->m_codef = 0;
	mp->m_cfrag = 0;
	mp->m_mid = 0;
	mp->m_len = 0;
	mp->m_ctx = 0;
	mp->m_tag = 0;
	mp->m_wid = 0;
	mp->m_src = 0;
	mp->m_dst = 0;
	mp->m_enc = 0;
	mp->m_flag = 0;
	mp->m_cpos = 0;
	mp->m_crc = 0;
	return mp;

fail:
	return (struct pmsg *)0;
}
