pvm_delete(name, req)
	char *name;		/* class name */
	int req;		/* class index or -1 for all */
{
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_DELETE,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_CN, TEV_DATA_SCALAR,
				name ? name : "", 1, 1 );
			TEV_PACK_INT( TEV_DID_CI, TEV_DATA_SCALAR, &req, 1, 1 );
			TEV_FIN;
		}
	}

	if (!name || !*name || req < 0) {
		cc = PvmBadParam;

	} else {
		if (!(cc = BEATASK)) {
			cc = pvm_delinfo( name, req, PvmMboxDefault );
		}
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_DELETE,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		if (cc != PvmNoEntry)
			lpvmerr("pvm_delete", cc);
		else
			pvm_errno = cc;
	return cc;
}
