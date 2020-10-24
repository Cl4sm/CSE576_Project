int
pvm_freecontext(cid)
	int cid;
{
	int sbf, rbf, cc;
	int mid = -1;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_FREECONTEXT,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_CXF, TEV_DATA_SCALAR, &cid, 1, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);
		pvm_pkint(&cid, 1, 1);
		if ((cc = msendrecv(TIDPVMD, TM_CONTEXT, SYSCTX_TM)) > 0) {
			pvm_upkint(&cc, 1, 1);
			pvm_freebuf(pvm_setrbuf(rbf));

		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_FREECONTEXT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_freecontext", cc);
	return cc;
}
