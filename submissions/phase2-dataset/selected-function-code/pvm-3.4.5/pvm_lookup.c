int
pvm_lookup(name, req, datap)
	char *name;		/* class name */
	int req;		/* req class index or -1 for any */
	int *datap;		/* data return */
{
	int sbf, rbf, cc;
	int flags;
	int mid;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_LOOKUP,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_CN, TEV_DATA_SCALAR,
				name ? name : "", 1, 1 );
			TEV_PACK_INT( TEV_DID_CI, TEV_DATA_SCALAR, &req, 1, 1);
			TEV_FIN;
		}
	}

	if (!name || !*name || req < -1) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {

			flags = PvmMboxDefault;

			if ( req < 0 ) {
				flags |= PvmMboxFirstAvail;
				req = 0;
			}

			rbf = pvm_setrbuf(0);

			sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
			cc = TMDB_GET;
			pvm_pkint(&cc, 1, 1);
			pvm_pkint(&pvmmytid, 1, 1);
			pvm_pkstr(name);
			pvm_pkint(&req, 1, 1);
			pvm_pkint(&flags, 1, 1);
			if ((cc = msendrecv(TIDPVMD, TM_DB, SYSCTX_TM)) > 0) {
				pvm_upkint(&cc, 1, 1);
				if (cc >= 0) {
					mid = pvm_upkmesg();
					pvm_freebuf(pvm_setrbuf(mid));
				}
			}
			pvm_freebuf(pvm_setsbuf(sbf));

			if ( cc >= 0 && datap )
				pvm_upkint(datap, 1, 1);
			
			pvm_freebuf(pvm_setrbuf(rbf));
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_LOOKUP,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		if (cc != PvmNoEntry)
			lpvmerr("pvm_lookup", cc);
		else
			pvm_errno = cc;
	return cc;
}
