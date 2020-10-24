int
pvm_freebuf(mid)
	int mid;		/* message handle */
{
	int cc;
	struct pmsg *mp;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_FREEBUF,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
	}

	if (mid < 0) {
		cc = PvmBadParam;
	} else if (mp = midtobuf(mid)) {
		if (pvmsbuf == mp)
			pvmsbuf = 0;
		if (pvmrbuf == mp)
			pvmrbuf = 0;
		umbuf_free(mp);
		cc = 0;
	} else if (mid)
		cc = PvmNoSuchBuf;
	else
		cc = PvmOk;	/* don't complain when freeing the null/0 buffer */
				/* many 3.3 codes switch and free without checking */

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_FREEBUF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_freebuf", cc);
	return cc;
}
