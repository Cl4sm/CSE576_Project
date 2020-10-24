int
pvm_getcontext()
{
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_GETCONTEXT,TEV_EVENT_ENTRY)) {
			TEV_FIN;
		}
	}

	/* Yep, overkill... */

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_GETCONTEXT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CXC, TEV_DATA_SCALAR,
				&pvmmyctx, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	return pvmmyctx;
}
