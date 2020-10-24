int
pvm_putinfo(name, mid, flags)
	char *name;			/* class name */
	int mid;			/* message to store */
	int flags;			/* options */
{
	int index = -1;
	int sbf, rbf, cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PUTINFO,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_CN, TEV_DATA_SCALAR,
				name ? name : "", 1, 1 );
			TEV_PACK_INT( TEV_DID_CI, TEV_DATA_SCALAR, &index, 1, 1 );
			TEV_PACK_INT( TEV_DID_CF, TEV_DATA_SCALAR, &flags, 1, 1);
			TEV_PACK_INT( TEV_DID_CD, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
	}

	if (!name || !*name || index < -1) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {
			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			rbf = pvm_setrbuf(0);
			cc = TMDB_PUT;
			pvm_pkint(&cc, 1, 1);
			pvm_pkint(&pvmmytid, 1, 1);
			pvm_pkstr(name);
			pvm_pkint(&index, 1, 1);
			pvm_pkint(&flags, 1, 1);
			pvm_pkmesg(mid);
			if ((cc = msendrecv(TIDPVMD, TM_DB, SYSCTX_TM)) > 0) {
				pvm_upkint(&cc, 1, 1);
				pvm_freebuf(pvm_setrbuf(rbf));

			} else
				pvm_setrbuf(rbf);
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PUTINFO,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		if (cc != PvmDenied && cc != PvmExists)
			lpvmerr("pvm_putinfo", cc);
		else
			pvm_errno = cc;
	return cc;
}
