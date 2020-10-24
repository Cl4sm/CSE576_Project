int
pvm_setsbuf(mid)
	int mid;
{
	struct pmsg *mp = 0;
	int cc = 0;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SETSBUF,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
	}

	if (mid < 0) {
		cc = PvmBadParam;
	} else if (mid == 0) {
		cc = pvmsbuf ? pvmsbuf->m_mid : 0;
		pvmsbuf = 0;
	} else if (mp = midtobuf(mid)) {
		if (mp == pvmrbuf)
			pvmrbuf = 0;
		cc = pvmsbuf ? pvmsbuf->m_mid : 0;
		pvmsbuf = mp;
		(mp->m_codef->enc_init)(mp);
	} else
		cc = PvmNoSuchBuf;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SETSBUF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_MSB, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_setsbuf", cc);
	return cc;
}
