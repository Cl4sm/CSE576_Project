int
pvm_mstat(host)
	char *host;
{
	int sbf, rbf, cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_MSTAT,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_HN, TEV_DATA_SCALAR,
				host ? host : "", 1, 1 );
			TEV_FIN;
		}
	}
	if (!host || !*host) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			pvm_pkstr(host);
			if ((cc = msendrecv(TIDPVMD, TM_MSTAT, SYSCTX_TM)) > 0) {
				pvm_upkint(&cc, 1, 1);
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_MSTAT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_HS, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0 && cc != PvmNoHost && cc != PvmHostFail)
		lpvmerr("pvm_mstat", cc);
	return cc;
}
