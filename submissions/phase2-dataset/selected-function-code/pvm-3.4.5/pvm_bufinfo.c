int
pvm_bufinfo(mid, len, code, tid)
	int mid;
	int *len;
	int *code;
	int *tid;
{
	struct pmsg *mp;
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_BUFINFO,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
	}

	if (mid <= 0)
		cc = PvmBadParam;
	else
		if (mp = midtobuf(mid)) {
			cc = PvmOk;
			if (len) {
				if (mp->m_flag & MM_PACK)
					pmsg_setlen(mp);
				*len = mp->m_len;
			}
			if (code)
				*code = mp->m_tag;
			if (tid)
				*tid = mp->m_src;
		} else
			cc = PvmNoSuchBuf;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_BUFINFO,TEV_EVENT_EXIT)) {
			int ln, tg, sc;
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			if (!cc) {
				ln = mp->m_len;
				tg = mp->m_tag;
				sc = mp->m_src;
			}
			else
				ln = tg = sc = cc;
			TEV_PACK_INT( TEV_DID_INB, TEV_DATA_SCALAR, &ln, 1, 1 );
			TEV_PACK_INT( TEV_DID_IMC, TEV_DATA_SCALAR, &tg, 1, 1 );
			TEV_PACK_INT( TEV_DID_IST, TEV_DATA_SCALAR, &sc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_bufinfo", cc);
	return cc;
}
