int
pvm_tickle(narg, argp, nresp, resp)
	int narg;
	int *argp;
	int *nresp;
	int *resp;
{
	int cc;
	int sbf, rbf;
	int nres;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_TICKLE,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_TA, TEV_DATA_ARRAY, argp, narg, 1 );
			TEV_FIN;
		}
	}

	if (!(cc = BEATASK)) {
		if (narg < 1 || narg > 10)
			cc = PvmBadParam;

		else {

			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			pvm_pkint(&narg, 1, 1);
			pvm_pkint(argp, narg, 1);
			if ((cc = msendrecv(TIDPVMD, TM_TICKLE, SYSCTX_TM)) > 0) {
				pvm_upkint(&nres, 1, 1);
				if (nresp)
					*nresp = nres;
				if (resp)
					pvm_upkint(resp, nres, 1);
				pvm_freebuf(pvm_setrbuf(rbf));
				cc = 0;

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_TICKLE,TEV_EVENT_EXIT)) {
			int tevcount;
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			tevcount = ( resp ) ? nres : 0;
			TEV_PACK_INT( TEV_DID_TR, TEV_DATA_ARRAY,
				resp, tevcount, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_tickle", cc);
	return cc;
}
