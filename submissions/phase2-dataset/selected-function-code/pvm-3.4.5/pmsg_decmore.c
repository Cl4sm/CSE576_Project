pmsg_decmore(mp)
	struct pmsg *mp;
{
#if 0
	mp->m_cpos = 0;
	if (mp->m_cfrag == mp->m_frag)		/* no message left */
		return PvmNoData;
	mp->m_cfrag = mp->m_cfrag->fr_link;
	if (mp->m_cfrag == mp->m_frag)
		return PvmNoData;
	return 0;
#endif
	mp->m_cpos = 0;
	if (mp->m_cfrag == mp->m_frag)		/* no message left */
		return PvmNoData;
	while ((mp->m_cfrag = mp->m_cfrag->fr_link) != mp->m_frag)
		if (mp->m_cfrag->fr_len > 0)
			break;
	if (mp->m_cfrag == mp->m_frag)
		return PvmNoData;
	return 0;
}
