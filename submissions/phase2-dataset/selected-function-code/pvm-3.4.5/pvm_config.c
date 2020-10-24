int
pvm_config(nhostp, narchp, hostp)
	int *nhostp;
	int *narchp;
	struct pvmhostinfo **hostp;
{
	int sbf, rbf, cc;
	static int nhost = 0;
	static int narch = 0;
	static struct pvmhostinfo *hlist = 0;
	int i;
	/* char buf[256]; XXX static limit, argh (Not Any More! :-) JAK */
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_CONFIG,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	if (hlist) {
		while (nhost-- > 0) {
			PVM_FREE(hlist[nhost].hi_name);
			PVM_FREE(hlist[nhost].hi_arch);
		}
		PVM_FREE(hlist);
		hlist = 0;
		nhost = 0;
	}
	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);
		if (pvmschedtid)
			cc = msendrecv(pvmschedtid, SM_CONFIG, PvmBaseContext);
		else
			cc = msendrecv(TIDPVMD, TM_CONFIG, SYSCTX_TM);
		if (cc > 0) {
			pvm_upkint(&nhost, 1, 1);
			pvm_upkint(&narch, 1, 1);
			hlist = TALLOC(nhost, struct pvmhostinfo, "hi");
			for (i = 0; i < nhost; i++) {
				pvm_upkint(&hlist[i].hi_tid, 1, 1);
				pvmupkstralloc(&(hlist[i].hi_name));
				pvmupkstralloc(&(hlist[i].hi_arch));
				pvm_upkint(&hlist[i].hi_speed, 1, 1);
				pvm_upkint(&hlist[i].hi_dsig, 1, 1);
			}
			pvm_freebuf(pvm_setrbuf(rbf));
			if (nhostp)
				*nhostp = nhost;
			if (narchp)
				*narchp = narch;
			if (hostp)
				*hostp = hlist;
			cc = 0;
		}
		pvm_freebuf(pvm_setsbuf(sbf));
		pvm_setrbuf(rbf);
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_CONFIG,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_NH, TEV_DATA_SCALAR, &nhost, 1, 1 );
			TEV_PACK_INT( TEV_DID_NA, TEV_DATA_SCALAR, &narch, 1, 1 );
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_config", cc);
	return cc;
}
