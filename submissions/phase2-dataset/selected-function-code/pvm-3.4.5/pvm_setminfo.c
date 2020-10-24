pvm_setminfo(mid, info)
	int mid;
	struct pvmminfo *info;
{
	struct pmsg *mp;
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SETMINFO,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
	}

	if (mid <= 0)
		cc = PvmBadParam;
	else if (mp = midtobuf(mid)) {
		mp->m_ctx = info->ctx;
		mp->m_tag = info->tag;
		mp->m_wid = info->wid;
		mp->m_src = info->src;
		mp->m_dst = info->dst;
		cc = PvmOk;
	} else
		cc = PvmNoSuchBuf;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SETMINFO,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_setminfo", cc);
	return cc;
}
