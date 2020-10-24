int
pvm_delinfo(name, index, flags)
	char *name;		/* class name */
	int index;		/* class index or -1 for all */
	int flags;		/* options */
{
	int sbf, rbf, cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_DELINFO,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_CN, TEV_DATA_SCALAR,
				name ? name : "", 1, 1 );
			TEV_PACK_INT( TEV_DID_CI, TEV_DATA_SCALAR, &index, 1, 1 );
			TEV_PACK_INT( TEV_DID_CF, TEV_DATA_SCALAR, &flags, 1, 1 );
			TEV_FIN;
		}
	}

	if (!name || !*name || index < 0) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			cc = TMDB_REMOVE;
			pvm_pkint(&cc, 1, 1);
			pvm_pkint(&pvmmytid, 1, 1);
			pvm_pkstr(name);
			pvm_pkint(&index, 1, 1);
			pvm_pkint(&flags, 1, 1);
			if ((cc = msendrecv(TIDPVMD, TM_DB, SYSCTX_TM)) > 0) {
				pvm_upkint(&cc, 1, 1);
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_DELINFO,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		if (cc != PvmDenied && cc != PvmNotFound)
			lpvmerr("pvm_delinfo", cc);
		else
			pvm_errno = cc;
	return cc;
}
