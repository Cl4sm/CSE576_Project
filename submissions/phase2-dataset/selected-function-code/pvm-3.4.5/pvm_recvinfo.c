int
pvm_recvinfo(name, index, flags)
	char *name;		/* class name */
	int index;		/* req class index or -1 for any */
	int flags;		/* options */
{
	int sbf, cc;
	int mid = -1;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_GETINFO,TEV_EVENT_ENTRY)) {
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
			if (pvmrbuf)
				umbuf_free(pvmrbuf);
			pvmrbuf = 0;

			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			cc = TMDB_GET;
			pvm_pkint(&cc, 1, 1);
			pvm_pkint(&pvmmytid, 1, 1);
			pvm_pkstr(name);
			pvm_pkint(&index, 1, 1);
			pvm_pkint(&flags, 1, 1);
			if ((cc = msendrecv(TIDPVMD, TM_DB, SYSCTX_TM)) > 0) {
				pvm_upkint(&cc, 1, 1);
				if (cc >= 0) {
					mid = pvm_upkmesg();
					pvm_freebuf(pvm_setrbuf(mid));
				}
			}
			pvm_freebuf(pvm_setsbuf(sbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_GETINFO,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_PACK_INT( TEV_DID_CR, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if ( cc < 0 ) {
		if ( cc != PvmNotFound )
			lpvmerr("pvm_recvinfo", cc);
		else
			pvm_errno = cc;
		return cc;
	}
	else
		return mid;
}
