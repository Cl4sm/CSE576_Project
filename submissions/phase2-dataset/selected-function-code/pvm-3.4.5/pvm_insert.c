int
pvm_insert(name, req, data)
	char *name;		/* class name */
	int req;		/* requested class index or -1 for any */
	int data;
{
	int sbf, rbf, cc;
	int flags;
	int mid;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_INSERT,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_CN, TEV_DATA_SCALAR,
				name ? name : "", 1, 1 );
			TEV_PACK_INT( TEV_DID_CI, TEV_DATA_SCALAR, &req, 1, 1 );
			TEV_PACK_INT( TEV_DID_CD, TEV_DATA_SCALAR, &data, 1, 1 );
			TEV_FIN;
		}
	}

	if (!name || !*name || req < -1) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {

			flags = PvmMboxDefault
				| PvmMboxMultiInstance
				| PvmMboxPersistent;

			mid = pvm_mkbuf(PvmDataFoo);
			sbf = pvm_setsbuf(mid);
			pvm_pkint(&data, 1, 1);

			pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			cc = TMDB_PUT;
			pvm_pkint(&cc, 1, 1);
			pvm_pkint(&pvmmytid, 1, 1);
			pvm_pkstr(name);
			pvm_pkint(&req, 1, 1);
			pvm_pkint(&flags, 1, 1);
			pvm_pkmesg(mid);
			if ((cc = msendrecv(TIDPVMD, TM_DB, SYSCTX_TM)) > 0) {
				pvm_upkint(&cc, 1, 1);
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
			pvm_freebuf(mid);
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_INSERT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		if (cc != PvmDupEntry)
			lpvmerr("pvm_insert", cc);
		else
			pvm_errno = cc;
	return cc;
}
