pvm_pstat(tid)
	int tid;	/* task */
{
	int sbf, rbf;
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PSTAT,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_TT, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		if (!TIDISTASK(tid))
			cc = PvmBadParam;

		else {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			pvm_pkint(&tid, 1, 1);
			if ((cc = msendrecv(TIDPVMD, TM_PSTAT, SYSCTX_TM)) > 0) {
				pvm_upkint(&cc, 1, 1);
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PSTAT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_TST, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0 && cc != PvmNoTask)
		lpvmerr("pvm_pstat", cc);
	return cc;
}
