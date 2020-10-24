int
pvm_sendsig(tid, signum)
	int tid;
	int signum;
{
	int cc;
	int sbf, rbf;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SENDSIG,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_TT, TEV_DATA_SCALAR, &tid, 1, 1 );
			TEV_PACK_INT( TEV_DID_SN, TEV_DATA_SCALAR, &signum, 1, 1 );
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
			pvm_pkint(&signum, 1, 1);
			if ((cc = msendrecv(TIDPVMD, TM_SENDSIG, SYSCTX_TM)) > 0) {
				pvm_freebuf(pvm_setrbuf(rbf));
				cc = 0;

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SENDSIG,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_sendsig", cc);
	return cc;
}
