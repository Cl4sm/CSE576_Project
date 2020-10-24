int
pvm_getminfo(mid, info)
	int mid;
	struct pvmminfo *info;
{
	struct pmsg *mp;
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_GETMINFO,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
	}

	if (mid <= 0)
		cc = PvmBadParam;
	else if (mp = midtobuf(mid)) {
		if (mp->m_flag & MM_PACK)
			pmsg_setlen(mp);
		info->len = mp->m_len;
		info->ctx = mp->m_ctx;
		info->tag = mp->m_tag;
		info->wid = mp->m_wid;
		info->enc = mp->m_enc;
		info->crc = mp->m_crc;
		info->src = mp->m_src;
		info->dst = mp->m_dst;
		cc = PvmOk;
	} else
		cc = PvmNoSuchBuf;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_GETMINFO,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_getminfo", cc);
	return cc;
}
